#include "Game.h"
#include "ObjectList.h"
#include "Constants.h"
#include "FileUtils.h"
#include "StringUtils.h"
#include "TWCallbackFunctions.h"

#include "SimpleMath.h"

#include <windows.h>
#include <time.h>

Game::Game(ID3D11Device* _d3d_device, HWND _hWnd, HINSTANCE _hInstance)
    : hWnd_(_hWnd)
{
	//set up audio
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
#endif
	audio_engine_.reset(new AudioEngine(eflags));

	//Create DirectXTK spritebatch stuff
	ID3D11DeviceContext* pd3dImmediateContext;
    _d3d_device->GetImmediateContext(&pd3dImmediateContext);
	DD2D_.sprites.reset(new SpriteBatch(pd3dImmediateContext));
	DD2D_.font.reset(new SpriteFont(_d3d_device, L"..\\Assets\\italic.spritefont"));

	//seed the random number generator
	srand((UINT)time(NULL));

    fx_factory_ = std::make_unique<EffectFactory>(_d3d_device);
    states_ = std::make_unique<CommonStates>(_d3d_device);

    //Tell the fxFactory to look to the correct build directory to pull stuff in from
#ifdef DEBUG
    ((EffectFactory*)fx_factory_.get())->SetDirectory(L"../Debug");
#else
    ((EffectFactory*)fx_factory_.get())->SetDirectory(L"../Release");
#endif

    // Core systems.
    input_handler_ = std::make_unique<InputHandler>(_hWnd, _hInstance);
    cmo_manager_ = std::make_unique<CMOManager>(*_d3d_device, *fx_factory_);
    boid_manager_ = std::make_unique<BoidManager>(*cmo_manager_);

    enumerate_boid_types();
    init_tweak_bar(_d3d_device);

    // Configure GameData struct.
    GD_.game_state = GS_SIMULATING;
    GD_.active_camera = CAM_TT;
    GD_.input_handler = input_handler_.get();

	// Init render system for VBGOs.
	VBGO::init(_d3d_device);

	// Calculate aspect ratio.
	RECT rc;
	GetClientRect(hWnd_, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;
	float AR = (float)width / (float)height;

	// Base light.
	auto light = std::make_unique<Light>(Vector3(0.0f, 100.0f, 160.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Color(0.5f, 0.5f, 0.5f, 1.0f));
    light_ = light.get();
	game_objects_.push_back(std::move(light));

    // Tabletop Simulator style camera to orbit around the simulation.
    tabletop_camera_ = std::make_unique<TabletopCamera>(0.25f * XM_PI, AR, 1.0f, 10000.0f, Vector3::UnitY, 0.5f, 150.0f);

	// Player to fly around the scene when TPS is set up.
	auto player = std::make_unique<Player>(cmo_manager_->get_model("BirdModelV1"));
    player->set_pos({ 0, 10, 0 });

	// TPS Camera to follow the Player's movements.
	tps_camera_ = std::make_unique<TPSCamera>(0.25f * XM_PI, AR, 1.0f, 10000.0f, player.get(), Vector3::UnitY, Vector3(0.0f, 10.0f, 50.0f));
    game_objects_.push_back(std::move(player));

	// Configure DrawData struct.
	DD_.d3d_immediate_context = nullptr;
	DD_.states = states_.get();
	DD_.camera = tabletop_camera_.get();
	DD_.light = light_;

	// Add terrain for boids to run around on.
	auto terrain = std::make_unique<FileVBGO>("../Assets/terrainTex.txt", _d3d_device);
    terrain->set_scale(10.0f, 0.5f, 10.0f);
    terrain->set_pos({ 0, -0.5f, 0 });
	game_objects_.push_back(std::move(terrain));
};

Game::~Game() 
{
	// Tidy up VBGO render system.
	VBGO::clean_up();

    TwTerminate();
}

bool Game::tick() 
{
    // Crude delta-time system.
    GD_.delta_time = timer_.getTimeDifference();
    timer_.reset();

    // Tick core systems.
    input_handler_->tick();

    // Tick cameras.
    tabletop_camera_->tick(&GD_);
    tps_camera_->tick(&GD_);

	// Tick audio engine.
	if (!audio_engine_->Update())
	{
		// No audio device is active.
		if (audio_engine_->IsCriticalError())
		{
			return false;
		}
	}

    // Quit on Escape key press.
	if (input_handler_->get_key_down(DIK_ESCAPE))
	{
		return false;
	}

    handle_pause();
    handle_camera_change();

	switch (GD_.game_state)
	{
        case GS_SIMULATING:
        {
            simulating_tick();
        } break;

        default: {}
	}

    tick_all_objects();
	
	return true;
};

void Game::draw_pause_text()
{
    if (GD_.game_state == GS_PAUSED)
    {
        DD2D_.font->DrawString(DD2D_.sprites.get(),
            StringUtils::char_to_wchar("PAUSED"),
            XMFLOAT2(440, 0), Colors::White, 0, XMFLOAT2(0, 0), XMFLOAT2(0.75f, 0.75f));

        DD2D_.font->DrawString(DD2D_.sprites.get(),
            StringUtils::char_to_wchar("Press P to Unpause"),
            XMFLOAT2(410, 35), Colors::White, 0, XMFLOAT2(0, 0), XMFLOAT2(0.5f, 0.5f));
    }
    else
    {
        DD2D_.font->DrawString(DD2D_.sprites.get(),
            StringUtils::char_to_wchar("Press P to Pause"),
            XMFLOAT2(390, 0), Colors::White, 0, XMFLOAT2(0, 0), XMFLOAT2(0.75f, 0.75f));
    }
}

void Game::simulating_tick()
{
    boid_manager_->tick(&GD_);
}

void Game::draw(ID3D11DeviceContext* _d3d_immediate_context)
{
	// Set immediate context of the graphics device.
	DD_.d3d_immediate_context = _d3d_immediate_context;

	// Update the constant buffer for the rendering of VBGOs.
	VBGO::update_constant_buffer(&DD_);

    // Draw Boids.
    boid_manager_->draw(&DD_);
    
	// Draw all other objects.
    for (auto& obj : game_objects_)
	{
		obj->draw(&DD_);
	}

	// Draw sprite batch stuff.
	DD2D_.sprites->Begin();
    for (auto& obj : game_objects_2d_)
	{
		obj->draw(&DD2D_);
	}

    draw_pause_text();

	DD2D_.sprites->End();

	// Reset Depth Stencil State after drawing sprites.
    _d3d_immediate_context->OMSetDepthStencilState(states_->DepthDefault(), 0);

    // Draw Ant Tweak Bar.
    TwDraw();
}

void Game::enumerate_boid_types() const
{
    auto file = FileUtils::read_json("BoidTypes");

    int type_count = 0;
    for (const auto& entry : file.object_value())
    {
        auto name = entry.name();
        const auto& data = entry.value();
        auto settings = std::make_unique<BoidSettings>();

        settings->type = name;
        settings->type_id = type_count++;
        settings->model = cmo_manager_->get_model(data["model"].as_string());
        settings->max_speed = data["max_speed"].as_double();
        settings->max_steer = data["max_steer"].as_double();
        settings->desired_separation = data["desired_separation"].as_double();
        settings->neighbour_scan = data["neighbour_scan"].as_double();
        settings->tag_distance = data["tag_distance"].as_double();

        if (data.has_member("tag_functions"))
        {
            auto function_names = data["tag_functions"].as_vector<std::string>();
            
            for (auto& function_name : function_names)
            {
                settings->tag_functions.push_back(boid_manager_->get_tag_function(function_name));
            }
        }

        json weighted_rules = json::array{data["parameterised_rules"].array_value()};
        for (auto& rules_entry : weighted_rules.array_value())
        {
            auto fetched_rule = boid_manager_->get_rule(rules_entry["rule"].as_string());
            auto rule_weight = rules_entry["weight"].as_double();
            auto valid_types = rules_entry["valid_types"].as_vector<std::string>();

            ParameterisedRule parameterised_rule(fetched_rule, rule_weight, valid_types);

            settings->parameterised_rules.push_back(parameterised_rule);
        }

        boid_manager_->add_boid_type(name, std::move(settings));
    }
}

void Game::init_tweak_bar(ID3D11Device* _d3d_device) const
{
    // Ant Tweak Bar initialisation.
    TwInit(TW_DIRECT3D11, _d3d_device); // for Direct3D 11
    TwWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    TwBar* boid_bar = TwNewBar("Boid Settings");

    // Read-only boid counter.
    TwAddVarRO(boid_bar, "Num Boids", TW_TYPE_INT32, boid_manager_->get_num_boids(), "");
    TwAddSeparator(boid_bar, "sep1", "");

    tweak_bar_spawn_selection(boid_bar);
    TwAddButton(boid_bar, "deltype", delete_all_of_spawn_type, boid_manager_.get(), " label='Delete Boids by Type' ");
    TwAddSeparator(boid_bar, "sep2", "");

    tweak_bar_boid_settings(boid_bar);
}

void Game::tweak_bar_spawn_selection(TwBar* _twbar) const
{
    auto& boid_types = boid_manager_->get_boid_types();
    TwEnumVal* tw_enum = new TwEnumVal[boid_types.size()];
    for (int i = 0; i < boid_types.size(); ++i)
    {
        auto it = std::find_if(boid_types.begin(), boid_types.end(), [i](const auto& _elem)
        {
            return _elem.second->type_id == i;
        });

        tw_enum[i].Label = it->second->type.c_str();
        tw_enum[i].Value = i;
    }

    TwType spawn_type = TwDefineEnum("spawntypeenum", tw_enum, boid_types.size());
    TwAddVarRW(_twbar, "spawntype", spawn_type, boid_manager_->get_editable_spawn_id(), " label='Spawn Type' ");
}

void Game::tweak_bar_boid_settings(TwBar* _twbar) const
{
    const std::map<std::string, std::unique_ptr<BoidSettings>>& boid_types = boid_manager_->get_boid_types();
    for (auto& elem : boid_types)
    {
        std::string var_prefix = elem.second->type;
        std::string group_value;
        group_value.append(" group='" + var_prefix + "settings' ");

        TwAddVarRW(_twbar, (var_prefix + "maxspeed").c_str(), TW_TYPE_FLOAT, &elem.second->max_speed,
            (" label='Max Speed' min=1.0 max=30.0 step=0.2 " + group_value).c_str());

        TwAddVarRW(_twbar, (var_prefix + "maxsteer").c_str(), TW_TYPE_FLOAT, &elem.second->max_steer,
            (" label='Max Steer' min=0.0 max=5.0 step=0.2 " + group_value).c_str());

        TwAddVarRW(_twbar, (var_prefix + "desiredsep").c_str(), TW_TYPE_FLOAT, &elem.second->desired_separation,
            (" label='Desired Separation' min=0.0 max=30.0 step=0.2 " + group_value).c_str());

        TwAddVarRW(_twbar, (var_prefix + "scan").c_str(), TW_TYPE_FLOAT, &elem.second->neighbour_scan,
            (" label='Neighbour Scan' min=1.0 max=30.0 step=0.2 " + group_value).c_str());

        TwAddVarRW(_twbar, (var_prefix + "tag").c_str(), TW_TYPE_FLOAT, &elem.second->tag_distance,
            (" label='Tag Range' min=1.0 max=30.0 step=0.2 " + group_value).c_str());

        TwAddSeparator(_twbar, (var_prefix + "sep").c_str(), "");
    }
}

void Game::tick_all_objects()
{
    // Update all game objects.
    for (auto& obj : game_objects_)
    {
        obj->tick(&GD_);
    }

    // Update all 2D game objects.
    for (auto& obj : game_objects_2d_)
    {
        obj->tick(&GD_);
    }
}

void Game::handle_pause()
{
    // Pause/Unpause on P key press.
    if (input_handler_->get_key_down(DIK_P))
    {
        if (GD_.game_state == GS_SIMULATING)
            GD_.game_state = GS_PAUSED;
        else
            GD_.game_state = GS_SIMULATING;
    }
}

void Game::handle_camera_change()
{
    // Switch camera on Space key press.
    if (input_handler_->get_key_down(DIK_SPACE))
    {
        if (GD_.active_camera == CAM_TT)
        {
            GD_.active_camera = CAM_TPS;
            DD_.camera = tps_camera_.get();
        }
        else
        {
            GD_.active_camera = CAM_TT;
            DD_.camera = tabletop_camera_.get();
        }
    }
}

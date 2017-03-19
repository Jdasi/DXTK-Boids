#include "Game.h"
#include "ObjectList.h"
#include "Constants.h"
#include "FileUtils.h"

#include "SimpleMath.h"
#include <AntTweakBar.h>

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
    states_ = new CommonStates(_d3d_device);

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

    // Configure initial GameData struct settings.
    GD_.game_state = GS_PLAY_MAIN_CAM;
    GD_.input_handler = input_handler_.get();

	//init render system for VBGOs
	VBGO::init(_d3d_device);

	//find how big my window is to correctly calculate my aspect ratio
	RECT rc;
	GetClientRect(hWnd_, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;
	float AR = (float)width / (float)height;

	// Base light.
	light_ = new Light(Vector3(0.0f, 100.0f, 160.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Color(0.5f, 0.5f, 0.5f, 1.0f));
	game_objects_.push_back(light_);

    // Tabletop Simulator style camera to orbit around the simulation.
    tabletop_camera_ = new TabletopCamera(0.25f * XM_PI, AR, 1.0f, 10000.0f, Vector3::UnitY, 0.3f, 70.0f);
    game_objects_.push_back(tabletop_camera_);

	// Player.
	Player* pPlayer = new Player(cmo_manager_->get_model("BirdModelV1"));
    pPlayer->set_pos({ 0, 10, 0 });
	game_objects_.push_back(pPlayer);

	// TPS Camera to follow the Player's movements.
	tps_camera_ = new TPSCamera(0.25f * XM_PI, AR, 1.0f, 10000.0f, pPlayer, Vector3::UnitY, Vector3(0.0f, 10.0f, 50.0f));
	game_objects_.push_back(tps_camera_);

	//create DrawData struct and populate its pointers
	DD_.d3d_immediate_context = nullptr;
	DD_.states = states_;
	DD_.camera = tabletop_camera_;
	DD_.light = light_;

	//add some stuff to show off
	FileVBGO* terrainBox = new FileVBGO("../Assets/terrainTex.txt", _d3d_device);
    terrainBox->set_scale(10.0f, 0.5f, 10.0f);
    terrainBox->set_pos({ 0, -0.5f, 0 });
	game_objects_.push_back(terrainBox);
};

Game::~Game() 
{
	//tidy up VBGO render system
	VBGO::clean_up();

	//get rid of the game objects here
    for (auto& obj : game_objects_)
    {
		delete obj;
	}
	game_objects_.clear();


	//and the 2D ones
    for (auto& obj : game_objects_2d_)
    {
		delete obj;
	}
	game_objects_2d_.clear();

	//clear away CMO render system
	delete states_;

    TwTerminate();
}

bool Game::tick() 
{
    // Crude delta-time system.
    GD_.delta_time = timer_.getTimeDifference();
    timer_.reset();

    // Tick core systems.
    input_handler_->tick();
    boid_manager_->tick(&GD_);

	// Tick audio engine.
	if (!audio_engine_->Update())
	{
		// No audio device is active.
		if (audio_engine_->IsCriticalError())
		{
			return false;
		}
	}

    // Quit on ESC.
	if (input_handler_->get_key_down(DIK_ESCAPE))
	{
		return false;
	}

	switch (GD_.game_state)
	{
	    case GS_ATTRACT:
		    break;
	    case GS_PAUSE:
		    break;
	    case GS_GAME_OVER:
		    break;
	    case GS_PLAY_MAIN_CAM:
	    case GS_PLAY_TPS_CAM:
		    play_tick();
		    break;

        default: {}
	}
	
	return true;
};

void Game::play_tick()
{
	//upon space bar switch camera state
	if (input_handler_->get_key_down(DIK_SPACE))
	{
		if (GD_.game_state == GS_PLAY_MAIN_CAM)
		{
			GD_.game_state = GS_PLAY_TPS_CAM;
		}
		else
		{
			GD_.game_state = GS_PLAY_MAIN_CAM;
		}
	}

	//update all objects
    for (auto& obj : game_objects_)
    {
		obj->tick(&GD_);
	}

    for (auto& obj : game_objects_2d_)
    {
		obj->tick(&GD_);
	}
}

void Game::draw(ID3D11DeviceContext* _d3d_immediate_context)
{
	//set immediate context of the graphics device
	DD_.d3d_immediate_context = _d3d_immediate_context;

	//set which camera to be used
	DD_.camera = tabletop_camera_;
	if (GD_.game_state == GS_PLAY_TPS_CAM)
	{
		DD_.camera = tps_camera_;
	}

	//update the constant buffer for the rendering of VBGOs
	VBGO::update_constant_buffer(&DD_);

    // Draw Boids.
    boid_manager_->draw(&DD_);
    
	//draw all objects
    for (auto& obj : game_objects_)
	{
		obj->draw(&DD_);
	}

	// Draw sprite batch stuff 
	DD2D_.sprites->Begin();
    for (auto& obj : game_objects_2d_)
	{
		obj->draw(&DD2D_);
	}
	DD2D_.sprites->End();

	//drawing text screws up the Depth Stencil State, this puts it back again!
    _d3d_immediate_context->OMSetDepthStencilState(states_->DepthDefault(), 0);

    TwDraw();
}

void Game::enumerate_boid_types() const
{
    auto file = FileUtils::read_json("BoidTypes");

    int type_count = 0;
    for (const auto& entry : file.members())
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
    TwBar* myBar = TwNewBar("Boid Settings");

    // Read-only boid counter.
    int* num_boids = boid_manager_->get_num_boids();
    TwAddVarRO(myBar, "Num Boids", TW_TYPE_INT32, num_boids, "");
    TwAddSeparator(myBar, "sep1", "");

    tweak_bar_human_settings(myBar);
    tweak_bar_zombie_settings(myBar);
}

void Game::tweak_bar_human_settings(TwBar* _twbar) const
{
    BoidSettings* human_settings = boid_manager_->get_boid_settings("human");

    TwAddVarRW(_twbar, "hmaxspeed", TW_TYPE_FLOAT, &human_settings->max_speed,
        " label='Max Speed' min=1.0 max=30.0 step=0.2 group='HumanSettings' ");

    TwAddVarRW(_twbar, "hmaxsteer", TW_TYPE_FLOAT, &human_settings->max_steer,
        " label='Max Steer' min=0.0 max=5.0 step=0.2 group='HumanSettings' ");

    TwAddVarRW(_twbar, "hdesiredsep", TW_TYPE_FLOAT, &human_settings->desired_separation,
        " label='Desired Separation' min=0.0 max=30.0 step=0.2 group='HumanSettings' ");

    TwAddVarRW(_twbar, "hscan", TW_TYPE_FLOAT, &human_settings->neighbour_scan,
        " label='Neighbour Scan' min=1.0 max=30.0 step=0.2 group='HumanSettings' ");
}

void Game::tweak_bar_zombie_settings(TwBar* _twbar) const
{
    BoidSettings* zombie_settings = boid_manager_->get_boid_settings("zombie");

    TwAddVarRW(_twbar, "zmaxspeed", TW_TYPE_FLOAT, &zombie_settings->max_speed,
        " label='Max Speed' min=1.0 max=30.0 step=0.2 group='ZombieSettings' ");

    TwAddVarRW(_twbar, "zmaxsteer", TW_TYPE_FLOAT, &zombie_settings->max_steer,
        " label='Max Steer' min=0.0 max=5.0 step=0.2 group='ZombieSettings' ");

    TwAddVarRW(_twbar, "zdesiredsep", TW_TYPE_FLOAT, &zombie_settings->desired_separation,
        " label='Desired Separation' min=0.0 max=30.0 step=0.2 group='ZombieSettings' ");

    TwAddVarRW(_twbar, "zscan", TW_TYPE_FLOAT, &zombie_settings->neighbour_scan,
        " label='Neighbour Scan' min=1.0 max=30.0 step=0.2 group='ZombieSettings' ");

    TwAddVarRW(_twbar, "zinfect", TW_TYPE_FLOAT, &zombie_settings->infection_distance,
        " label='Infection Range' min=1.0 max=30.0 step=0.2 group='ZombieSettings' ");
}

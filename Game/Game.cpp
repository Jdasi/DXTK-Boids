#include "Game.h"
#include "ObjectList.h"

#include "SimpleMath.h"
#include <AntTweakBar.h>

#include <windows.h>
#include <time.h>

Game::Game(ID3D11Device* _d3d_device, HWND _hWnd, HINSTANCE _hInstance)
    : play_time_()
    , hWnd_(_hWnd)
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

    // Ant Tweak Bar initialisation.
    TwInit(TW_DIRECT3D11, _d3d_device); // for Direct3D 11
    TwWindowSize(1024, 768);
    TwBar *myBar = TwNewBar("Test");
    TwAddVarRW(myBar, "Test Float", TW_TYPE_FLOAT, &test_float, "");

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
    boid_manager_ = std::make_unique<BoidManager>(*cmo_manager_, 10);

    //create GameData struct and populate its pointers
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

	// Base Camera that views the simulation from a fixed perspective.
	camera_ = new Camera(0.25f * XM_PI, AR, 1.0f, 10000.0f, Vector3::UnitY, Vector3::Zero);
	camera_->set_pos(Vector3(0.0f, 100.0f, 100.0f));
	game_objects_.push_back(camera_);

	// Base light.
	light_ = new Light(Vector3(0.0f, 100.0f, 160.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Color(0.4f, 0.1f, 0.1f, 1.0f));
	game_objects_.push_back(light_);

	// Player.
	Player* pPlayer = new Player(cmo_manager_->get_model("BirdModelV1"));
    pPlayer->set_pos(Vector3(0, 10, 0));
	game_objects_.push_back(pPlayer);

	// TPS Camera to follow the Player's movements.
	tps_camera_ = new TPSCamera(0.25f * XM_PI, AR, 1.0f, 10000.0f, pPlayer, Vector3::UnitY, Vector3(0.0f, 10.0f, 50.0f));
	game_objects_.push_back(tps_camera_);

    // Free Camera to orbit around the simulation.
    free_camera_ = new FreeCamera(0.25f * XM_PI, AR, 1.0f, 10000.0f, Vector3::Zero, Vector3::UnitY, Vector3(0.0f, 0.0f, 50.0f));
    game_objects_.push_back(free_camera_);

	//create DrawData struct and populate its pointers
	DD_.d3d_immediate_context = nullptr;
	DD_.states = states_;
	DD_.camera = camera_;
	DD_.light = light_;

	//add some stuff to show off
	FileVBGO* terrainBox = new FileVBGO("../Assets/terrainTex.txt", _d3d_device);
    terrainBox->set_scale(10.0f, 0.5f, 10.0f);
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
    // tick core systems.
    input_handler_->tick();
    boid_manager_->tick(&GD_);

	//tick audio engine
	if (!audio_engine_->Update())
	{
		// No audio device is active
		if (audio_engine_->IsCriticalError())
		{
			//something has gone wrong with audio so QUIT!
			return false;
		}
	}

	//Upon pressing escape QUIT
	if (input_handler_->get_key_down(DIK_ESCAPE))
	{
		return false;
	}

	//calculate frame time-step dt for passing down to game objects
	DWORD currentTime = GetTickCount();
	GD_.delta_time = min((float)(currentTime - play_time_) / 1000.0f, 0.1f);
	play_time_ = currentTime;

	//start to a VERY simple FSM
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
	DD_.camera = free_camera_;
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
};

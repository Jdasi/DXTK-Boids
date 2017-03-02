#pragma once
#include <dinput.h>
#include "Effects.h"
#include "CommonStates.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "Audio.h"

#include <windows.h>
#include <list>

//=================================================================
//Basic Game Manager Class
//=================================================================

using namespace DirectX;

class GameObject;
class GameObject2D;
class Camera;
class TPSCamera;
struct GameData;
struct DrawData;
struct DrawData2D;
class Light;
class InputHandler;

class Game
{
public:
	Game(ID3D11Device* _d3d_device, HWND _hWnd, HINSTANCE _hInstance);
	virtual ~Game();

	bool tick(); //tick the game state

	void draw(ID3D11DeviceContext* _d3d_immediate_context); //render the current game state

protected:
	DWORD play_time_; //amount of time since the game started
    HWND hWnd_;

    float test_float = 0;

    std::unique_ptr<InputHandler> input_handler_;

	Camera* camera_;            //principle camera
	TPSCamera* tps_camera_;     //TPS cam
	Light* light_;              //base light

    std::list<GameObject *> game_objects_;      //data structure storing all GameObjects of this Game
    std::list<GameObject2D *> game_objects_2d_; //ditto 2D objects

	//required for the CMO model rendering system
	CommonStates* states_;
	IEffectFactory* fx_factory_;

	GameData* GD_;		//Data to be shared to all Game Objects as they are ticked
	DrawData* DD_;		//Data to be shared to all Game Objects as they are drawn
	DrawData2D* DD2D_;	//Data to be passed by game to all 2D Game Objects via Draw 

	//sound stuff
	std::unique_ptr<AudioEngine> audio_engine_;

	//tick functions for each state
	void play_tick();
};

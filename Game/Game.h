#pragma once
#include "Effects.h"
#include "CommonStates.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "Audio.h"
#include "GameData.h"
#include "DrawData.h"
#include "DrawData2D.h"
#include "SimpleTimer.h"

#include <windows.h>
#include <list>

//=================================================================
//Basic Game Manager Class
//=================================================================

using namespace DirectX;

class GameObject;
class GameObject2D;
class TabletopCamera;
class TPSCamera;
class Player;
class Light;
class InputHandler;
class CMOManager;
class BoidManager;
typedef struct CTwBar TwBar;

class Game
{
public:
	Game(ID3D11Device* _d3d_device, HWND _hWnd, HINSTANCE _hInstance);
	virtual ~Game();

	bool tick();
	void draw(ID3D11DeviceContext* _d3d_immediate_context);

    void recompile_json();

protected:
    void enumerate_boid_types() const;
    void init_tweak_bar(ID3D11Device* _d3d_device) const;
    void tweak_bar_spawn_selection(TwBar* _twbar) const;
    void tweak_bar_boid_settings(TwBar* _twbar) const;

    void handle_pause();
    void handle_camera_change();
    void draw_pause_text() const;

    HWND hWnd_;
    SimpleTimer timer_;

    std::unique_ptr<InputHandler> input_handler_;
    std::unique_ptr<CMOManager> cmo_manager_;
    std::unique_ptr<BoidManager> boid_manager_;

    std::unique_ptr<TabletopCamera> tabletop_camera_;   // Tabletop cam
	std::unique_ptr<TPSCamera> tps_camera_;             // TPS cam
    std::unique_ptr<Player> player_;                    // Controllable player object
	Light* light_;                                      // Base light

    std::list<std::unique_ptr<GameObject>> sim_objects_;      // All GOs used in the sim
    std::list<std::unique_ptr<GameObject2D>> sim_objects_2d_; // All GO2Ds used in the sim

	//required for the CMO model rendering system
	std::unique_ptr<CommonStates> states_;
	std::unique_ptr<IEffectFactory> fx_factory_;

	GameData GD_;		//Data to be shared to all Game Objects as they are ticked
	DrawData DD_;		//Data to be shared to all Game Objects as they are drawn
	DrawData2D DD2D_;	//Data to be passed by game to all 2D Game Objects via Draw 

	// Sound engine.
	std::unique_ptr<AudioEngine> audio_engine_;

    // GameState tick functions.
	void simulating_tick();

};

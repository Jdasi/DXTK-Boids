#pragma once
#include "GameState.h"

#include <d3d11.h>
#include "SimpleMath.h"

//=================================================================
//Data to be passed by game to all Game Objects via tick
//=================================================================

class InputHandler;

struct GameData
{
	float delta_time;  //time step since last frame
	GameState game_state; //global GameState
    InputHandler* input_handler;
    DirectX::SimpleMath::Vector3 boid_spawn_pos;
};

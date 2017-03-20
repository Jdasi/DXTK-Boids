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
	float delta_time; // Holds the time since the last cycle.
	GameState game_state; // The state of the simulation.
    ActiveCamera active_camera; // Which camera is currently being used.
    InputHandler* input_handler; // Object for handling input.
    DirectX::SimpleMath::Vector3 boid_spawn_pos; // Where boids will spawn.
};

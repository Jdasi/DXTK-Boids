#pragma once

//=================================================================
//Data to be passed by game to all Game Objects via Draw 
//=================================================================

#include "CommonStates.h"

using namespace DirectX;

class Camera;
class Light;

struct DrawData
{
	ID3D11DeviceContext* d3d_immediate_context;
	CommonStates* states;
	Camera* camera;
	Light* light;
};

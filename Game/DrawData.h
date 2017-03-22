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
    ID3D11Device* d3d_device = nullptr;
	ID3D11DeviceContext* d3d_immediate_context = nullptr;
	CommonStates* states = nullptr;
	Camera* camera = nullptr;
	Light* light = nullptr;
};

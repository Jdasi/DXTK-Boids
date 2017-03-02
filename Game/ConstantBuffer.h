#pragma once

//=================================================================
//constant buffer for the base shader used for the VBGO renderer
//=================================================================

#include <windows.h>
#include <d3d11.h>
#include <SimpleMath.h>

using namespace DirectX;
using namespace SimpleMath;

//as passing to GPU needs to be correctly memory aligned
__declspec(align(16))
struct ConstantBuffer
{
	Matrix	world;
	Matrix	view;
	Matrix	projection;
	Matrix	rot;
	Color	light_colour;
	Color	ambient_colour;
	Vector3 pos;
};

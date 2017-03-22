#pragma once
#include "SpriteBatch.h"
#include "SpriteFont.h"

//=================================================================
//Data to be passed by game to all 2D Game Objects via Draw 
//=================================================================

using namespace DirectX;

struct DrawData2D
{
    std::unique_ptr<SpriteBatch> sprites;
	std::unique_ptr<SpriteFont> font;
};

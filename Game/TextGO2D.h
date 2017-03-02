#pragma once
#include "GameObject2D.h"

//=================================================================
//a Game Object 2D for displaying text
//=================================================================

class TextGO2D : public GameObject2D
{
public:
	TextGO2D(const std::string& _text);
    ~TextGO2D() = default;

	virtual void tick(GameData* _GD);
	virtual void draw(DrawData2D* _DD);

protected:
    std::string text_;

};

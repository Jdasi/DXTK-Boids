#pragma once
#include "CMOGO.h"

//=================================================================
//Base Player Class (i.e. a GO the player controls)
//=================================================================

class CMOModel;

class Player : public CMOGO
{
public:
    explicit Player(CMOModel* _model);
	~Player() = default;

	virtual void tick(GameData* _GD) override;

protected:


};

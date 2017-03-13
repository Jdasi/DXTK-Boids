#pragma once
#include "CMOGO.h"

//=================================================================
//Base Player Class (i.e. a GO the player controls)
//=================================================================

class CMOModel;

class Player final: public CMOGO
{
public:
    explicit Player(CMOModel* _model);
	~Player() = default;

	virtual void tick(GameData* _GD) override;

private:
    void horizontal_movement(GameData*_GD);
    void vertical_movement(GameData* _GD);
    void rotate(GameData* _GD);
    void limit_speed();

    float move_speed_ = 40.0f;
    float max_speed_ = 500.0f;
    float rotate_speed_ = 2.0f;

};

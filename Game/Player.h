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

	void tick(GameData* _GD) override;

private:
    void horizontal_movement(GameData*_GD);
    void rotate(GameData* _GD);
    void limit_speed();

};

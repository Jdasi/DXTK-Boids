#pragma once
#include "CMOGO.h"

//=================================================================
//Base Player Class (i.e. a GO the player controls)
//=================================================================

class Player : public CMOGO
{
public:
	Player(const std::string& _file_name, ID3D11Device* _d3d_device, IEffectFactory* _EF);
	~Player() = default;

	virtual void tick(GameData* _GD) override;

protected:


};

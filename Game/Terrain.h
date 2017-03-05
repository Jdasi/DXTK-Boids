#pragma once
#include "CMOGO.h"

//=================================================================
//Terrian Game Object Class (i.e. an Object that just sits there)
//=================================================================

struct GameData;
class CMOModel;

class Terrain : public CMOGO
{
public:
	Terrain(CMOModel* _model, Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale);
	~Terrain() = default;

	void tick(GameData* _GD) override {} ;

protected:


};

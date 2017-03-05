#pragma once
#include "GameObject.h"

//=================================================================
//A Class for loading and displaying CMO as GameObjects
//=================================================================

using namespace DirectX;

struct GameData;
class CMOModel;

class CMOGO : public GameObject
{
public:
    explicit CMOGO(CMOModel* _model);
	virtual ~CMOGO() = default;

	virtual void tick(GameData* _GD) override;
	virtual void draw(DrawData* _DD) override;

protected:
    CMOModel* model_;

};

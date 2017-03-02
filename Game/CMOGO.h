#pragma once
#include "GameObject.h"

#include "Model.h"

//=================================================================
//A Class for loading and displaying CMO as GameObjects
//=================================================================

using namespace DirectX;

struct GameData;

class CMOGO : public GameObject
{
public:
	CMOGO(const std::string& _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF);
	virtual ~CMOGO();

	virtual void tick(GameData* _GD) override;
	virtual void draw(DrawData* _DD) override;


protected:
    std::unique_ptr<Model>  model_;

	//needs a slightly different raster state that the VBGOs so create one and let them all use it
	static ID3D11RasterizerState*  raster_state_;
	static int count_;

};

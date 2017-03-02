#pragma once
#include "CMOGO.h"

#include <list>

//=================================================================
//A component of the L-System (ish) tree
//=================================================================

class TreeBit :public CMOGO
{
public:
	TreeBit(int _levels, int _perLevel, int _ofLevel, float _scale, Vector3 _shift, float _lean, const std::string& _bitFileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, TreeBit* _parent, std::list<TreeBit*>* _tree);
	~TreeBit() = default;

	virtual void tick(GameData* _GD) override;

protected:
	TreeBit* parent_;

};

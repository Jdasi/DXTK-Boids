#pragma once
#include "GameObject.h"

#include "Model.h"

#include <list>
#include <string>

//=================================================================
//An L-System (ish) tree
//=================================================================

using namespace DirectX;

class TreeBit;

class Tree :public GameObject
{
public:
	Tree(int _levels, int _per_level, float _scale, Vector3 _shift, float _lean, const std::string& _bit_file_name, ID3D11Device* _d3d_device, IEffectFactory* _EF);
	~Tree();

	virtual void tick(GameData* _GD) override;
	virtual void draw(DrawData* _DD) override; 

protected:
    std::list<TreeBit*> m_Tree;
};

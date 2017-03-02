#include "TreeBit.h"
#include "GameData.h"

TreeBit::TreeBit(int _levels, int _per_level, int _of_level, float _scale, Vector3 _shift, float _lean, const std::string& _bit_file_name, ID3D11Device* _d3d_device, IEffectFactory* _EF, TreeBit* _parent, std::list<TreeBit*>* _tree)
    : CMOGO(_bit_file_name, _d3d_device, _EF)
    , parent_(_parent)
{
	if (_of_level >= 0 )
	{
		yaw_ = XM_2PI *  (float)_of_level / (float)_per_level; //equally spread the branches around the trunk/parent
		pitch_ = _lean; //leaning the branches out by this angle
		pos_ = _shift;  //move this far up the trunk/parent
		scale_ = _scale*Vector3::One; //scale me down by this much relative to my trunk/parent
	}
	
	_tree->push_back(this);
	
	if (_levels >1)
	{
		for (int i = 0; i < _per_level; i++)
		{
			//recursively create my children
			new TreeBit(_levels - 1, _per_level, i, _scale, _shift, _lean, _bit_file_name, _d3d_device, _EF, this, _tree);
		}
	}
}

void TreeBit::tick(GameData* _GD)
{
	CMOGO::tick(_GD);
	if (parent_)
	{
		//as should all be ticked in generational order
		//this should pick up all the previous generations of the parenting
		world_ = world_ * parent_->world_;
	}
}

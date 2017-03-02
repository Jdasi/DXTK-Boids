#include "Tree.h"
#include "TreeBit.h"

Tree::Tree(int _levels, int _per_level, float _scale, Vector3 _shift, float _lean, const std::string& _bit_file_name, ID3D11Device* _pd3dDevice, IEffectFactory* _EF)
{
	// Recursively create the tree.
	new TreeBit(_levels, _per_level, -1, _scale, _shift, _lean, _bit_file_name, _pd3dDevice, _EF, nullptr, &m_Tree);
}

Tree::~Tree()
{
	for (std::list<TreeBit *>::iterator it = m_Tree.begin(); it != m_Tree.end(); ++it)
	{
		delete (*it);
	}

	m_Tree.clear();
}


void Tree::tick(GameData* _GD)
{
	//update all tree parts
	for (std::list<TreeBit *>::iterator it = m_Tree.begin(); it != m_Tree.end(); it++)
	{
		(*it)->tick(_GD);
	}
}

void Tree::draw(DrawData* _DD)
{
	//draw all tree parts
	for (std::list<TreeBit *>::iterator it = m_Tree.begin(); it != m_Tree.end(); it++)
	{
		(*it)->draw(_DD);
	}
}
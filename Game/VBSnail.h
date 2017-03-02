#pragma once
#include "VBGO.h"

#include <string>

//=================================================================
//create a snail shell from a base line shape read in from a txt file
//=================================================================

struct GameData;

class VBSnail : public VBGO
{
public:
	VBSnail(ID3D11Device* _GD, const std::string& _filename, int _sections, float _scale, float _rot, float _step, Color _col1, Color _col2 );
	virtual ~VBSnail() = default;

	virtual void tick(GameData* _GD) override;

protected:


};

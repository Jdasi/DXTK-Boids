#pragma once
#include "VBCube.h"

//=================================================================
//create a puffed out cube VBGO
//=================================================================

class VBPillow : public VBCube
{
public:
	VBPillow(){};

protected:
	virtual void Transform() override
	{
		for (unsigned int i = 0; i<num_prims_ * 3; i++)
		{
			vertices_[i].color *= ((i / 3) % 2) ? 1.0f : 0.5f;
			vertices_[i].color.w = 1.0f;

			Vector3 vertPos = vertices_[i].pos;
			Vector3 spherePos = vertices_[i].pos;

			spherePos.Normalize();

			Vector3 newPos = 0.5f * (vertPos + (float)(size_ - 1) * spherePos);

			vertices_[i].pos = newPos;
		}
	}

};

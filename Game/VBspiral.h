#pragma once
#include "VBCube.h"

//=================================================================
//create a twisted cube VBGO
//=================================================================

class VBSpiral : public VBCube
{
public:
	VBSpiral(){};

protected:
	virtual void Transform() override
	{
		for (int i = 0; i<num_prims_ * 3; i++)
		{
			Vector3 vertPos = vertices_[i].pos;

			Matrix rotMat = Matrix::CreateFromYawPitchRoll(XM_PI*(vertPos.y + 5.0f) / 20.0f, 0.0f, 0.0f);
			
			Vector3 newPos = Vector3::Transform(vertPos, rotMat);

			vertices_[i].pos = newPos;
		}
	}
};

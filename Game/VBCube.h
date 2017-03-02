#pragma once
#include "VBGO.h"
#include "Vertex.h"

//=================================================================
//procedurally generate a VBGO Cube
//each side be divided in to _size * _size squares (2 triangles per square)
//=================================================================

class VBCube : public VBGO
{
public:
	VBCube() = default;
	virtual ~VBCube() = default;

	//initialise the Veretx and Index buffers for the cube
	void init(int _size, ID3D11Device* _GD);

protected:
	//this is to allow custom versions of this which create the basic cube and then distort it
	//see VBSpiral, VBSpiked and VBPillow
	virtual void Transform() = 0;

	int size_;
	myVertex* vertices_;

};

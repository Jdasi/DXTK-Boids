#include "VBCube.h"

void VBCube::init(int _size, ID3D11Device* GD)
{
	// this is most certinaly not the most efficent way of doing most of this
	//but it does give you a very clear idea of what is actually going on

	size_ = _size;

	//calculate number of vertices and primatives
	int numVerts = 6 * 6 * (size_ - 1) * (size_ - 1);
	num_prims_ = numVerts / 3;
	vertices_ = new myVertex[numVerts];
	WORD* indices = new WORD[numVerts];

	//as using the standard VB shader set the tex-coords somewhere safe
	for (int i = 0; i<numVerts; i++)
	{
		indices[i] = i;
		vertices_[i].tex_coord = Vector2::One; 
	}

	//in each loop create the two traingles for the matching sub-square on each of the six faces
	int vert = 0;
	for (int i = -(size_ - 1) / 2; i<(size_ - 1) / 2; i++)
	{
		for (int j = -(size_ - 1) / 2; j<(size_ - 1) / 2; j++)
		{
			//top
			vertices_[vert].color = Color(1.0f, 0.0f, 0.0f, 1.0f);
			vertices_[vert++].pos = Vector3((float)i, 0.5f * (float)(size_ - 1), (float)j);
			vertices_[vert].color = Color(1.0f, 0.0f, 0.0f, 1.0f);
			vertices_[vert++].pos = Vector3((float)i, 0.5f * (float)(size_ - 1), (float)(j + 1));
			vertices_[vert].color = Color(1.0f, 0.0f, 0.0f, 1.0f);
			vertices_[vert++].pos = Vector3((float)(i + 1), 0.5f * (float)(size_ - 1), (float)j);

			vertices_[vert].color = Color(1.0f, 0.0f, 0.0f, 1.0f);
			vertices_[vert++].pos = Vector3((float)(i + 1), 0.5f * (float)(size_ - 1), (float)j);
			vertices_[vert].color = Color(1.0f, 0.0f, 0.0f, 1.0f);
			vertices_[vert++].pos = Vector3((float)i, 0.5f * (float)(size_ - 1), (float)(j + 1));
			vertices_[vert].color = Color(1.0f, 0.0f, 0.0f, 1.0f);
			vertices_[vert++].pos = Vector3((float)(i + 1), 0.5f * (float)(size_ - 1), (float)(j + 1));

			//back
			vertices_[vert].color = Color(0.0f, 1.0f, 0.0f, 1.0f);
			vertices_[vert++].pos = Vector3((float)i, (float)j, 0.5f * (float)(size_ - 1));
			vertices_[vert].color = Color(0.0f, 1.0f, 0.0f, 1.0f);
			vertices_[vert++].pos = Vector3((float)(i + 1), (float)j, 0.5f * (float)(size_ - 1));
			vertices_[vert].color = Color(0.0f, 1.0f, 0.0f, 1.0f);
			vertices_[vert++].pos = Vector3((float)i, (float)(j + 1), 0.5f * (float)(size_ - 1));

			vertices_[vert].color = Color(0.0f, 1.0f, 0.0f, 1.0f);
			vertices_[vert++].pos = Vector3((float)(i + 1), (float)j, 0.5f * (float)(size_ - 1));
			vertices_[vert].color = Color(0.0f, 1.0f, 0.0f, 1.0f);
			vertices_[vert++].pos = Vector3((float)(i + 1), (float)(j + 1), 0.5f * (float)(size_ - 1));
			vertices_[vert].color = Color(0.0f, 1.0f, 0.0f, 1.0f);
			vertices_[vert++].pos = Vector3((float)i, (float)(j + 1), 0.5f * (float)(size_ - 1));

			//right
			vertices_[vert].color = Color(0.0f, 0.0f, 1.0f, 1.0f);
			vertices_[vert++].pos = Vector3(0.5f * (float)(size_ - 1), (float)i, (float)j);
			vertices_[vert].color = Color(0.0f, 0.0f, 1.0f, 1.0f);
			vertices_[vert++].pos = Vector3(0.5f * (float)(size_ - 1), (float)(i + 1), (float)j);
			vertices_[vert].color = Color(0.0f, 0.0f, 1.0f, 1.0f);
			vertices_[vert++].pos = Vector3(0.5f * (float)(size_ - 1), (float)i, (float)(j + 1));

			vertices_[vert].color = Color(0.0f, 0.0f, 1.0f, 1.0f);
			vertices_[vert++].pos = Vector3(0.5f * (float)(size_ - 1), (float)(i + 1), (float)j);
			vertices_[vert].color = Color(0.0f, 0.0f, 1.0f, 1.0f);
			vertices_[vert++].pos = Vector3(0.5f * (float)(size_ - 1), (float)(i + 1), (float)(j + 1));
			vertices_[vert].color = Color(0.0f, 0.0f, 1.0f, 1.0f);
			vertices_[vert++].pos = Vector3(0.5f * (float)(size_ - 1), (float)i, (float)(j + 1));

			//Bottom
			vertices_[vert].color = Color(1.0f, 1.0f, 0.0f, 1.0f);
			vertices_[vert++].pos = Vector3((float)j, -0.5f * (float)(size_ - 1), (float)i);
			vertices_[vert].color = Color(1.0f, 1.0f, 0.0f, 1.0f);
			vertices_[vert++].pos = Vector3((float)(j + 1), -0.5f * (float)(size_ - 1), (float)i);
			vertices_[vert].color = Color(1.0f, 1.0f, 0.0f, 1.0f);
			vertices_[vert++].pos = Vector3((float)j, -0.5f * (float)(size_ - 1), (float)(i + 1));

			vertices_[vert].color = Color(1.0f, 1.0f, 0.0f, 1.0f);
			vertices_[vert++].pos = Vector3((float)j, -0.5f * (float)(size_ - 1), (float)(i + 1));
			vertices_[vert].color = Color(1.0f, 1.0f, 0.0f, 1.0f);
			vertices_[vert++].pos = Vector3((float)(j + 1), -0.5f * (float)(size_ - 1), (float)i);
			vertices_[vert].color = Color(1.0f, 1.0f, 0.0f, 1.0f);
			vertices_[vert++].pos = Vector3((float)(j + 1), -0.5f * (float)(size_ - 1), (float)(i + 1));

			//front
			vertices_[vert].color = Color(0.0f, 1.0f, 1.0f, 1.0f);
			vertices_[vert++].pos = Vector3((float)j, (float)i, -0.5f * (float)(size_ - 1));
			vertices_[vert].color = Color(0.0f, 1.0f, 1.0f, 1.0f);
			vertices_[vert++].pos = Vector3((float)j, (float)(i + 1), -0.5f * (float)(size_ - 1));
			vertices_[vert].color = Color(0.0f, 1.0f, 1.0f, 1.0f);
			vertices_[vert++].pos = Vector3((float)(j + 1), (float)i, -0.5f * (float)(size_ - 1));

			vertices_[vert].color = Color(0.0f, 1.0f, 1.0f, 1.0f);
			vertices_[vert++].pos = Vector3((float)j, (float)(i + 1), -0.5f * (float)(size_ - 1));
			vertices_[vert].color = Color(0.0f, 1.0f, 1.0f, 1.0f);
			vertices_[vert++].pos = Vector3((float)(j + 1), (float)(i + 1), -0.5f * (float)(size_ - 1));
			vertices_[vert].color = Color(0.0f, 1.0f, 1.0f, 1.0f);
			vertices_[vert++].pos = Vector3((float)(j + 1), (float)i, -0.5f * (float)(size_ - 1));

			//left
			vertices_[vert].color = Color(1.0f, 0.0f, 1.0f, 1.0f);
			vertices_[vert++].pos = Vector3(-0.5f * (float)(size_ - 1), (float)j, (float)i);
			vertices_[vert].color = Color(1.0f, 0.0f, 1.0f, 1.0f);
			vertices_[vert++].pos = Vector3(-0.5f * (float)(size_ - 1), (float)j, (float)(i + 1));
			vertices_[vert].color = Color(1.0f, 0.0f, 1.0f, 1.0f);
			vertices_[vert++].pos = Vector3(-0.5f * (float)(size_ - 1), (float)(j + 1), (float)i);

			vertices_[vert].color = Color(1.0f, 0.0f, 1.0f, 1.0f);
			vertices_[vert++].pos = Vector3(-0.5f * (float)(size_ - 1), (float)j, (float)(i + 1));
			vertices_[vert].color = Color(1.0f, 0.0f, 1.0f, 1.0f);
			vertices_[vert++].pos = Vector3(-0.5f * (float)(size_ - 1), (float)(j + 1), (float)(i + 1));
			vertices_[vert].color = Color(1.0f, 0.0f, 1.0f, 1.0f);
			vertices_[vert++].pos = Vector3(-0.5f * (float)(size_ - 1), (float)(j + 1), (float)i);

		}
	}

	//carry out some kind of transform on these vertices to make this object more interesting
	Transform();

	//calculate the normals for the basic lighting in the base shader
	for (int i = 0; i < num_prims_; i++)
	{
		WORD V1 = 3 * i;
		WORD V2 = 3 * i + 1;
		WORD V3 = 3 * i + 2;

		//build normals
		Vector3 norm;
		Vector3 vec1 = vertices_[V1].pos - vertices_[V2].pos;
		Vector3 vec2 = vertices_[V3].pos - vertices_[V2].pos;
		norm = vec1.Cross(vec2);
		norm.Normalize();

		vertices_[V1].norm = norm;
		vertices_[V2].norm = norm;
		vertices_[V3].norm = norm;
	}


	BuildIB(GD, indices);
	BuildVB(GD, numVerts, vertices_);

	delete[] indices;    //this is no longer needed as this is now in the index Buffer
	delete[] vertices_; //this is no longer needed as this is now in the Vertex Buffer
	vertices_ = nullptr;
}

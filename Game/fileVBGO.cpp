#include "fileVBGO.h"
#include "StringUtils.h"
#include "Vertex.h"

#include "DDSTextureLoader.h"
#include <DirectXColors.h>

#include <iostream>
#include <fstream>

FileVBGO::FileVBGO(const std::string& _file_name, ID3D11Device* _GD)
{
	//open file
	std::ifstream meshFile;
	meshFile.open(_file_name);

	//what data does this file give
	tex_coords_ = false;
	colour_ = false;

	char texOrColour;
	meshFile >> texOrColour;

	if (texOrColour == 'T')
	{
		tex_coords_ = true;
		//get texture fileName
		char  texFileName[256];
		meshFile >> texFileName;

		std::string fullfilename =
#if DEBUG
			"../Debug/";
#else
			"../Release/";
#endif

		fullfilename += texFileName;
		fullfilename += ".dds";

		//load texture 
		HRESULT hr = CreateDDSTextureFromFile(_GD, StringUtils::charToWChar(fullfilename.c_str()), nullptr, &texture_rv_);
	}
	else
	{
		colour_ = true;
	}

	//get number of vertices
	int numVerts = 0;
	meshFile >> numVerts;
	myVertex* vertices = new myVertex[numVerts];

	//load verts
	int dummy;
	for (int i = 0; i< numVerts; i++)
	{
		meshFile >> dummy >> vertices[i].pos.x >> vertices[i].pos.y >> vertices[i].pos.z;

		if (tex_coords_)
		{
			meshFile >> vertices[i].tex_coord.x >> vertices[i].tex_coord.y;
			vertices[i].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
		}

		if (colour_)
		{
			float R, G, B, A;
			meshFile >> R >> G >> B >> A;
			vertices[i].color = Color(R, G, B, A);
			vertices[i].tex_coord = Vector2::One;
		}
	}

	//load indices
	meshFile >> num_prims_;
	WORD* indices = new WORD[3 * num_prims_];
	for (unsigned int i = 0; i<num_prims_; i++)
	{
		WORD V1, V2, V3;
		meshFile >> dummy >> V1 >> V2 >> V3;

		indices[3 * i] = V1;
		indices[3 * i + 1] = V2;
		indices[3 * i + 2] = V3;

		//build normals
		Vector3 norm;
		Vector3 vec1 = vertices[V1].pos - vertices[V2].pos;
		Vector3 vec2 = vertices[V3].pos - vertices[V2].pos;
		norm = vec1.Cross(vec2);
		norm.Normalize();

		vertices[V1].norm = norm;
		vertices[V2].norm = norm;
		vertices[V3].norm = norm;
	}

	BuildIB(_GD, indices);
	BuildVB(_GD, numVerts, vertices);

	//tidy up
	meshFile.close();
	delete[] vertices;
}

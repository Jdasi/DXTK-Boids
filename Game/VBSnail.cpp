#include "VBSnail.h"
#include "Vertex.h"
#include "GameData.h"

#include <iostream>
#include <fstream>

VBSnail::VBSnail(ID3D11Device* _GD, const std::string& _filename, int _sections, float _scale, float _rot, float _step, Color _col1, Color _col2)
{
	fudge_ = Matrix::CreateTranslation(-3.0f,-3.0f, 0.0f);
	int width = 0;

	//load line
    std::ifstream lineFile;
	lineFile.open(_filename);
	lineFile >> width;

	Vector2* baseLine = new Vector2[width];

	for (int i = 0; i < width; i++)
	{
		lineFile >> baseLine[i].x >> baseLine[i].y;
	}
	
	num_prims_ = 2 * (width - 1) * (_sections - 1);
	int numVerts = 3 * num_prims_;
	myVertex* vertices = new myVertex[numVerts];
	WORD* indices = new WORD[numVerts];

	//as using the standard VB shader set the tex-coords somewhere safe
	for (int i = 0; i<numVerts; i++)
	{
		indices[i] = i;
		vertices[i].tex_coord = Vector2::One;
	}	
	
	//build base transform
	Matrix rotTrans = Matrix::CreateRotationY(_rot);
	Matrix stepTrans = Matrix::CreateTranslation(0.0f, _step, 0.0f);
	Matrix scaleTrans = Matrix::CreateScale(_scale);
	Matrix baseTrans = scaleTrans *rotTrans * stepTrans;

	//build array of transforms to rotate,scale and translate baseline around the shell
	Matrix* transforms = new Matrix[_sections];
	transforms[0] = Matrix::Identity;
	for (int i = 1; i < _sections; i++)
	{
		transforms[i] = transforms[i - 1] * baseTrans;
	}

	//build the vertices using these transforms and the line
	//two trinagles for each point scale, turn ad step up
	int vert = 0;
	Vector3 calcPos;
	//transform up the steps
	for (int i = 0; i < (_sections - 1) ; i++)
	{
		for (int j = 0 ; j < (width -1); j++)
		{
			vertices[vert].color = Color::Lerp(_col1, _col2, (float)i / (float)_sections);
			//vertices[vert++].pos = Vector3((float)i, (float)j, 0.0f);
			calcPos = Vector3(baseLine[j].x, baseLine[j].y, 0.0f);
			vertices[vert++].pos = Vector3::Transform(calcPos, transforms[i]);

			vertices[vert].color = Color::Lerp(_col1, _col2, (float)(i + 1) / (float)_sections);
			calcPos = Vector3(baseLine[j].x, baseLine[j].y, 0.0f);
			vertices[vert++].pos = Vector3::Transform(calcPos, transforms[i+1]);
			//vertices[vert++].pos = Vector3((float)(i + 1), (float)j, 0.0f);

			vertices[vert].color = Color::Lerp(_col1, _col2, (float)i / (float)_sections);
			//vertices[vert++].pos = Vector3((float)i, (float)(j + 1), 0.0f);
			calcPos = Vector3(baseLine[j+1].x, baseLine[j+1].y, 0.0f);
			vertices[vert++].pos = Vector3::Transform(calcPos, transforms[i]);
			
			vertices[vert].color = Color::Lerp(_col1, _col2, (float)(i + 1) / (float)_sections);
			//vertices[vert++].pos = Vector3((float)(i + 1), (float)j, 0.0f);
			calcPos = Vector3(baseLine[j].x, baseLine[j].y, 0.0f);
			vertices[vert++].pos = Vector3::Transform(calcPos, transforms[i+1]);

			vertices[vert].color = Color::Lerp(_col1, _col2, (float)(i + 1) / (float)_sections);
			//vertices[vert++].pos = Vector3((float)(i + 1), (float)(j + 1), 0.0f);
			calcPos = Vector3(baseLine[j+1].x, baseLine[j+1].y, 0.0f);
			vertices[vert++].pos = Vector3::Transform(calcPos, transforms[i + 1]);

			vertices[vert].color = Color::Lerp(_col1, _col2, (float)i / (float)_sections);
			//vertices[vert++].pos = Vector3((float)i, (float)(j + 1), 0.0f);
			calcPos = Vector3(baseLine[j+1].x, baseLine[j+1].y, 0.0f);
			vertices[vert++].pos = Vector3::Transform(calcPos, transforms[i]);

		}
	}

	//calculate the normals for the basic lighting in the base shader
	for (int i = 0; i < num_prims_; i++)
	{
		WORD V1 = 3 * i;
		WORD V2 = 3 * i + 1;
		WORD V3 = 3 * i + 2;

		//build normals
		Vector3 norm;
		Vector3 vec1 = vertices[V1].pos - vertices[V2].pos;
		Vector3 vec2 = vertices[V3].pos - vertices[V2].pos;
		norm = vec2.Cross(vec1);
		norm.Normalize();

		vertices[V1].norm = norm;
		vertices[V2].norm = norm;
		vertices[V3].norm = norm;
	}


	BuildIB(_GD, indices);
	BuildVB(_GD, numVerts, vertices);

	delete[] vertices; //these are all no longer needed as this is now in the Vertex Buffer
	delete[] transforms;
	delete[] indices;
	delete[] baseLine;

	//Setup Raster State
	//we need to see both side of the shell if its empty
	//so set rasterDesc.CullMode = D3D11_CULL_NONE; 
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE; 
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = true;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	HRESULT hr = _GD->CreateRasterizerState(&rasterDesc, &raster_state_);

	//use the 2 sided version
	ID3DBlob* pPixelShaderBuffer = NULL;
	hr = CompileShaderFromFile(L"../Assets/shader.fx", "PS2", "ps_4_0_level_9_1", &pPixelShaderBuffer);
	_GD->CreatePixelShader(pPixelShaderBuffer->GetBufferPointer(), pPixelShaderBuffer->GetBufferSize(), NULL, &pixel_shader_);
}

void VBSnail::tick(GameData* _GD)
{
	VBGO::tick(_GD);
}
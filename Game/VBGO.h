#pragma once
#include "GameObject.h"
#include "ConstantBuffer.h"

//=================================================================
//the base Game Object drawn using a Vertex and Index Buffer
//all of the main aspects of drawing it have a default which is pointed to by a static pointer
//this is only used if the version for this object is not set to nullptr
//=================================================================

class VBGO : public GameObject
{
public:
	VBGO();
	virtual ~VBGO();

	virtual void tick(GameData* _GD) override;
	virtual void draw(DrawData* _DD) override;

	//set up and then destroy all static default render stuff for VBGOs
	static void init(ID3D11Device* _graphics_device);
	static void clean_up();

	//Update static const buffer required for default rendering
	static void update_constant_buffer(DrawData* _DD);

protected:
	//Direct X information for the model for this GameObject
	ID3D11Buffer* vertex_buffer_;
	ID3D11Buffer* index_buffer_;
	DXGI_FORMAT index_format_;
	UINT num_prims_;

	//vertex topology in VB
	D3D_PRIMITIVE_TOPOLOGY topology_;

	static ID3D11VertexShader* VERTEX_SHADER;
	static ID3D11InputLayout* VERTEX_LAYOUT;
	static ID3D11PixelShader* PIXEL_SHADER;
	static ID3D11ShaderResourceView* TEXTURE_RV;
	static ID3D11Buffer* CONSTANT_BUFFER;	        //GPU side
	static ConstantBuffer* CPU_CB;			        //CPU side
	static ID3D11SamplerState* SAMPLER;
	static ID3D11RasterizerState* RASTER_STATE;

	ID3D11VertexShader* vertex_shader_;
	ID3D11InputLayout* vertex_layout_;
	ID3D11PixelShader* pixel_shader_;
	ID3D11ShaderResourceView* texture_rv_;
	ID3D11Buffer* constant_buffer_;                 //GPU side
	void* cpu_cb_;                                  //CPU side

	ID3D11SamplerState* sampler_;
	ID3D11RasterizerState* raster_state_;

	//once populated build an Index Buffer
	void BuildIB(ID3D11Device* _GD, void* _indices);

	//once populated build a Vertex Buffer
	void BuildVB(ID3D11Device* _GD, int _numVerts, void* _vertices);

	static HRESULT CompileShaderFromFile(WCHAR* _szFileName, LPCSTR _szEntryPoint, LPCSTR _szShaderModel, ID3DBlob** _ppBlobOut);

};

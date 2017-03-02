#include "VBGO.h"
#include "DrawData.h"
#include "Vertex.h"
#include "Camera.h"
#include "Light.h"

#include <d3dcompiler.h>
#include "DDSTextureLoader.h"

#include <windows.h>

#define DESTROY( x ) if( x ){ x->Release(); x = nullptr;}

//the base Game Object drawn using a Vertex and Index Buffer
//all of the main aspects of drawing it have a default which is pointed to by a static pointer
//this is only used if the version for this object is not set to nullptr

ID3D11VertexShader*			VBGO::VERTEX_SHADER = nullptr;
ID3D11InputLayout*			VBGO::VERTEX_LAYOUT = nullptr;
ID3D11PixelShader*			VBGO::PIXEL_SHADER = nullptr;
ID3D11ShaderResourceView*	VBGO::TEXTURE_RV = nullptr;
ID3D11Buffer*				VBGO::CONSTANT_BUFFER = nullptr;
ConstantBuffer*				VBGO::CPU_CB = nullptr;
ID3D11SamplerState*			VBGO::SAMPLER = nullptr;
ID3D11RasterizerState*		VBGO::RASTER_STATE = nullptr;

VBGO::VBGO()
    : vertex_buffer_(nullptr)
    , index_buffer_(nullptr)
    , index_format_(DXGI_FORMAT_R16_UINT)
    , num_prims_(0)
    , topology_(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
    , vertex_shader_(nullptr)
    , vertex_layout_(nullptr)
    , pixel_shader_(nullptr)
    , texture_rv_(nullptr)
    , constant_buffer_(nullptr)
    , cpu_cb_(nullptr)
    , sampler_(nullptr)
    , raster_state_(nullptr)
{
	//NOTE WE DON'T CREATE ANYTHING HERE
	//WHATEVER INHERITS THIS WILL DO THAT
}

VBGO::~VBGO()
{
	//BUT WE DO TIDY THEM AWAY
	DESTROY(vertex_buffer_);
	DESTROY(index_buffer_);
	DESTROY(vertex_shader_);
	DESTROY(vertex_layout_);
	DESTROY(pixel_shader_);
	DESTROY(texture_rv_);
	DESTROY(constant_buffer_);
	//if (cpu_cb_) delete cpu_cb_; delete this where created as there will know its type
	DESTROY(sampler_);
	DESTROY(raster_state_);
}

void VBGO::tick(GameData* _GD)
{
	GameObject::tick(_GD);
}

void VBGO::draw(DrawData* _DD)
{
	//set raster state
	ID3D11RasterizerState* useRasterS = raster_state_ ? raster_state_ : RASTER_STATE;
	_DD->d3d_immediate_context->RSSetState(useRasterS);

	//set standard Constant Buffer to match this object
	CPU_CB->world = world_.Transpose();
	CPU_CB->rot = rot_.Transpose();

	//Set vertex buffer
	UINT stride = sizeof(myVertex);
	UINT offset = 0;
	_DD->d3d_immediate_context->IASetVertexBuffers(0, 1, &vertex_buffer_, &stride, &offset);

	// Set index buffer
	_DD->d3d_immediate_context->IASetIndexBuffer(index_buffer_, index_format_, 0);

	//update the Constant Buffer in classes that inherit from this then push to the GPU here
	ID3D11Buffer* useCB = constant_buffer_ ? constant_buffer_ : CONSTANT_BUFFER;
	void* useCBData = cpu_cb_ ? cpu_cb_ : CPU_CB;

	_DD->d3d_immediate_context->UpdateSubresource(useCB, 0, NULL, useCBData, 0, 0);
	_DD->d3d_immediate_context->VSSetConstantBuffers(0, 1, &useCB);
	_DD->d3d_immediate_context->PSSetConstantBuffers(0, 1, &useCB);

	//unless it has it own set them to the static defaults

	//set primitive type used
	_DD->d3d_immediate_context->IASetPrimitiveTopology(topology_);

	//set  vertex layout
	//note that if you do use this you'll need to change the stride and offset above
	ID3D11InputLayout* useLayout = vertex_layout_ ? vertex_layout_ : VERTEX_LAYOUT;
	_DD->d3d_immediate_context->IASetInputLayout(useLayout);

	//set Vertex Shader
	ID3D11VertexShader* useVS = vertex_shader_ ? vertex_shader_ : VERTEX_SHADER;
	_DD->d3d_immediate_context->VSSetShader(useVS, NULL, 0);

	//set Pixel Shader
	ID3D11PixelShader* usePS = pixel_shader_ ? pixel_shader_ : PIXEL_SHADER;
	_DD->d3d_immediate_context->PSSetShader(usePS, NULL, 0);

	//set Texture
	ID3D11ShaderResourceView* useTex = texture_rv_ ? texture_rv_ : TEXTURE_RV;
	_DD->d3d_immediate_context->PSSetShaderResources(0, 1, &useTex);

	//set sampler
	ID3D11SamplerState* useSample = sampler_ ? sampler_ : SAMPLER;
	_DD->d3d_immediate_context->PSSetSamplers(0, 1, &useSample);

	//and draw
	_DD->d3d_immediate_context->DrawIndexed(3 * num_prims_, 0, 0);//number here will need to change depending on the primative topology!
}

//--------------------------------------------------------------------------------------
// Compile the  shader
//--------------------------------------------------------------------------------------
HRESULT VBGO::CompileShaderFromFile(WCHAR* _szFileName, LPCSTR _szEntryPoint, LPCSTR _szShaderModel, ID3DBlob** _ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DCompileFromFile(_szFileName, NULL, NULL, _szEntryPoint, _szShaderModel,
		dwShaderFlags, 0, _ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob != NULL)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
		return hr;
	}

	return S_OK;
}

void VBGO::init(ID3D11Device* _graphics_device)
{
	//set up all static stuff

	//default vertex shader
	ID3DBlob* pVertexShaderBuffer = NULL;
	HRESULT hr = CompileShaderFromFile(L"../Assets/shader.fx", "VS", "vs_4_0_level_9_1", &pVertexShaderBuffer);
	_graphics_device->CreateVertexShader(pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(), NULL, &VERTEX_SHADER);

	//default pixelshader
	ID3DBlob* pPixelShaderBuffer = NULL;
	hr = CompileShaderFromFile(L"../Assets/shader.fx", "PS", "ps_4_0_level_9_1", &pPixelShaderBuffer);
	_graphics_device->CreatePixelShader(pPixelShaderBuffer->GetBufferPointer(), pPixelShaderBuffer->GetBufferSize(), NULL, &PIXEL_SHADER);

	//default vertex layout
	_graphics_device->CreateInputLayout(myVertexLayout, ARRAYSIZE(myVertexLayout), pVertexShaderBuffer->GetBufferPointer(),
		pVertexShaderBuffer->GetBufferSize(), &VERTEX_LAYOUT);

	//default texture (white square)
#ifdef DEBUG
	hr = CreateDDSTextureFromFile(_graphics_device, L"../Debug/white.dds", nullptr, &TEXTURE_RV);
#else
	hr = CreateDDSTextureFromFile(_graphics_device, L"../Release/white.dds", nullptr, &TEXTURE_RV);
#endif

	//deafult const buffer
	//GPU side
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = _graphics_device->CreateBuffer(&bd, NULL, &CONSTANT_BUFFER);
	//CPU side
	CPU_CB = new ConstantBuffer();
	ZeroMemory(CPU_CB, sizeof(ConstantBuffer));

	//default sampler
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(SamDesc));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.MipLODBias = 0.0f;
	SamDesc.MaxAnisotropy = 1;
	SamDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	SamDesc.BorderColor[0] = SamDesc.BorderColor[1] = SamDesc.BorderColor[2] = SamDesc.BorderColor[3] = 0;
	SamDesc.MinLOD = 0;
	SamDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = _graphics_device->CreateSamplerState(&SamDesc, &SAMPLER);

	//Setup Raster State
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = true;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	hr = _graphics_device->CreateRasterizerState(&rasterDesc, &RASTER_STATE);
}

void VBGO::update_constant_buffer(DrawData* _DD)
{
	//you'll need your own version of this if you use a different Constant Buffer
	CPU_CB->view = _DD->camera->get_view().Transpose();
	CPU_CB->projection = _DD->camera->get_proj().Transpose();
	if (_DD->light)
	{
		CPU_CB->light_colour = _DD->light->get_colour();
		CPU_CB->pos = _DD->light->get_pos();
		CPU_CB->ambient_colour = _DD->light->get_ambient_colour();
	}
}

void VBGO::clean_up()
{
	//clear away all the static objects
	DESTROY(VERTEX_SHADER);
	DESTROY(VERTEX_LAYOUT);
	DESTROY(PIXEL_SHADER);
	DESTROY(TEXTURE_RV);
	DESTROY(CONSTANT_BUFFER);
	if (CPU_CB)
	{
		delete CPU_CB;
		CPU_CB = nullptr;
	}
	DESTROY(SAMPLER);
	DESTROY(RASTER_STATE);
}

void VBGO::BuildIB(ID3D11Device* _GD, void* _indices)
{
	//structures from creating buffers
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;
	HRESULT hr = S_OK;

	//build index buffer
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 3 * num_prims_;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = _indices;
	hr = _GD->CreateBuffer(&bd, &InitData, &index_buffer_);
}

void VBGO::BuildVB(ID3D11Device* _GD, int _numVerts, void* _vertices)
{
	//structures from creating buffers
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;
	HRESULT hr = S_OK;

	//build vertex buffer
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(myVertex) * _numVerts;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = _vertices;
	hr = _GD->CreateBuffer(&bd, &InitData, &vertex_buffer_);
}
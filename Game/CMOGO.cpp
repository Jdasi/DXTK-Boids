#include "CMOGO.h"
#include "Camera.h"
#include "DrawData.h"
#include "StringUtils.h"

#include "Effects.h"
#include "CommonStates.h"

#include <windows.h>

ID3D11RasterizerState* CMOGO::raster_state_ = nullptr;
int CMOGO::count_ = 0;

CMOGO::CMOGO(const std::string& _file_name, ID3D11Device* _d3d_device, IEffectFactory* _EF)
    : model_(nullptr)
{
	//if we've not created it yet do so now
	if (!raster_state_)
	{
		//Setup Raster State
		D3D11_RASTERIZER_DESC rasterDesc;
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		// Create the rasterizer state from the description we just filled out.
		HRESULT hr = _d3d_device->CreateRasterizerState(&rasterDesc, &raster_state_);
	}

    std::string filePath =
#ifdef DEBUG
		"../Debug/"
#else
		"../Release/"
#endif
		+ _file_name;

	wchar_t* file = StringUtils::charToWChar(filePath.c_str());

	model_ = Model::CreateFromCMO(_d3d_device, file, *_EF);
}

CMOGO::~CMOGO()
{
	//model shouldn't need deleting as it's linked to by a unique_ptr
	count_--;

	//okay I've just deleted the last CMOGO let's get rid of this
	if (count_ == 0 && raster_state_)
	{
		raster_state_->Release();
		raster_state_ = nullptr;
	}
}

void CMOGO::tick(GameData* _GD)
{
	GameObject::tick(_GD);
}

void CMOGO::draw(DrawData* _DD)
{
	//a dirty hack as the CMO model drawer breaks the depth stencil state
	ID3D11DepthStencilState *DSS = nullptr;
	UINT ref;

	//pick up a copy of the current state...
	_DD->d3d_immediate_context->OMGetDepthStencilState(&DSS, &ref);

	model_->Draw(_DD->d3d_immediate_context, *_DD->states, //graphics device and CommonStates reuqired by model system
		world_, //world transform to poisiton this model in the world
		_DD->camera->get_view(), _DD->camera->get_proj(), //veiw and projection matrix of the camera
		false, //NO! I don't want wireframe
		[&](){_DD->d3d_immediate_context->RSSetState(raster_state_);} //this VERY weird construction creates a function on the fly to set up the render states correctly else the model system overrides them BADLY
		);

	//...and put the depth stencil state back again
	_DD->d3d_immediate_context->OMSetDepthStencilState(DSS, ref);

	//clear this copy away
	if (DSS) 
	{
		DSS->Release();
	}
}

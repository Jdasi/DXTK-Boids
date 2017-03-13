#include "CMOGO.h"
#include "CMOManager.h"
#include "CMOModel.h"
#include "Camera.h"
#include "DrawData.h"
#include "StringUtils.h"

#include "Effects.h"
#include "CommonStates.h"

#include <windows.h>

CMOGO::CMOGO(CMOModel* _model)
    : model_(_model)
{
}

void CMOGO::tick(GameData* _GD)
{
	GameObject::tick(_GD);
}

void CMOGO::draw(DrawData* _DD)
{
    if (!model_)
        return;

	//a dirty hack as the CMO model drawer breaks the depth stencil state
	ID3D11DepthStencilState *DSS = nullptr;
	UINT ref;

	//pick up a copy of the current state...
	_DD->d3d_immediate_context->OMGetDepthStencilState(&DSS, &ref);

	model_->get_model()->Draw(_DD->d3d_immediate_context, *_DD->states, //graphics device and CommonStates reuqired by model system
		world_, //world transform to poisiton this model in the world
		_DD->camera->get_view(), _DD->camera->get_proj(), //veiw and projection matrix of the camera
		false, //NO! I don't want wireframe
		[&](){_DD->d3d_immediate_context->RSSetState(model_->get_raster_state());} //this VERY weird construction creates a function on the fly to set up the render states correctly else the model system overrides them BADLY
		);

	//...and put the depth stencil state back again
	_DD->d3d_immediate_context->OMSetDepthStencilState(DSS, ref);

	//clear this copy away
	if (DSS) 
	{
		DSS->Release();
	}
}

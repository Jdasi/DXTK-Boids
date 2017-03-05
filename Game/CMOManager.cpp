#include "CMOManager.h"
#include "CMOModel.h"

CMOManager::CMOManager(ID3D11Device& _d3d_device, IEffectFactory& _ef)
    : d3d_device_(_d3d_device)
    , ef_(_ef)
    , raster_state_(nullptr)
{
    // Setup Raster State.
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
    HRESULT hr = _d3d_device.CreateRasterizerState(&rasterDesc, &raster_state_);
}

CMOManager::~CMOManager()
{
    if (raster_state_)
    {
        raster_state_->Release();
        raster_state_ = nullptr;
    }
}

CMOModel* CMOManager::get_model(const std::string& _file_name)
{
    auto entry = loaded_models_.find(_file_name);
    if (entry != loaded_models_.end())
        return entry->second.get();

    return create_model_entry(_file_name);
}

CMOModel* CMOManager::create_model_entry(const std::string& _file_name)
{
    auto model = std::make_unique<CMOModel>(_file_name, d3d_device_, ef_, *raster_state_);
    auto model_p = model.get();

    loaded_models_[_file_name] = std::move(model);

    return model_p;
}

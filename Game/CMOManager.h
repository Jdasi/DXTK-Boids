#pragma once
#include <Effects.h>

#include <map>
#include <memory>

using namespace DirectX;

class CMOModel;

class CMOManager
{
public:
    CMOManager(ID3D11Device& _d3d_device, IEffectFactory& _ef);
    ~CMOManager();

    CMOModel* get_model(const std::string& _file_name);

private:
    CMOModel* create_model_entry(const std::string& _file_name);

    std::map<std::string, std::unique_ptr<CMOModel>> loaded_models_;
    ID3D11Device& d3d_device_;
    IEffectFactory& ef_;

    // CMOGOs need a different raster state.
    ID3D11RasterizerState*  raster_state_;

};

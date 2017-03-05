#pragma once
#include <Model.h>

using namespace DirectX;

/* A class that stores a unique_ptr to a DirectX::model.
* The CMOFactory creates instances of this class whenever a new model needs to be loaded.
* Thereafter, a pointer to one of these instances is passed around to prevent
* reloading already loaded assets.
*/
class CMOModel
{
public:
    CMOModel(const std::string& _file_name, ID3D11Device& _d3d_device, IEffectFactory& _ef, ID3D11RasterizerState& _raster_state);
    ~CMOModel() = default;

    Model* get_model() const;
    ID3D11RasterizerState* get_raster_state() const;

private:
    std::unique_ptr<Model> model_;
    ID3D11RasterizerState& raster_state_;

};

#include "CMOModel.h"
#include "StringUtils.h"

CMOModel::CMOModel(const std::string& _file_name, ID3D11Device& _d3d_device, IEffectFactory& _ef, ID3D11RasterizerState& _raster_state)
    : raster_state_(_raster_state)
{
    std::string file_path =
#ifdef DEBUG
        "../Debug/"
#else
        "../Release/"
#endif
        + _file_name + ".cmo";

    wchar_t *file = StringUtils::char_to_wchar(file_path.c_str());
    model_ = Model::CreateFromCMO(&_d3d_device, file, _ef);
}

Model* CMOModel::get_model() const
{
    return model_.get();
}

ID3D11RasterizerState* CMOModel::get_raster_state() const
{
    return &raster_state_;
}

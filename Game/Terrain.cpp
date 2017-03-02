#include "Terrain.h"

Terrain::Terrain(const std::string& _file_name, ID3D11Device* _d3d_device, IEffectFactory* _EF, Vector3 _pos, float _yaw, float _pitch, float _roll, Vector3 _scale)
    : CMOGO(_file_name, _d3d_device, _EF)
{
	pos_ = _pos;
    yaw_ = _yaw;
	pitch_ = _pitch;
	roll_ = _roll;
	scale_ = _scale;

    // Update world transform.
	GameObject::tick(nullptr);
}

#include "Terrain.h"

Terrain::Terrain(CMOModel* _model, Vector3 _pos, float _yaw, float _pitch, float _roll, Vector3 _scale)
    : CMOGO(_model)
{
	pos_ = _pos;
    yaw_ = _yaw;
	pitch_ = _pitch;
	roll_ = _roll;
	scale_ = _scale;

    // Update world transform.
	GameObject::tick(nullptr);
}

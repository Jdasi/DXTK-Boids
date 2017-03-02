#include "TPSCamera.h"
#include "GameData.h"

TPSCamera::TPSCamera(float _fov, float _aspect_ratio, float _near_plane_dist, float _far_plane_dist, GameObject* _target, Vector3 _up, Vector3 _dpos)
	: Camera(_fov, _aspect_ratio, _near_plane_dist, _far_plane_dist, _up)
    , target_object_(_target)
    , dpos_(_dpos)
{
}

void TPSCamera::tick(GameData* _GD)
{
	// Set up position of camera and target position of camera based on new position and orientation of target object.
	Matrix rotCam = Matrix::CreateFromYawPitchRoll(target_object_->get_yaw(), 0.0f, 0.0f);
	target_ = target_object_->get_pos();
	pos_ = target_ + Vector3::Transform(dpos_, rotCam) ;

	// Set up proj and view matrices.
	Camera::tick(_GD);
}

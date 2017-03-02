#include "Camera.h"

Camera::Camera(float _fov, float _aspect_ratio, float _near_plane_dist, float _far_plane_dist, Vector3 _up, Vector3 _target)
    : fov_(_fov)
    , aspect_ratio_(_aspect_ratio)
    , near_plane_dist_(_near_plane_dist)
    , far_plane_dist_(_far_plane_dist)
    , target_(_target)
    , up_(_up)
{
}

void Camera::tick(GameData* _GD)
{
	projection_ = Matrix::CreatePerspectiveFieldOfView( fov_, aspect_ratio_, near_plane_dist_, far_plane_dist_ );
	view_ = Matrix::CreateLookAt( pos_, target_, up_ );
	GameObject::tick(_GD);
}

void Camera::draw(DrawData* _DD)
{
	//standard camera doesn't draw ANYTHING
}

Matrix Camera::get_view() const
{
    return view_;
}

Matrix Camera::get_proj() const
{
    return projection_;
}

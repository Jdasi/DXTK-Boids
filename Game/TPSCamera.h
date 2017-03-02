#pragma once
#include "Camera.h"

//=================================================================
//TPS style camera which will follow a given GameObject around _target
//=================================================================

class TPSCamera : public Camera
{
public:
	TPSCamera(float _fov, float _aspect_ratio, float _near_plane_dist, float _far_plane_dist, GameObject* _target, Vector3 _up, Vector3 _dpos);
	virtual ~TPSCamera() = default;

	virtual void tick(GameData* _GD) override;

protected:
	GameObject*	target_object_; // Follow this object.
	Vector3	dpos_; // Hover with this offset.

};

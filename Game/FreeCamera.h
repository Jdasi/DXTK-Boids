#pragma once
#include "Camera.h"

//=================================================================
//TPS style camera which will follow a given GameObject around _target
//=================================================================

class FreeCamera : public Camera
{
public:
    FreeCamera(float _fov, float _aspect_ratio, float _near_plane_dist, float _far_plane_dist, Vector3 _target, Vector3 _up, Vector3 _dpos);
    virtual ~FreeCamera() = default;

    virtual void tick(GameData* _GD) override;

protected:
    void rotate_cam(GameData* _GD);
    void zoom_cam(GameData* _GD);

    Vector3 look_at_; // Follow this position.
    Vector3	dpos_; // Hover with this offset.
    bool orbiting_; // Whether or not the camera is in orbiting mode (user input).

    float sensitivity_x_ = 0.01f;
    float sensitivity_y_ = 0.01f;

    float min_scroll_ = 50.f;
    float max_scroll_ = 500.0f;
    float scroll_speed_ = 0.25f;

};

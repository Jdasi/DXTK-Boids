#pragma once
#include "Camera.h"

//=================================================================
// Tabletop Sim style camera that follows an invisible GameObject
//=================================================================

class TabletopCamera : public Camera
{
public:
    TabletopCamera(float _fov, float _aspect_ratio, float _near_plane_dist, float _far_plane_dist, Vector3 _up, float _initial_pitch, float _initial_zoom);
    virtual ~TabletopCamera() = default;

    virtual void tick(GameData* _GD) override;

    Vector3 get_origin_point() const;

protected:
    void rotate_cam(GameData* _GD);
    void zoom_cam(GameData* _GD);
    void move_origin(GameData* _GD);

    Vector3 origin_point_; // The camera is centered on this point.
    Vector3	dpos_; // Hover from the origin point with this offset.
    bool orbiting_; // Whether or not the camera is in orbiting mode (user input).

    // Magic numbers galore.
    float sensitivity_x_ = 0.01f;
    float sensitivity_y_ = 0.01f;

    float min_scroll_ = 40.f;
    float max_scroll_ = 500.0f;

    float min_pitch_ = -1.0f;
    float max_pitch_ = -0.15f;

    float shift_modifier_ = 1.0f;
    float shift_modifier_amount_ = 3.0f;
    float move_speed_ = 30.0f;
    float scroll_speed_ = 0.1f;

};

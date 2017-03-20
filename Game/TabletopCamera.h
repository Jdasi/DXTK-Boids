#pragma once
#include "Camera.h"

// Tabletop Simulator style camera that orbits around a point on the floor.
class TabletopCamera : public Camera
{
public:
    TabletopCamera(float _fov, float _aspect_ratio, float _near_plane_dist, float _far_plane_dist, Vector3 _up, float _initial_pitch, float _initial_zoom);
    virtual ~TabletopCamera() = default;

    virtual void tick(GameData* _GD) override;

protected:
    void rotate_cam(GameData* _GD);
    void zoom_cam(GameData* _GD);
    void move_origin(GameData* _GD);

    Vector3 origin_point_;  // The camera is centered on this point.
    Vector3	dpos_;          // Hover from the origin point with this offset.
    bool orbiting_;         // Whether or not the camera is in orbiting mode (user input).

    float min_pitch_;
    float max_pitch_;
    float speed_modifier_;

};

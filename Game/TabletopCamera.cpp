#include "TabletopCamera.h"
#include "GameData.h"
#include "InputHandler.h"
#include "JMath.h"

TabletopCamera::TabletopCamera(float _fov, float _aspect_ratio, float _near_plane_dist, float _far_plane_dist, Vector3 _up, float _initial_pitch, float _initial_zoom)
    : Camera(_fov, _aspect_ratio, _near_plane_dist, _far_plane_dist, _up)
    , dpos_(Vector3(0, 0, _initial_zoom))
    , orbiting_(false)
{
    pitch_ = JMath::clampf(_initial_pitch, min_pitch_, max_pitch_);
    dpos_.z = JMath::clampf(dpos_.z, min_scroll_, max_scroll_);
}

void TabletopCamera::tick(GameData* _GD)
{
    if (_GD->game_state != GameState::GS_PLAY_MAIN_CAM)
        return;

    orbiting_ = _GD->input_handler->get_mouse_button(1);
    shift_modifier_ = _GD->input_handler->get_key(DIK_LSHIFT) ? shift_modifier_amount_ : 1.0f;

    rotate_cam(_GD);
    zoom_cam(_GD);
    move_origin(_GD);

    Matrix rot_mat = Matrix::CreateFromYawPitchRoll(get_yaw(), get_pitch(), 0.0f);

    set_pos(origin_point_ + Vector3::Transform(dpos_, rot_mat));
    target_ = origin_point_;

    _GD->boid_spawn_pos = origin_point_;

    // Set up proj and view matrices.
    Camera::tick(_GD);
}

Vector3 TabletopCamera::get_origin_point() const
{
    return origin_point_;
}

void TabletopCamera::rotate_cam(GameData* _GD)
{
    if (!orbiting_)
        return;

    yaw_ -= _GD->input_handler->get_mouse_state().lX * sensitivity_x_;
    pitch_ -= _GD->input_handler->get_mouse_state().lY * sensitivity_y_;

    pitch_ = JMath::clampf(pitch_, min_pitch_, max_pitch_);
}

void TabletopCamera::zoom_cam(GameData* _GD)
{
    dpos_.z -= _GD->input_handler->get_mouse_state().lZ * scroll_speed_ * shift_modifier_;
    dpos_.z = JMath::clampf(dpos_.z, min_scroll_, max_scroll_);
}

void TabletopCamera::move_origin(GameData* _GD)
{
    Matrix rot_mat = Matrix::CreateRotationY(yaw_);

    Vector3 forward_move = move_speed_ * Vector3::Forward;
    forward_move = Vector3::Transform(forward_move, rot_mat);

    Vector3 side_move = move_speed_ * Vector3::Left;
    side_move = Vector3::Transform(side_move, rot_mat);

    if (_GD->input_handler->get_key(DIK_W))
    {
        origin_point_ += forward_move * shift_modifier_ * _GD->delta_time;
    }

    if (_GD->input_handler->get_key(DIK_S))
    {
        origin_point_ -= forward_move * shift_modifier_ * _GD->delta_time;
    }

    if (_GD->input_handler->get_key(DIK_A))
    {
        origin_point_ += side_move * shift_modifier_ * _GD->delta_time;
    }

    if (_GD->input_handler->get_key(DIK_D))
    {
        origin_point_ -= side_move * shift_modifier_ * _GD->delta_time;
    }
}

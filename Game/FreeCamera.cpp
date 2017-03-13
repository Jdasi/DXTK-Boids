#include "FreeCamera.h"
#include "GameData.h"
#include "InputHandler.h"
#include "JMath.h"

FreeCamera::FreeCamera(float _fov, float _aspect_ratio, float _near_plane_dist, float _far_plane_dist, GameObject* _follow_target, Vector3 _up, Vector3 _dpos)
    : Camera(_fov, _aspect_ratio, _near_plane_dist, _far_plane_dist, _up)
    , follow_object_(_follow_target)
    , dpos_(_dpos)
    , orbiting_(false)
{
    dpos_.z = JMath::clampf(dpos_.z, min_scroll_, max_scroll_);
}

void FreeCamera::tick(GameData* _GD)
{
    orbiting_ = _GD->input_handler->get_mouse_button(1) && _GD->game_state == GameState::GS_PLAY_MAIN_CAM;

    rotate_cam(_GD);
    zoom_cam(_GD);

    Matrix rotCam = Matrix::CreateFromYawPitchRoll(get_yaw(), get_pitch(), 0.0f);

    set_pos(follow_object_->get_pos() + Vector3::Transform(dpos_, rotCam));
    target_ = follow_object_->get_pos();

    follow_object_->set_yaw(get_yaw());

    // Set up proj and view matrices.
    Camera::tick(_GD);
}

void FreeCamera::rotate_cam(GameData* _GD)
{
    if (!orbiting_)
        return;

    yaw_ -= _GD->input_handler->get_mouse_state().lX * sensitivity_x_;
    pitch_ -= _GD->input_handler->get_mouse_state().lY * sensitivity_y_;

    pitch_ = JMath::clampf(pitch_, -1.0f, 1.0f);
}

void FreeCamera::zoom_cam(GameData* _GD)
{
    float shift_modifier = _GD->input_handler->get_key(DIK_LSHIFT) ? 3.0f : 1.0f;

    dpos_.z -= _GD->input_handler->get_mouse_state().lZ * scroll_speed_ * shift_modifier;
    dpos_.z = JMath::clampf(dpos_.z, min_scroll_, max_scroll_);
}

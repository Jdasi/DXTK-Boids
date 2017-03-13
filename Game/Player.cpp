#include "Player.h"
#include "GameData.h"
#include "InputHandler.h"

Player::Player(CMOModel* _model)
    : CMOGO(_model)
{
	set_drag(0.7f);
	set_physics_enabled(true);
}

void Player::horizontal_movement(GameData* _GD)
{
    if (!visible_)
        return;

    Matrix rot_mat = Matrix::CreateRotationY(yaw_);

    Vector3 forward_move = move_speed_ * Vector3::Forward;
    forward_move = Vector3::Transform(forward_move, rot_mat);

    // Horizontal movement.
    if (_GD->input_handler->get_key(DIK_W))
    {
        acceleration_ += forward_move;
    }

    if (_GD->input_handler->get_key(DIK_S))
    {
        acceleration_ -= forward_move;
    }
}

void Player::vertical_movement(GameData* _GD)
{
    if (!visible_)
        return;

    if (_GD->input_handler->get_key(DIK_F))
    {
        acceleration_.y += move_speed_;
    }

    if (_GD->input_handler->get_key(DIK_R))
    {
        acceleration_.y -= move_speed_;
    }
}

void Player::rotate(GameData* _GD)
{
    if (!visible_)
        return;

    if (_GD->input_handler->get_key(DIK_A))
    {
        yaw_ += rotate_speed_ * _GD->delta_time;
    }

    if (_GD->input_handler->get_key(DIK_D))
    {
        yaw_ -= rotate_speed_ * _GD->delta_time;
    }
}

void Player::limit_speed()
{
    float current_speed = pos_.Length();

    if (current_speed > max_speed_)
    {
        pos_.Normalize();
        pos_ *= max_speed_;
        velocity_ *= -0.9f; // Simple bounce back.
    }
}

void Player::tick(GameData* _GD)
{
    visible_ = _GD->game_state == GameState::GS_PLAY_TPS_CAM ? true : false;

    horizontal_movement(_GD);
    vertical_movement(_GD);
    rotate(_GD);
    limit_speed();

    if (visible_)
        _GD->boid_spawn_pos = Vector3(pos_.x, 0, pos_.z);

	// Base behaviour.
	CMOGO::tick(_GD);
}

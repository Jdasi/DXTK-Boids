#include "Player.h"
#include "GameData.h"
#include "InputHandler.h"
#include "Constants.h"

Player::Player(CMOModel* _model)
    : CMOGO(_model)
{
	set_drag(0.7f);
	set_physics_enabled(true);
}

// Allow the player to move forward or backward based on input.
void Player::horizontal_movement(GameData* _GD)
{
    Matrix rot_mat = Matrix::CreateRotationY(yaw_);

    Vector3 forward_move = PLAYER_MOVE_SPEED * Vector3::Forward;
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

// Allow the player to rotate based on input.
void Player::rotate(GameData* _GD)
{
    if (_GD->input_handler->get_key(DIK_A))
    {
        yaw_ += PLAYER_ROTATE_SPEED * _GD->delta_time;
    }

    if (_GD->input_handler->get_key(DIK_D))
    {
        yaw_ -= PLAYER_ROTATE_SPEED * _GD->delta_time;
    }
}

// Prevent the player from moving too fast.
void Player::limit_speed()
{
    float current_speed = pos_.Length();

    if (current_speed > PLAYER_MAX_SPEED)
    {
        pos_.Normalize();
        pos_ *= PLAYER_MAX_SPEED;
        velocity_ *= -0.9f; // Simple bounce back.
    }
}

void Player::tick(GameData* _GD)
{
    // Player is only visible and controllable while the TPS camera is active.
    visible_ = _GD->active_camera == CAM_TPS;

    if (visible_)
    {
        horizontal_movement(_GD);
        rotate(_GD);
        
        _GD->boid_spawn_pos = Vector3(pos_.x, 0, pos_.z);
    }
    
    // The player can still slow down even when the TPS camera is not active.
    limit_speed();

	// Base behaviour.
	CMOGO::tick(_GD);
}

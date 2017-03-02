#include "Player.h"
#include "GameData.h"
#include "InputHandler.h"

Player::Player(const std::string& _file_name, ID3D11Device* _d3d_device, IEffectFactory* _EF)
    : CMOGO(_file_name, _d3d_device, _EF)
{
	fudge_ = Matrix::CreateRotationY(XM_PI);

	set_drag(0.7f);
	set_physics_enabled(true);
}

void Player::tick(GameData* _GD)
{
	switch (_GD->game_state)
	{
	case GS_PLAY_MAIN_CAM:
	{
		{
			// Mouse controls.
			float speed = 10.0f;
			acceleration_.x += speed * _GD->input_handler->get_mouse_state().lX;
			acceleration_.z += speed * _GD->input_handler->get_mouse_state().lY;
			break;
		}
	}
	case GS_PLAY_TPS_CAM:
	{
		// Keyboard controls.
		Vector3 forwardMove = 40.0f * Vector3::Forward;
		Matrix rotMove = Matrix::CreateRotationY(yaw_);
		forwardMove = Vector3::Transform(forwardMove, rotMove);
		if (_GD->input_handler->get_button(DIK_W))
		{
			acceleration_ += forwardMove;
		}
		if (_GD->input_handler->get_button(DIK_S))
		{
			acceleration_ -= forwardMove;
		}
		break;
	}
	}

	// Player rotation.
	float rotSpeed = 2.0f * _GD->delta_time;
	if (_GD->input_handler->get_button(DIK_A))
	{
		yaw_ += rotSpeed;
	}
	if (_GD->input_handler->get_button(DIK_D))
	{
		yaw_ -= rotSpeed;
	}

	// Up-down movement.
	if (_GD->input_handler->get_button(DIK_R))
	{
		acceleration_.y += 40.0f;
	}

	if (_GD->input_handler->get_button(DIK_F))
	{
		acceleration_.y -= 40.0f;
	}

	// Limit speed.
	float length = pos_.Length();
	float maxLength = 500.0f;
	if (length > maxLength)
	{
		pos_.Normalize();
		pos_ *= maxLength;
		velocity_ *= -0.9f; //VERY simple bounce back
	}

	// Base behaviour.
	CMOGO::tick(_GD);
}

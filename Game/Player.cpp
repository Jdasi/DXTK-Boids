#include "Player.h"
#include "GameData.h"
#include "InputHandler.h"

Player::Player(CMOModel* _model)
    : CMOGO(nullptr)
{
	set_drag(0.7f);
	set_physics_enabled(true);
}

void Player::tick(GameData* _GD)
{
    float shift_modifier = _GD->input_handler->get_key(DIK_LSHIFT) ? 3.0f : 1.0f;

	// Keyboard controls.
	Vector3 forwardMove = 20.0f * Vector3::Forward;

	Matrix rotMove = Matrix::CreateRotationY(yaw_);
	forwardMove = Vector3::Transform(forwardMove, rotMove);

    Vector3 sideMove = 20.0f * Vector3::Left;
    sideMove = Vector3::Transform(sideMove, rotMove);

	if (_GD->input_handler->get_key(DIK_W))
	{
		pos_ += forwardMove * shift_modifier * _GD->delta_time;
	}

	if (_GD->input_handler->get_key(DIK_S))
	{
		pos_ -= forwardMove * shift_modifier * _GD->delta_time;
	}

	// Player rotation.
	if (_GD->input_handler->get_key(DIK_A))
	{
        pos_ += sideMove * shift_modifier * _GD->delta_time;
	}

	if (_GD->input_handler->get_key(DIK_D))
	{
        pos_ -= sideMove * shift_modifier * _GD->delta_time;
	}

	// Base behaviour.
	CMOGO::tick(_GD);
}

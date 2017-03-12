#include "GameObject.h"
#include "GameData.h"

GameObject::GameObject()
    : pos_(Vector3::Zero)
    , scale_(Vector3::One)
    , yaw_(0)
    , pitch_(0)
    , roll_(0)
    , world_(Matrix::Identity)
    , fudge_(Matrix::Identity)
{
}

void GameObject::tick(GameData* _GD)
{
	if (physics_enabled_)
	{
		Vector3 newVel = velocity_ + _GD->delta_time * (acceleration_ - drag_*velocity_);
		Vector3 newPos = pos_ + _GD->delta_time * velocity_;

		velocity_ = newVel;
		pos_ = newPos;
	}

	//build up the world matrix depending on the new position of the GameObject
	//the assumption is that this class will be inherited by the class that ACTUALLY changes this
	Matrix  scaleMat = Matrix::CreateScale(scale_);
	rot_ = Matrix::CreateFromYawPitchRoll(yaw_, pitch_, roll_); //possible not the best way of doing this!
	Matrix  transMat = Matrix::CreateTranslation(pos_);

	world_ = fudge_ * scaleMat * rot_ * transMat;

	//zero acceleration ready for the next time round
	acceleration_ = Vector3::Zero;
}

Vector3 GameObject::get_pos() const
{
    return pos_;
}

void GameObject::set_pos(Vector3 _pos)
{
    pos_ = _pos;
}

Vector3 GameObject::get_scale() const
{
    return scale_;
}

void GameObject::set_scale(float _scale)
{
    scale_ = _scale * Vector3::One;
}

void GameObject::set_scale(float _x, float _y, float _z)
{
    scale_ = Vector3(_x, _y, _z);
}

void GameObject::set_scale(Vector3 _scale)
{
    scale_ = _scale;
}

float GameObject::get_yaw() const
{
    return yaw_;
}

void GameObject::set_yaw(float _yaw)
{
    yaw_ = _yaw;
}

float GameObject::get_pitch() const
{
    return pitch_;
}

void GameObject::set_pitch(float _pitch)
{
    pitch_ = _pitch;
}

float GameObject::get_roll() const
{
    return roll_;
}

void GameObject::set_roll(float _roll)
{
    roll_ = _roll;
}

void GameObject::set_yaw_pitch_roll(float _yaw, float _pitch, float _roll)
{
    yaw_ = _yaw;
    pitch_ = _pitch;
    roll_ = _roll;
}

bool GameObject::is_physics_enabled() const
{
    return physics_enabled_;
}

void GameObject::set_physics_enabled(bool _physics)
{
    physics_enabled_ = _physics;
}

void GameObject::toggle_physics()
{
    physics_enabled_ = !physics_enabled_;
}

float GameObject::get_drag() const
{
    return drag_;
}

Matrix GameObject::get_world_matrix()
{
    return world_;
}

void GameObject::set_world_matrix(Matrix _world)
{
    world_ = _world;
}

void GameObject::set_drag(float _drag)
{
    drag_ = _drag;
}

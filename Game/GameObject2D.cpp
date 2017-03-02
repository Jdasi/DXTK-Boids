#include "GameObject2D.h"
#include "GameData.h"
#include "DrawData2D.h"

GameObject2D::GameObject2D()
    : pos_(Vector2::Zero)
    , scale_(Vector2::One)
    , rot_(0)
    , colour_(Color(1.0f, 1.0f, 1.0f, 1.0f))
{
}

Vector2 GameObject2D::get_pos() const
{
    return pos_;
}

void GameObject2D::set_pos(Vector2 _pos)
{
    pos_ = _pos;
}

float GameObject2D::get_rot() const
{
    return rot_;
}

void GameObject2D::set_rot(float _rot)
{
    rot_ = _rot;
}

Color GameObject2D::get_colour() const
{
    return colour_;
}

void GameObject2D::set_colour(Color _colour)
{
    colour_ = _colour;
}

Vector2 GameObject2D::get_scale() const
{
    return scale_;
}

void GameObject2D::set_scale(Vector2 _scale)
{
    scale_ = _scale;
}

void GameObject2D::set_scale(float _scale)
{
    scale_ = _scale * Vector2::One;
}

Vector2 GameObject2D::get_origin() const
{
    return origin_;
}

void GameObject2D::set_origin(Vector2 _origin)
{
    origin_ = _origin;
}

#include "Light.h"
#include "GameData.h"

Light::Light(Vector3 _pos, Color _colour, Color _ambient_colour)
    : colour_ (_colour)
    , ambient_colour_(_ambient_colour)
{
	pos_ = _pos;
}

void Light::tick(GameData* _GD)
{
	//not really needed but spins the light around to show off the lambert default VBGO shader
	/*static float time = 0.0f;
	time += _GD->delta_time;
	pos.x = 100.0f * cos(time);
	pos.z = 100.0f * sin(time);
	if (time > XM_2PI) time -= XM_2PI;*/
}

void Light::set_colour(Color _colour)
{
    colour_ = _colour;
}

Color Light::get_colour() const
{
    return colour_;
}

void Light::set_ambient_colour(Color _colour)
{
    ambient_colour_ = _colour;
}

Color Light::get_ambient_colour() const
{
    return ambient_colour_;
}

#pragma once
#include "GameObject.h"

//=================================================================
//a simple light defined by a colour and and ambient colour and 
//used as a point light with no drop off in the standard shader 
//used to draw VBGOs
//=================================================================

struct DrawData;

class Light : public GameObject
{
public:
	Light(Vector3 _pos, Color _colour, Color _ambient_colour);
	~Light() = default;

	virtual void draw(DrawData* _DD) override {};
	virtual void tick(GameData* _GD) override;

    void set_colour(Color _colour);
    Color get_colour() const;

    void set_ambient_colour(Color _colour);
    Color get_ambient_colour() const;

protected:
	Color colour_;
	Color ambient_colour_;

};

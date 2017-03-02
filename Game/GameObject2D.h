#pragma once
#include <d3d11_1.h>
#include "SimpleMath.h"

//=================================================================
//the base Game Object 2D
//=================================================================

using namespace DirectX;
using namespace SimpleMath;

struct GameData;
struct DrawData2D;

class GameObject2D
{
public:
	GameObject2D();
	virtual ~GameObject2D() {};

	virtual void tick(GameData* _GD) = 0;
	virtual void draw(DrawData2D* _DD) = 0;

    Vector2 get_pos() const;
    void set_pos(Vector2 _pos);

    float get_rot() const;
    void set_rot(float _rot);

    Color get_colour() const;
    void set_colour(Color _colour);

    Vector2 get_scale() const;
    void set_scale(Vector2 _scale);
    void set_scale(float _scale);

    Vector2 get_origin() const;
    void set_origin(Vector2 _origin);

protected:
	Vector2 pos_;
    Vector2 scale_;
	float rot_;
	Color colour_;
	Vector2 origin_;

};

#pragma once
#include "CommonStates.h"
#include "SimpleMath.h"

//=================================================================
//Base Game Object Class
//=================================================================

using namespace DirectX;
using namespace SimpleMath;

class Camera;
struct ID3D11DeviceContext;
struct GameData;
struct DrawData;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject() = default;

	virtual void tick(GameData* _GD);
	virtual void draw(DrawData* _DD) = 0;

    Vector3 get_pos() const;
    void set_pos(Vector3 _pos);

    Vector3 get_scale() const;
    void set_scale(float _scale);
    void set_scale(float _x, float _y, float _z);
    void set_scale(Vector3 _scale);

    float get_yaw() const;
    void set_yaw(float _yaw);

    float get_pitch() const;
    void set_pitch(float _pitch);

    float get_roll() const;
    void set_roll(float _roll);

    void set_yaw_pitch_roll(float _yaw, float _pitch, float _roll);

    bool is_physics_enabled() const;
    void set_physics_enabled(bool _physics);
    void toggle_physics();

    float get_drag() const;
    void set_drag(float _drag);

protected:
    Vector3 pos_;
    Vector3 scale_;
    float yaw_;
    float pitch_;
    float roll_;
    Matrix rot_;
	Matrix world_;
	Matrix fudge_;

	//very basic physics
	bool physics_enabled_ = false;
	float drag_ = 0.0f;
	Vector3 velocity_ = Vector3::Zero;
	Vector3 acceleration_ = Vector3::Zero;

};

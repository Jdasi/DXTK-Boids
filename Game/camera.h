#pragma once
#include "GameObject.h"

//=================================================================
//Base Camera Class
//=================================================================

struct GameData;

class Camera : public GameObject
{
public:
	Camera(float _fov, float _aspect_ratio, float _near_plane_dist, float _far_plane_dist, Vector3 _up = Vector3::Up, Vector3 _target = Vector3::Zero);
	~Camera() = default;

	virtual void tick(GameData* _GD) override;
	virtual void draw(DrawData* _DD) override;

	//Getters
    Matrix get_proj() const;
    Matrix get_view() const;

    //Setters

protected:

	//Principle transforms/matrices for this camera
	Matrix projection_;
	Matrix view_;

	//parameters for setting up a camera
	float fov_;
	float aspect_ratio_;
	float near_plane_dist_;
	float far_plane_dist_;

	Vector3 target_;
	Vector3 up_;

};

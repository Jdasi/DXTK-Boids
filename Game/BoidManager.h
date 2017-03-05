#pragma once
#include "BoidData.h"

#include <d3d11.h>
#include "SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

class BoidManager
{
public:
    BoidManager(unsigned int _initial_boids);
    ~BoidManager() = default;

private:
    void add_boid(Vector3 _pos);

    BoidData boid_data;

};

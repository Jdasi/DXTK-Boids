#pragma once
#include "Boid.h"

#include <d3d11.h>
#include "SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

struct BoidData;
struct GameData;

class Rule
{
public:
    enum ID
    {
        Separation,
        Alignment,
        Cohesion
    };

    Rule()
        : this_boid_(nullptr)
    {
    }

    virtual ~Rule() = default;

    virtual Vector3 force(GameData* _GD, BoidData& _BD) = 0;

    void set_boid(Boid* _boid)
    {
        this_boid_ = _boid;
    }

protected:
    Boid* this_boid_;

};

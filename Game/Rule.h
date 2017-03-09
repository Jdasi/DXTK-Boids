#pragma once
#include "Boid.h"

#include <d3d11.h>
#include "SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

struct BoidData;
struct GameData;
struct BoidSettings;

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
        , boid_settings_(nullptr)
    {
    }

    virtual ~Rule() = default;

    virtual Vector3 force(GameData* _GD, BoidData& _BD) = 0;

    void set_boid(Boid* _boid, BoidSettings& _boid_settings)
    {
        this_boid_ = _boid;
        boid_settings_ = &_boid_settings;
    }

protected:
    Boid* this_boid_;
    BoidSettings* boid_settings_;

};

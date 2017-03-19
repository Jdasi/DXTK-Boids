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
    Rule()
        : this_boid_(nullptr)
        , boid_settings_(nullptr)
    {
    }

    virtual ~Rule() = default;

    virtual Vector3 force(GameData* _GD, std::vector<Boid*>& _neighbours, float _weight) = 0;

    void set_boid(Boid* _boid)
    {
        this_boid_ = _boid;
        boid_settings_ = this_boid_->getSettings();
    }

protected:
    Boid* this_boid_;
    BoidSettings* boid_settings_;

};

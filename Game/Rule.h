#pragma once
#include "Boid.h"

#include <d3d11.h>
#include "SimpleMath.h"

using namespace DirectX;
using namespace SimpleMath;

struct BoidData;
struct GameData;
struct BoidSettings;

/* Abstract class for all Rules to inherit from and implement.
 * This follows the 'Rules as Modules' design principle, and allows for easy
 * expansion to boid behaviour by adding different rules.
 */
class Rule
{
public:
    Rule()
        : this_boid_(nullptr)
        , boid_settings_(nullptr)
    {
    }

    virtual ~Rule() = default;

    virtual Vector3 force(GameData* _GD, std::vector<Boid*>& _neighbours, ParameterisedRule* _rule_params) = 0;

    // Tell the rule which boid it is going to be affecting.
    void set_boid(Boid* _boid)
    {
        this_boid_ = _boid;
        boid_settings_ = this_boid_->getSettings();
    }

protected:
    Boid* this_boid_; // The boid that is currently being acted upon.
    BoidSettings* boid_settings_; // The current boid's settings for quick reference.

};

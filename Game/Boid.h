#pragma once
#include "CMOGO.h"
#include "BoidSettings.h"

#include <d3d11.h>
#include "SimpleMath.h"

#include <vector>

using namespace DirectX;
using namespace SimpleMath;

class BoidManager;
class CMOModel;
struct BoidData;
class Rule;
struct DrawData;
struct GameData;

class Boid : public CMOGO
{
public:
    explicit Boid(CMOModel* _model);
    ~Boid() = default;

    void tick(GameData* _GD, BoidData& _BD);
    void draw(DrawData* _DD) override;

    void add_rule(Rule* _rule);

    const Vector3& get_velocity() const;
    BoidSettings& settings();

private:
    void rules(GameData* _GD, BoidData& _data);
    void move();
    void rotate();
    void wrap();
    void apply_force(Vector3 _force);

    Vector3 velocity_;
    Vector3 acceleration_;

    BoidSettings settings_;

};

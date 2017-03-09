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
    explicit Boid(BoidData& _BD, BoidSettings& _settings);
    ~Boid() = default;

    void tick(GameData* _GD) override;
    void draw(DrawData* _DD) override;

    const Vector3& get_velocity() const;
    BoidSettings& settings() const;

private:
    void rules(GameData* _GD);
    void move();
    void rotate();
    void wrap();
    void apply_force(Vector3 _force);

    Vector3 velocity_;
    Vector3 acceleration_;

    BoidData& BD_;
    BoidSettings& settings_;

};

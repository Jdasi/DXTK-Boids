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

    float get_scan_modifier() const;
    void set_scan_modifier(float _f);
    void modify_scan_modifier(float _f);

private:
    void rules(GameData* _GD);
    void move(GameData* _GD);
    void wrap();
    void apply_force(Vector3 _force);

    Vector3 velocity_;
    Vector3 acceleration_;
    float scan_modifier_;

    BoidData& BD_;
    BoidSettings& settings_;

};

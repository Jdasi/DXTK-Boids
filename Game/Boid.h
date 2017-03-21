#pragma once
#include "CMOGO.h"
#include "BoidSettings.h"

#include <d3d11.h>
#include "SimpleMath.h"

#include <vector>

using namespace DirectX;
using namespace SimpleMath;

class CMOModel;
struct BoidData;
class Rule;
struct DrawData;
struct GameData;

class Boid : public CMOGO
{
friend class BoidManager;

public:
    explicit Boid(BoidSettings* _settings);
    ~Boid() = default;

    void tick(GameData* _GD) override;
    void draw(DrawData* _DD) override;

    const Vector3& get_velocity() const;

    float get_scan_modifier() const;
    void reset_scan_modifier();
    void modify_scan_modifier(float _f);

    bool is_alive() const;
    void set_alive(bool _value);

    BoidSettings* getSettings() const;

private:
    void rules(GameData* _GD);
    void wrap();
    void move(GameData* _GD);
    void update_world();
    void apply_force(Vector3 _force);

    Vector3 velocity_;
    Vector3 acceleration_;
    float scan_modifier_; // A value that increases the search radius for other boids.
    bool alive_; // Whether the boid is alive or dead.

    BoidSettings* settings_; // Common settings of boids with this type (essentially a Breed).
    std::vector<Boid*> neighbours_; // Which agents are around the boid.

};

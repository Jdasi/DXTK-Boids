#pragma once
#include "BoidData.h"
#include "BoidSettings.h"
#include "Rule.h"

#include <d3d11.h>
#include "SimpleMath.h"

#include <map>
#include <memory>

using namespace DirectX;
using namespace SimpleMath;

class CMOManager;
struct DrawData;
struct GameData;

class BoidManager
{
public:
    BoidManager(CMOManager& _cmo_manager);
    ~BoidManager() = default;

    void tick(GameData* _GD);
    void draw(DrawData* _DD);

    BoidSettings& get_human_settings();
    BoidSettings& get_zombie_settings();

    int* get_num_boids();

private:
    void register_rules();
    void configure_boid_settings();
    void spawn_controls(GameData* _GD);
    void add_boid(BoidType _type, Vector3 _pos);

    CMOModel* fetch_model(BoidType _type) const;
    BoidSettings* fetch_settings(BoidType _type);

    std::map<Rule::ID, std::unique_ptr<Rule>> rules_;
    CMOManager& cmo_manager_;
    std::vector<std::unique_ptr<Boid>> boids_;
    int num_boids_;
    int max_boids_;

    BoidSettings human_settings_;
    BoidSettings zombie_settings_;

};

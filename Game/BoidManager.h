#pragma once
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

    int* get_num_boids();
    Rule* get_rule(const std::string& _rule) const;
    BoidSettings* get_boid_settings(const std::string& _type) const;

    void add_boid_type(const std::string& _str, std::unique_ptr<BoidSettings> _settings);

private:
    void register_rules();
    void spawn_controls(GameData* _GD);
    void add_boid(const std::string& _type, Vector3 _pos);

    BoidSettings* fetch_settings(const std::string& _type);

    CMOManager& cmo_manager_;

    std::map<std::string, std::unique_ptr<Rule>> rules_;
    std::map<std::string, std::unique_ptr<BoidSettings>> boid_types_;
    std::vector<std::unique_ptr<Boid>> boids_;

    int num_boids_;
    int max_boids_;

};

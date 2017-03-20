#pragma once
#include "BoidSettings.h"
#include "Rule.h"

#include <d3d11.h>
#include "SimpleMath.h"

#include <map>
#include <memory>
#include <functional>

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
    int* get_editable_spawn_id();

    Rule* get_rule(const std::string& _rule) const;
    std::function<void(Boid*, Boid*)> get_tag_function(const std::string& _str);

    const std::map<std::string, std::unique_ptr<BoidSettings>>& get_boid_types() const;
    void add_boid_type(const std::string& _str, std::unique_ptr<BoidSettings> _settings);

    void delete_all_of_current_type_selection();

private:
    void register_rules();
    void register_tag_functions();
    void update_spawn_selection();
    void spawn_controls(GameData* _GD);
    void add_boid(const std::string& _type, Vector3 _pos);
    void garbage_collect_boids();

    CMOManager& cmo_manager_;

    std::map<std::string, std::unique_ptr<Rule>> rules_;
    std::map<std::string, std::unique_ptr<BoidSettings>> boid_types_;
    std::map<std::string, std::function<void(Boid*, Boid*)>> tag_functions_;

    std::vector<std::unique_ptr<Boid>> boids_;
    bool boids_dirty_;
    int num_boids_;
    int editable_spawn_id_;
    BoidSettings* current_type_selection_;

};

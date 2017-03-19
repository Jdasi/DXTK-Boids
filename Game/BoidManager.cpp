#include "BoidManager.h"
#include "Boid.h"
#include "CMOManager.h"
#include "Rules.h"
#include "GameData.h"
#include "InputHandler.h"

BoidManager::BoidManager(CMOManager& _cmo_manager)
    : cmo_manager_(_cmo_manager)
    , num_boids_(0)
    , max_boids_(1500)
{
    register_rules();
}

void BoidManager::tick(GameData* _GD)
{
    spawn_controls(_GD);

    for (auto& boid : boids_)
    {
        boid->neighbours_.clear();

        for (auto& other : boids_)
        {
            if (boid == other)
                continue;

            if (Vector3::Distance(boid->get_pos(), other->get_pos()) < 
                boid->settings_->neighbour_scan * boid->scan_modifier_)
            {
                boid->neighbours_.push_back(other.get());
            }
        }

        boid->tick(_GD);
    }
}

void BoidManager::draw(DrawData* _DD)
{
    for (auto& boid : boids_)
    {
        boid->draw(_DD);
    }
}

Rule* BoidManager::get_rule(const std::string& _rule) const
{
    return rules_.at(_rule).get();
}

BoidSettings* BoidManager::get_boid_settings(const std::string& _type) const
{
    return boid_types_.at(_type).get();
}

int* BoidManager::get_num_boids()
{
    return &num_boids_;
}

void BoidManager::add_boid_type(const std::string& _str, std::unique_ptr<BoidSettings> _settings)
{
    auto entry = boid_types_.find(_str);
    if (entry != boid_types_.end())
        return;

    boid_types_[_str] = std::move(_settings);
}

void BoidManager::register_rules()
{
    rules_["separation"] = std::make_unique<Separation>();
    rules_["alignment"] = std::make_unique<Alignment>();
    rules_["cohesion"] = std::make_unique<Cohesion>();
}

void BoidManager::spawn_controls(GameData* _GD)
{
    if (_GD->input_handler->get_key(DIK_H))
        add_boid("human", _GD->boid_spawn_pos);

    if (_GD->input_handler->get_key(DIK_Z))
        add_boid("zombie", _GD->boid_spawn_pos);
}

void BoidManager::add_boid(const std::string& _type, Vector3 _pos)
{
    if (num_boids_ >= max_boids_)
        return;

    ++num_boids_;

    auto boid = std::make_unique<Boid>(fetch_settings(_type));
    boid->set_pos(_pos);

    boids_.push_back(std::move(boid));
}

BoidSettings* BoidManager::fetch_settings(const std::string& _type)
{
    return boid_types_.at(_type).get();
}

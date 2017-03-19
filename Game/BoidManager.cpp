#include "BoidManager.h"
#include "Boid.h"
#include "CMOManager.h"
#include "Rules.h"
#include "GameData.h"
#include "InputHandler.h"
#include "Constants.h"

#include <algorithm>

BoidManager::BoidManager(CMOManager& _cmo_manager)
    : cmo_manager_(_cmo_manager)
    , num_boids_(0)
    , editable_spawn_id_(0)
    , current_type_selection_(nullptr)
{
    register_rules();
    register_tag_functions();
}

void BoidManager::tick(GameData* _GD)
{
    update_spawn_selection();
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

std::function<void(Boid*, Boid*)> BoidManager::get_tag_function(const std::string& _str)
{
    return tag_functions_.at(_str);
}

BoidSettings* BoidManager::get_boid_settings(const std::string& _type) const
{
    return boid_types_.at(_type).get();
}

int BoidManager::get_num_types() const
{
    return boid_types_.size();
}

const std::map<std::string, std::unique_ptr<BoidSettings>>& BoidManager::get_boid_types() const
{
    return boid_types_;
}

int* BoidManager::get_num_boids()
{
    return &num_boids_;
}

int* BoidManager::get_editable_spawn_id()
{
    return &editable_spawn_id_;
}

void BoidManager::add_boid_type(const std::string& _str, std::unique_ptr<BoidSettings> _settings)
{
    auto entry = boid_types_.find(_str);
    if (entry != boid_types_.end())
        return;

    boid_types_[_str] = std::move(_settings);
    current_type_selection_ = boid_types_.begin()->second.get();
}

void BoidManager::register_rules()
{
    rules_["separation"] = std::make_unique<Separation>();
    rules_["alignment"] = std::make_unique<Alignment>();
    rules_["cohesion"] = std::make_unique<Cohesion>();
}

void BoidManager::register_tag_functions()
{
    // rhs converts lhs.
    tag_functions_["infect"] = [](Boid* lhs, Boid* rhs)
    {
        rhs->model_ = lhs->settings_->model;
        rhs->settings_ = lhs->settings_;
    };
}

void BoidManager::update_spawn_selection()
{
    if (!current_type_selection_)
        return;

    if (current_type_selection_->type_id != editable_spawn_id_)
    {
        auto it = std::find_if(boid_types_.begin(), boid_types_.end(), [this](const auto& _elem)
        {
            return _elem.second->type_id == editable_spawn_id_;
        });

        current_type_selection_ = it->second.get();
    }
}

void BoidManager::spawn_controls(GameData* _GD)
{
    if (_GD->input_handler->get_key(DIK_V))
        add_boid(current_type_selection_->type, _GD->boid_spawn_pos);
}

void BoidManager::add_boid(const std::string& _type, Vector3 _pos)
{
    if (num_boids_ >= MAX_BOIDS)
        return;

    ++num_boids_;

    auto boid = std::make_unique<Boid>(boid_types_.at(_type).get());
    boid->set_pos(_pos);

    boids_.push_back(std::move(boid));
}

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
    , boids_dirty_(false)
    , num_boids_(0)
    , editable_spawn_id_(0)
    , current_type_selection_(nullptr)
{
    register_rules();
    register_tag_functions();
}

// Updates each boid in the vector, and handles the input for spawning new boids.
void BoidManager::tick(GameData* _GD)
{
    update_spawn_selection();
    spawn_controls(_GD);

    // Each boid has a neighbour list which is populated based on their scan range.
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

        // Once the boid knows about its neighbours, update it.
        boid->tick(_GD);
    }

    // Erase any boids from the vector that are no longer alive.
    garbage_collect_boids();
}

void BoidManager::draw(DrawData* _DD)
{
    for (auto& boid : boids_)
    {
        boid->draw(_DD);
    }
}

// Fetches the registered rule with the same name as passed string.
Rule* BoidManager::get_rule(const std::string& _rule)
{
    return rules_.at(_rule).get();
}

// Fetches the registered tag function with the same name as passed string.
std::function<void(Boid*, Boid*)> BoidManager::get_tag_function(const std::string& _str)
{
    return tag_functions_.at(_str);
}

std::map<std::string, BoidSettings>& BoidManager::get_boid_types()
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

// Adds a new type of boid to the BoidManager.
void BoidManager::add_boid_type(const std::string& _str, BoidSettings _settings)
{
    auto entry = boid_types_.find(_str);
    if (entry != boid_types_.end())
        return;

    boid_types_[_str] = _settings;

    current_type_selection_ = &boid_types_.begin()->second;
}

// Removes all boids from the scene that are of the current type selection.
void BoidManager::delete_all_of_current_type_selection()
{
    auto type_str = current_type_selection_->type;

    for (auto& boid : boids_)
    {
        if (boid->settings_->type == type_str)
        {
            boid->set_alive(false);
            --num_boids_;
        }
    }

    boids_dirty_ = true;
}

void BoidManager::delete_all_boids()
{
    for (auto& boid : boids_)
    {
        boid->set_alive(false);
        --num_boids_;
    }

    boids_dirty_ = true;
}

// Delete all boids and clear all boid types ready for recompile.
void BoidManager::reset()
{
    delete_all_boids();
    boid_types_.clear();
}

// The rules that are available to boids.
void BoidManager::register_rules()
{
    rules_.emplace("separation", std::make_unique<Separation>());
    rules_.emplace("alignment", std::make_unique<Alignment>());
    rules_.emplace("cohesion", std::make_unique<Cohesion>());
}

/* The tag functions that are available to boids.
 *
 * Tag functions can assume the following:
 * - lhs and rhs are different boids.
 * - lhs and rhs are both alive.
 * - lhs and rhs are of a different boid type.
 */
void BoidManager::register_tag_functions()
{
    // lhs converts rhs.
    tag_functions_["infect"] = [](Boid* lhs, Boid* rhs)
    {
        rhs->model_ = lhs->settings_->model;
        rhs->set_scale(lhs->get_scale());
        rhs->settings_ = lhs->settings_;
    };

    // lhs kills rhs.
    tag_functions_["kill"] = [this](Boid* lhs, Boid* rhs)
    {
        rhs->set_alive(false);

        boids_dirty_ = true;
        --num_boids_;
    };

    // lhs increases the scale of rhs (up to a threshold).
    tag_functions_["grow"] = [](Boid* lhs, Boid* rhs)
    {
        if (rhs->get_scale().LengthSquared() > 0.1f)
            return;

        rhs->set_scale(rhs->get_scale() * 1.01f);
    };
}

// Sets the spawn selection to the boid type with the id equal to the editable_spawn_id.
void BoidManager::update_spawn_selection()
{
    if (!current_type_selection_)
        return;

    // Optimisation barrier to prevent unecessary find operations.
    if (current_type_selection_->type_id != editable_spawn_id_)
    {
        auto it = std::find_if(boid_types_.begin(), boid_types_.end(), [this](const auto& _elem)
        {
            return _elem.second.type_id == editable_spawn_id_;
        });

        current_type_selection_ = &it->second;
    }
}

void BoidManager::spawn_controls(GameData* _GD)
{
    if (_GD->input_handler->get_key(DIK_V))
        add_boid(current_type_selection_->type, _GD->boid_spawn_pos);
}

// Adds a new boid of the passed type at the passed position.
void BoidManager::add_boid(const std::string& _type, Vector3 _pos)
{
    if (num_boids_ >= MAX_BOIDS)
        return;

    ++num_boids_;

    auto boid = std::make_unique<Boid>(&boid_types_.at(_type));
    boid->set_pos(_pos);

    boids_.push_back(std::move(boid));
}

// Erase any boids from the vector that are no longer alive.
void BoidManager::garbage_collect_boids()
{
    if (!boids_dirty_)
        return;

    boids_.erase(std::remove_if(
        boids_.begin(),
        boids_.end(),
        [](std::unique_ptr<Boid>& _boid) { return !_boid->is_alive(); }),
        boids_.end());

    boids_dirty_ = false;
}

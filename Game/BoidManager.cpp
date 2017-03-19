#include "BoidManager.h"
#include "Boid.h"
#include "BoidType.h"
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
    configure_boid_settings();
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

BoidSettings& BoidManager::get_human_settings()
{
    return human_settings_;
}

BoidSettings& BoidManager::get_zombie_settings()
{
    return zombie_settings_;
}

int* BoidManager::get_num_boids()
{
    return &num_boids_;
}

void BoidManager::register_rules()
{
    rules_[Rule::ID::Separation] = std::make_unique<Separation>();
    rules_[Rule::ID::Alignment] = std::make_unique<Alignment>();
    rules_[Rule::ID::Cohesion] = std::make_unique<Cohesion>();
}

void BoidManager::configure_boid_settings()
{
    human_settings_.type = BoidType::HUMAN;
    human_settings_.model = fetch_model(BoidType::HUMAN);

    zombie_settings_.type = BoidType::ZOMBIE;
    zombie_settings_.model = fetch_model(BoidType::ZOMBIE);
    zombie_settings_.max_speed = 5.0f;
    zombie_settings_.max_steer = 0.5f;

    human_settings_.weighted_rules.push_back({ rules_.at(Rule::ID::Separation).get(), 1.5f });
    human_settings_.weighted_rules.push_back({ rules_.at(Rule::ID::Alignment).get(), 0.8f });
    human_settings_.weighted_rules.push_back({ rules_.at(Rule::ID::Cohesion).get(), 0.8f });

    zombie_settings_.weighted_rules.push_back({ rules_.at(Rule::ID::Separation).get(), 1.5f });
    zombie_settings_.weighted_rules.push_back({ rules_.at(Rule::ID::Cohesion).get(), 0.8f });
}

void BoidManager::spawn_controls(GameData* _GD)
{
    if (_GD->input_handler->get_key(DIK_H))
        add_boid(BoidType::HUMAN, _GD->boid_spawn_pos);

    if (_GD->input_handler->get_key(DIK_Z))
        add_boid(BoidType::ZOMBIE, _GD->boid_spawn_pos);
}

void BoidManager::add_boid(BoidType _type, Vector3 _pos)
{
    if (num_boids_ >= max_boids_)
        return;

    ++num_boids_;

    auto boid = std::make_unique<Boid>(fetch_settings(_type));
    boid->set_pos(_pos);

    boids_.push_back(std::move(boid));
}

CMOModel* BoidManager::fetch_model(BoidType _type) const
{
    switch (_type)
    {
        case HUMAN: return cmo_manager_.get_model("human");
        case ZOMBIE: return cmo_manager_.get_model("zombie");

        default: throw std::runtime_error("Error in fetch_model");
    }
}

BoidSettings* BoidManager::fetch_settings(BoidType _type)
{
    switch (_type)
    {
        case HUMAN: return &human_settings_;
        case ZOMBIE: return &zombie_settings_;

        default: throw std::runtime_error("Error in fetch_settings");
    }
}

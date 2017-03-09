#include "BoidManager.h"
#include "Boid.h"
#include "BoidType.h"
#include "CMOManager.h"
#include "Rules.h"
#include "GameData.h"
#include "InputHandler.h"

BoidManager::BoidManager(CMOManager& _cmo_manager, unsigned int _initial_humans)
    : cmo_manager_(_cmo_manager)
    , num_boids_(0)
{
    // Register rules.
    rules_[Rule::ID::Separation] = std::make_unique<Separation>();
    rules_[Rule::ID::Alignment] = std::make_unique<Alignment>();
    rules_[Rule::ID::Cohesion] = std::make_unique<Cohesion>();

    human_settings_.model = fetch_model(BoidType::HUMAN);
    zombie_settings_.model = fetch_model(BoidType::ZOMBIE);

    human_settings_.rules.push_back(rules_[Rule::ID::Separation].get());
    human_settings_.rules.push_back(rules_[Rule::ID::Alignment].get());
    human_settings_.rules.push_back(rules_[Rule::ID::Cohesion].get());

    for (int i = 0; i < _initial_humans; ++i)
    {
        add_boid(BoidType::HUMAN, Vector3::Zero);
    }
}

void BoidManager::tick(GameData* _GD)
{
    if (_GD->input_handler->get_key(DIK_H))
        add_boid(BoidType::HUMAN, Vector3::Zero);

    if (_GD->input_handler->get_key(DIK_Z))
        add_boid(BoidType::ZOMBIE, Vector3::Zero);

    for (auto& boid : boid_data_.boids)
    {
        boid->tick(_GD);
    }
}

void BoidManager::draw(DrawData* _DD)
{
    for (auto& boid : boid_data_.boids)
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

void BoidManager::add_boid(BoidType _type, Vector3 _pos)
{
    if (num_boids_ >= 1000)
        return;

    ++num_boids_;

    auto boid = std::make_unique<Boid>(boid_data_, fetch_settings(_type));
    boid->set_pos(_pos);

    boid_data_.boids.push_back(std::move(boid));
}

CMOModel* BoidManager::fetch_model(BoidType _type) const
{
    switch (_type)
    {
        case HUMAN: return cmo_manager_.get_model("JEMINA vase -up");
        case ZOMBIE: return cmo_manager_.get_model("JEMINA vase -up");

        default: throw std::runtime_error("Error in fetch_model");
    }
}

BoidSettings& BoidManager::fetch_settings(BoidType _type)
{
    switch (_type)
    {
        case HUMAN: return human_settings_;
        case ZOMBIE: return zombie_settings_;

        default: throw std::runtime_error("Error in fetch_settings");
    }
}

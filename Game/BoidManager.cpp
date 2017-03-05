#include "BoidManager.h"
#include "Boid.h"
#include "CMOManager.h"
#include "Rules.h"

BoidManager::BoidManager(CMOManager& _cmo_manager, unsigned int _initial_boids)
    : cmo_manager_(_cmo_manager)
{
    // Register rules.
    rules_[Rule::ID::Separation] = std::make_unique<Separation>();
    rules_[Rule::ID::Alignment] = std::make_unique<Alignment>();
    rules_[Rule::ID::Cohesion] = std::make_unique<Cohesion>();

    for (int i = 0; i < _initial_boids; ++i)
    {
        add_boid(Vector3::Zero);
    }
}

void BoidManager::tick(GameData* _GD)
{
    for (auto& boid : boid_data_.boids)
    {
        boid->tick(_GD, boid_data_);
    }
}

void BoidManager::draw(DrawData* _DD)
{
    for (auto& boid : boid_data_.boids)
    {
        boid->draw(_DD);
    }
}

void BoidManager::add_boid(Vector3 _pos)
{
    auto boid = std::make_unique<Boid>(cmo_manager_.get_model("BirdModelV1"));
    boid->set_pos(_pos);

    boid->add_rule(rules_[Rule::ID::Separation].get());
    boid->add_rule(rules_[Rule::ID::Alignment].get());
    boid->add_rule(rules_[Rule::ID::Cohesion].get());

    boid_data_.boids.push_back(std::move(boid));
}

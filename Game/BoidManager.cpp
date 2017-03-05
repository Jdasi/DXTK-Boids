#include "BoidManager.h"
#include "Boid.h"

BoidManager::BoidManager(unsigned int _initial_boids)
{
    for (int i = 0; i < _initial_boids; ++i)
    {
        add_boid(Vector3::Zero);
    }
}

void BoidManager::add_boid(Vector3 _pos)
{
    auto boid = std::make_unique<Boid>();
    boid_data.boids_.push_back(std::move(boid));
}

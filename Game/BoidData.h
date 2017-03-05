#pragma once
#include <memory>
#include <vector>

class Boid;

struct BoidData
{
    std::vector<std::unique_ptr<Boid>> boids_;
};

#pragma once
#include "BoidType.h"

#include <vector>

class Rule;
class CMOModel;

// Common settings that are shared amongst all boids that use this data.
struct BoidSettings
{
    BoidType type = BoidType::DEFAULT;

    float max_speed = 0.1f;
    float max_steer = 0.005;

    float desired_separation = 5.0f;
    float neighbour_scan = 7.0f;

    std::vector<Rule*> rules;
    CMOModel* model = nullptr;
};

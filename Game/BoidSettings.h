#pragma once
#include "BoidType.h"

#include <vector>

class Rule;
class CMOModel;

// Common settings that are shared amongst all boids that use this data.
struct BoidSettings
{
    BoidType type = BoidType::DEFAULT;

    float max_speed = 15.0f;
    float max_steer = 1.5;

    float desired_separation = 5.0f;
    float neighbour_scan = 7.0f;

    std::vector<Rule*> rules;
    CMOModel* model = nullptr;
};

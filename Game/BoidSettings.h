#pragma once
#include "BoidType.h"
#include "WeightedRule.h"

#include <vector>

class Rule;
class CMOModel;

// Common settings that are shared amongst all boids that use this data.
struct BoidSettings
{
    BoidType type = BoidType::DEFAULT;

    float max_speed = 15.0f;
    float max_steer = 1.0f;

    float desired_separation = 5.0f;
    float neighbour_scan = 7.0f;
    float infection_distance = 2.5f;

    std::vector<WeightedRule> weighted_rules;
    CMOModel* model = nullptr;
};

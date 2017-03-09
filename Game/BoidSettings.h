#pragma once
#include "BoidType.h"

#include <vector>

class Rule;
class CMOModel;

struct BoidSettings
{
    BoidType type = BoidType::DEFAULT;

    float max_speed = 0.1f;
    float max_steer = 0.005f;

    float desired_separation = 5.0f;
    float neighbour_scan = 7.0f;
    float scan_modifier = 1.0f;

    std::vector<Rule*> rules;
    CMOModel* model = nullptr;
};

#pragma once
#include <vector>

class Rule;

struct BoidSettings
{
    float max_speed = 0.25f;
    float max_steer = 0.005f;

    float desired_separation = 5.0f;
    float neighbour_scan = 5.0f;
    float scan_modifier = 1.0f;

    std::vector<Rule*> rules;
};

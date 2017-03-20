#pragma once
#include "ParameterisedRule.h"

#include <vector>
#include <functional>

class Rule;
class CMOModel;
class Boid;

/* Common settings that are shared amongst all boids that use these BoidSettings.
 * Default values are listed below, but all are subject to change after reading in data.
 */
struct BoidSettings
{
    std::string type;
    int type_id = 0;
    CMOModel* model = nullptr;

    float max_speed = 15.0f;
    float max_steer = 1.0f;

    float desired_separation = 5.0f;
    float neighbour_scan = 7.0f;
    float tag_distance = 2.5f;

    std::vector<ParameterisedRule> parameterised_rules;
    std::vector<std::function<void(Boid*, Boid*)>> tag_functions;
};

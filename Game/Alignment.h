#pragma once
#include "Rule.h"

// Rule for steering a boid towards the average heading of its neighbours.
class Alignment : public Rule
{
public:
    Alignment() = default;
    ~Alignment() = default;

    Vector3 force(GameData* _GD, std::vector<Boid*>& _neighbours, ParameterisedRule* _rule_params) override;

private:


};

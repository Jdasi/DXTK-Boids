#pragma once
#include "Rule.h"

class Separation : public Rule
{
public:
    Separation() = default;
    ~Separation() = default;

    Vector3 force(GameData* _GD, std::vector<Boid*>& _neighbours, ParameterisedRule* _rule_params) override;

private:


};

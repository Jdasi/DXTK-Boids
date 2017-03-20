#pragma once
#include "Rule.h"

class Cohesion : public Rule
{
public:
    Cohesion() = default;
    ~Cohesion() = default;

    Vector3 force(GameData* _GD, std::vector<Boid*>& _neighbours, ParameterisedRule* _rule_params) override;

private:
    void process_tag_functions(Boid* _neighbour, float _distance) const;
    Vector3 seek(Vector3 _target, float _weight) const;

};

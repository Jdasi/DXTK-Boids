#pragma once
#include "Rule.h"

class Cohesion : public Rule
{
public:
    Cohesion() = default;
    ~Cohesion() = default;

    Vector3 force(GameData* _GD, std::vector<Boid*>& _neighbours, float _weight) override;

private:
    Vector3 seek(Vector3 _target, float _weight) const;

};

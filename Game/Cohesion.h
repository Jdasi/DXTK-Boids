#pragma once
#include "Rule.h"

class Cohesion : public Rule
{
public:
    Cohesion() = default;
    ~Cohesion() = default;

    Vector3 force(GameData* _GD, BoidData& _BD) override;

private:
    Vector3 seek(Vector3 _target) const;

};

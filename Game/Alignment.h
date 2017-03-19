#pragma once
#include "Rule.h"

class Alignment : public Rule
{
public:
    Alignment() = default;
    ~Alignment() = default;

    Vector3 force(GameData* _GD, std::vector<Boid*>& _neighbours, float _weight) override;

private:


};

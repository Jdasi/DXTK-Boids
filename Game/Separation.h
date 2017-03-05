#pragma once
#include "Rule.h"

class Separation : public Rule
{
public:
    Separation() = default;
    ~Separation() = default;

    Vector3 force(GameData* _GD, BoidData& _BD) override;

private:


};

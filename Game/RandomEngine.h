#pragma once
#include <random>

class RandomEngine
{
public:
    RandomEngine() = default;
    ~RandomEngine() = default;

    static float range(float _min, float _max);

private:
    static std::random_device rd_;
    static std::mt19937 mt_;

};

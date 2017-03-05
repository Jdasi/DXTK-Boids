#include "RandomEngine.h"

std::random_device RandomEngine::rd_;
std::mt19937 RandomEngine::mt_(rd_());

float RandomEngine::range(float _min, float _max)
{
    std::uniform_real_distribution<float> random(_min, _max);
    return random(mt_);
}

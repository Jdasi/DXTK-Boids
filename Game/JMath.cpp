#include "JMath.h"

// Clamps a float. I.e. returns _min when _min is exceeded.
float JMath::clampf(float _curr, float _min, float _max)
{
    if (_curr < _min)
        return _min;

    if (_curr > _max)
        return _max;

    return _curr;
}

// Inversely clamps a float. I.e. returns _min when _max is exceeded.
float JMath::iclampf(float _curr, float _min, float _max)
{
    if (_curr < _min)
        return _max;

    if (_curr > _max)
        return _min;

    return _curr;
}

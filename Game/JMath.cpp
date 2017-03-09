#include "JMath.h"

float JMath::clampf(float _curr, float _min, float _max)
{
    if (_curr < _min)
        return _min;

    if (_curr > _max)
        return _max;

    return _curr;
}

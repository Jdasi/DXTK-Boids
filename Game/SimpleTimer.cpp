#include "SimpleTimer.h"

SimpleTimer::SimpleTimer()
{
    reset();
}

// Sets the internal start point to now.
void SimpleTimer::reset()
{
    start = std::chrono::system_clock::now();
}

// Returns the time difference in seconds between the start point and now.
float SimpleTimer::getTimeDifference() const
{
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsed_seconds = now - start;

    return elapsed_seconds.count();
}



#include "Alignment.h"
#include "Boid.h"
#include "BoidData.h"
#include "GameData.h"

Vector3 Alignment::force(GameData* _GD, BoidData& _BD)
{
    Vector3 sum = Vector3::Zero;

    int count = 0;

    for (auto& boid : _BD.boids)
    {
        if (boid.get() == this_boid_)
            continue;

        float distance = Vector3::Distance(this_boid_->get_pos(), boid->get_pos());

        if (distance > 0 && distance < (this_boid_->settings().neighbour_scan * 
            this_boid_->settings().scan_modifier))
        {
            sum += boid->get_velocity();
            ++count;
        }
    }

    if (count > 0)
        this_boid_->settings().scan_modifier = 1.0f;
    else
        this_boid_->settings().scan_modifier += 10.0f * _GD->delta_time;

    if (count > 0)
    {
        sum /= static_cast<float>(count);

        sum.Normalize();
        sum *= this_boid_->settings().max_speed;

        Vector3 steer = sum - this_boid_->get_velocity();

        if (steer.Length() > this_boid_->settings().max_steer)
        {
            steer.Normalize();
            steer *= this_boid_->settings().max_steer;
        }

        steer *= 0.8f; // Temporary arbitrary weighting.
        return steer;
    }
    else
    {
        return Vector3::Zero;
    }
}

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

        if (distance > 0 && distance < (boid_settings_->neighbour_scan *
            this_boid_->get_scan_modifier()))
        {
            sum += boid->get_velocity();
            ++count;
        }
    }

    if (count > 0)
        this_boid_->set_scan_modifier(1.0f);
    else
        this_boid_->modify_scan_modifier(10.0f * _GD->delta_time);

    if (count > 0)
    {
        sum /= static_cast<float>(count);

        sum.Normalize();
        sum *= boid_settings_->max_speed;

        Vector3 steer = sum - this_boid_->get_velocity();

        if (steer.Length() > boid_settings_->max_steer)
        {
            steer.Normalize();
            steer *= boid_settings_->max_steer;
        }

        steer *= 0.8f; // Temporary arbitrary weighting.
        return steer;
    }
    else
    {
        return Vector3::Zero;
    }
}

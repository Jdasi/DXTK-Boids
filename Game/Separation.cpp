#include "Separation.h"
#include "Boid.h"
#include "BoidData.h"
#include "GameData.h"

Vector3 Separation::force(GameData* _GD, BoidData& _BD)
{
    Vector3 steer = Vector3::Zero;

    int count = 0;

    for (auto& boid : _BD.boids)
    {
        if (boid.get() == this_boid_)
            continue;

        float distance = Vector3::Distance(this_boid_->get_pos(), boid->get_pos());

        if (distance > 0 && distance < this_boid_->settings().desired_separation)
        {
            Vector3 difference = this_boid_->get_pos() - boid->get_pos();
            difference.Normalize();
            difference /= distance;

            steer += difference;
            ++count;
        }
    }

    if (count > 0)
    {
        steer /= static_cast<float>(count);
    }

    if (steer.Length() > 0)
    {
        steer.Normalize();
        steer *= this_boid_->settings().max_speed;
        steer -= this_boid_->get_velocity();

        if (steer.Length() > this_boid_->settings().max_steer)
        {
            steer.Normalize();
            steer *= this_boid_->settings().max_steer;
        }
    }

    steer *= 1.5f; // Temporary arbitrary weighting.
    return steer;
}

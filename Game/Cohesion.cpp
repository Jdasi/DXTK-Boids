#include "Cohesion.h"
#include "Boid.h"
#include "GameData.h"

Vector3 Cohesion::force(GameData* _GD, std::vector<Boid*>& _neighbours, float _weight)
{
    Vector3 sum = Vector3::Zero;

    int count = 0;

    for (auto& boid : _neighbours)
    {
        if (this_boid_ == boid)
            continue;

        /*
        // Zombies don't flock.
        if (this_boid_->getSettings()->type == BoidType::ZOMBIE &&
            boid->getSettings()->type == BoidType::ZOMBIE)
            continue;
        */

        float distance = Vector3::Distance(this_boid_->get_pos(), boid->get_pos());

        /*
        // Handle infection.
        if (this_boid_->getSettings()->type == BoidType::ZOMBIE &&
            boid->getSettings()->type == BoidType::HUMAN)
        {
            if (distance > 0 && distance <= this_boid_->getSettings()->infection_distance)
            {
                boid->infect(this_boid_->getSettings());
                continue;
            }
        }
        */

        if (distance > 0 && distance < (boid_settings_->neighbour_scan *
            this_boid_->get_scan_modifier()))
        {
            sum += boid->get_pos();
            ++count;
        }
    }

    if (count <= 0)
        this_boid_->modify_scan_modifier(5.0f * _GD->delta_time);

    if (count > 0)
    {
        sum /= static_cast<float>(count);
        return seek(sum, _weight);
    }
    else
    {
        return Vector3::Zero;
    }
}

Vector3 Cohesion::seek(Vector3 _target, float _weight) const
{
    Vector3 desired = _target - this_boid_->get_pos();

    desired.Normalize();
    desired *= boid_settings_->max_speed;

    Vector3 steer = desired - this_boid_->get_velocity();
    if (steer.Length() > boid_settings_->max_steer)
    {
        steer.Normalize();
        steer *= boid_settings_->max_steer;
    }

    steer *= _weight;
    return steer;
}

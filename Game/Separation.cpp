#include "Separation.h"
#include "Boid.h"
#include "GameData.h"

Vector3 Separation::force(GameData* _GD, std::vector<Boid*>& _neighbours)
{
    Vector3 steer = Vector3::Zero;

    int count = 0;

    for (auto& boid : _neighbours)
    {
        if (this_boid_ == boid)
            continue;

        // Zombies don't want separation from humans.
        if (this_boid_->getSettings()->type == BoidType::ZOMBIE &&
            boid->getSettings()->type == BoidType::HUMAN)
            continue;

        float distance = Vector3::Distance(this_boid_->get_pos(), boid->get_pos());
        float separation_modifier = 1.0f;

        // Humans want a larger separation from zombies.
        if (this_boid_->getSettings()->type == BoidType::HUMAN &&
            boid->getSettings()->type == BoidType::ZOMBIE)
            separation_modifier = 10.0f;

        if (distance > 0 && distance < boid_settings_->desired_separation * separation_modifier)
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
        this_boid_->set_scan_modifier(1.0f);
    }

    if (steer.Length() > 0)
    {
        steer.Normalize();
        steer *= boid_settings_->max_speed;
        steer -= this_boid_->get_velocity();

        if (steer.Length() > boid_settings_->max_steer)
        {
            steer.Normalize();
            steer *= boid_settings_->max_steer;
        }
    }

    steer *= 1.5f; // Temporary arbitrary weighting.
    return steer;
}

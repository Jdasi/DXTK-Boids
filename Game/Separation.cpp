#include "Separation.h"
#include "Boid.h"
#include "GameData.h"

Vector3 Separation::force(GameData* _GD, std::vector<Boid*>& _neighbours, ParameterisedRule* _rule_params)
{
    Vector3 steer = Vector3::Zero;

    int count = 0;

    for (auto& neighbour : _neighbours)
    {
        if (this_boid_ == neighbour)
            continue;

        if (!_rule_params->concerns_type(neighbour->getSettings()->type))
            continue;

        float distance = Vector3::Distance(this_boid_->get_pos(), neighbour->get_pos());

        if (distance > 0 && distance < boid_settings_->desired_separation)
        {
            Vector3 difference = this_boid_->get_pos() - neighbour->get_pos();
            difference.Normalize();
            difference /= distance;

            steer += difference;
            ++count;
        }
    }

    if (count > 0)
    {
        steer /= static_cast<float>(count);
        this_boid_->reset_scan_modifier();
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

    steer *= _rule_params->get_weight();
    return steer;
}

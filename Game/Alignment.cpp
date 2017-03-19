#include "Alignment.h"
#include "Boid.h"
#include "GameData.h"
#include "Constants.h"

Vector3 Alignment::force(GameData* _GD, std::vector<Boid*>& _neighbours, ParameterisedRule* _rule_params)
{
    Vector3 sum = Vector3::Zero;

    int count = 0;

    for (auto& neighbour : _neighbours)
    {
        // Ignore self.
        if (this_boid_ == neighbour)
            continue;

        // Don't go any further if neighbour's type is not listed in the rule's params.
        if (!_rule_params->concerns_type(neighbour->getSettings()->type))
            continue;

        float distance = Vector3::Distance(this_boid_->get_pos(), neighbour->get_pos());

        if (distance > 0 && distance < (boid_settings_->neighbour_scan *
            this_boid_->get_scan_modifier()))
        {
            sum += neighbour->get_velocity();
            ++count;
        }
    }

    if (count <= 0)
        this_boid_->modify_scan_modifier(NEIGHBOUR_SCAN_MOD_GROWTH * _GD->delta_time);
    else
        this_boid_->modify_scan_modifier(-(NEIGHBOUR_SCAN_MOD_GROWTH * _GD->delta_time));

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

        steer *= _rule_params->get_weight();
        return steer;
    }
    else
    {
        return Vector3::Zero;
    }
}

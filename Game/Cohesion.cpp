#include "Cohesion.h"
#include "Boid.h"
#include "GameData.h"
#include "Constants.h"

Vector3 Cohesion::force(GameData* _GD, std::vector<Boid*>& _neighbours, ParameterisedRule* _rule_params)
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

        process_tag_functions(neighbour, distance);

        if (distance > 0 && distance < (boid_settings_->neighbour_scan *
            this_boid_->get_scan_modifier()))
        {
            sum += neighbour->get_pos();
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
        return seek(sum, _rule_params->get_weight());
    }
    else
    {
        return Vector3::Zero;
    }
}

void Cohesion::process_tag_functions(Boid* _neighbour, float _distance) const
{
    // Only tag alive boids.
    if (!_neighbour->is_alive())
        return;

    // Only tag foreign types.
    if (this_boid_->getSettings()->type_id == _neighbour->getSettings()->type_id)
        return;

    // Perform tag functions.
    if (_distance > 0 && _distance <= this_boid_->getSettings()->tag_distance)
    {
        for (auto& tag_function : this_boid_->getSettings()->tag_functions)
        {
            tag_function(this_boid_, _neighbour);
        }
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

#include "Boid.h"
#include "RandomEngine.h"
#include "Rule.h"
#include "GameData.h"
#include "Constants.h"
#include "JMath.h"

Boid::Boid(BoidSettings* _settings)
    : CMOGO(_settings->model)
    , scan_modifier_(1.0f)
    , alive_(true)
    , settings_(_settings)
{
    neighbours_.reserve(MAX_BOIDS / 4);

    fudge_ = Matrix::CreateRotationY(XM_PI * 1.5f);
    set_scale(0.1f);

    float angle = RandomEngine::range(0, XM_PI * 2);
    velocity_ = Vector3(cos(angle), 0, sin(angle));
}

void Boid::tick(GameData* _GD)
{
    rules(_GD);
    wrap();
    move(_GD);
    update_world();
}

void Boid::draw(DrawData* _DD)
{
    CMOGO::draw(_DD);
}

float Boid::get_scan_modifier() const
{
    return scan_modifier_;
}

void Boid::reset_scan_modifier()
{
    scan_modifier_ = NEIGHBOUR_SCAN_MOD_MIN;
}

bool Boid::is_alive() const
{
    return alive_;
}

void Boid::set_alive(bool _value)
{
    alive_ = _value;
}

void Boid::modify_scan_modifier(float _f)
{
    scan_modifier_ += _f;
    scan_modifier_ = JMath::clampf(scan_modifier_, NEIGHBOUR_SCAN_MOD_MIN, NEIGHBOUR_SCAN_MOD_MAX);
}

BoidSettings* Boid::getSettings() const
{
    return settings_;
}

const Vector3& Boid::get_velocity() const
{
    return velocity_;
}

void Boid::rules(GameData* _GD)
{
    for (auto& parameterised_rule : settings_->parameterised_rules)
    {
        // Dead boids don't process rules.
        if (!alive_)
            break;

        parameterised_rule.get_rule()->set_boid(this);
        apply_force(parameterised_rule.get_rule()->force(_GD, neighbours_, &parameterised_rule));
    }
}

void Boid::wrap()
{
    if (pos_.x < -PLAYABLE_AREA ||
        pos_.x > PLAYABLE_AREA ||
        pos_.z < -PLAYABLE_AREA ||
        pos_.z > PLAYABLE_AREA)
    {
        Vector3 steer = Vector3::Zero - pos_;

        if (steer.Length() > settings_->max_steer)
        {
            steer.Normalize();
            steer *= settings_->max_steer;
        }

        steer *= 2.0f; // Steering away from the edge is important to the boid.
        apply_force(steer);
    }
}

void Boid::move(GameData* _GD)
{
    if (settings_->max_speed <= 0)
        return;

    velocity_ += acceleration_;

    // Limit speed.
    if (velocity_.Length() > settings_->max_speed)
    {
        velocity_.Normalize();
        velocity_ *= settings_->max_speed;
    }

    pos_ += velocity_ * _GD->delta_time;
    acceleration_ *= 0;
}

void Boid::update_world()
{
    Vector3 dir = velocity_;
    dir.Normalize();

    Matrix scaleMat = Matrix::CreateScale(scale_);
    Matrix rotTransMat = Matrix::CreateWorld(pos_, dir, Vector3::Up);

    set_world_matrix(fudge_ * scaleMat * rotTransMat);
}

void Boid::apply_force(Vector3 _force)
{
    acceleration_ += _force;
}

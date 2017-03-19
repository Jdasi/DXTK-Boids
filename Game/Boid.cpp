#include "Boid.h"
#include "RandomEngine.h"
#include "Rule.h"
#include "GameData.h"
#include "Constants.h"

Boid::Boid(BoidSettings* _settings)
    : CMOGO(_settings->model)
    , scan_modifier_(1.0f)
    , settings_(_settings)
{
    neighbours_.reserve(500);

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

void Boid::set_scan_modifier(float _f)
{
    scan_modifier_ = _f;
}

void Boid::modify_scan_modifier(float _f)
{
    scan_modifier_ += _f;
}

BoidSettings* Boid::getSettings() const
{
    return settings_;
}

void Boid::infect(BoidSettings* _settings)
{
    set_model(_settings->model);
    settings_ = _settings;
}

const Vector3& Boid::get_velocity() const
{
    return velocity_;
}

void Boid::rules(GameData* _GD)
{
    for (auto& weighted_rule : settings_->weighted_rules)
    {
        weighted_rule.rule->set_boid(this);
        apply_force(weighted_rule.rule->force(_GD, neighbours_, weighted_rule.weight));
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

        steer *= 2.0f; // Temporary arbitrary weighting.
        apply_force(steer);
    }
}

void Boid::move(GameData* _GD)
{
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
    auto dir = velocity_;
    dir.Normalize();

    Matrix scaleMat = Matrix::CreateScale(scale_);
    Matrix rotTransMat = Matrix::CreateWorld(pos_, dir, Vector3::Up);

    set_world_matrix(fudge_ * scaleMat * rotTransMat);
}

void Boid::apply_force(Vector3 _force)
{
    acceleration_ += _force;
}

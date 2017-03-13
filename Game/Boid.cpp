#include "Boid.h"
#include "RandomEngine.h"
#include "Rule.h"
#include "JMath.h"
#include "GameData.h"

Boid::Boid(BoidData& _BD, BoidSettings& _settings)
    : CMOGO(_settings.model)
    , scan_modifier_(1.0f)
    , BD_(_BD)
    , settings_(_settings)
{
    fudge_ = Matrix::CreateRotationY(XM_PI * 1.5f);
    set_scale(0.1f);

    float angle = RandomEngine::range(0, XM_PI * 2);
    velocity_ = Vector3(cos(angle), 0, sin(angle));
}

void Boid::tick(GameData* _GD)
{
    rules(_GD);
    move(_GD);
    wrap();

    auto dir = velocity_;
    dir.Normalize();

    Matrix scaleMat = Matrix::CreateScale(scale_);
    Matrix rotTransMat = Matrix::CreateWorld(pos_, dir, Vector3::Up);

    set_world_matrix(fudge_ * scaleMat * rotTransMat);
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

const BoidSettings& Boid::getSettings() const
{
    return settings_;
}

const Vector3& Boid::get_velocity() const
{
    return velocity_;
}

void Boid::rules(GameData* _GD)
{
    for (auto& rule : settings_.rules)
    {
        rule->set_boid(this, settings_);
        apply_force(rule->force(_GD, BD_));
    }
}

void Boid::move(GameData* _GD)
{
    velocity_ += acceleration_;

    // Limit speed.
    if (velocity_.Length() > settings_.max_speed)
    {
        velocity_.Normalize();
        velocity_ *= settings_.max_speed;
    }

    pos_ += velocity_ * _GD->delta_time;
    acceleration_ *= 0;
}

void Boid::wrap()
{
    pos_.x = JMath::iclampf(pos_.x, -200.0f, 200.0f);
    pos_.z = JMath::iclampf(pos_.z, -200.0f, 200.0f);
}

void Boid::apply_force(Vector3 _force)
{
    acceleration_ += _force;
}

#include "Boid.h"
#include "RandomEngine.h"
#include "Rule.h"

Boid::Boid(CMOModel* _model)
    : CMOGO(_model)
{
    float angle = RandomEngine::range(0, XM_PI * 2);
    velocity_ = Vector3(cos(angle), 0, sin(angle));
}

void Boid::tick(GameData* _GD, BoidData& _BD)
{
    rules(_GD, _BD);
    move();
    rotate();
    wrap();

    CMOGO::tick(_GD);
}

void Boid::draw(DrawData* _DD)
{
    CMOGO::draw(_DD);
}

void Boid::add_rule(Rule* _rule)
{
    settings_.rules.push_back(_rule);
}

const Vector3& Boid::get_velocity() const
{
    return velocity_;
}

BoidSettings& Boid::settings()
{
    return settings_;
}

void Boid::rules(GameData* _GD, BoidData& _BD)
{
    for (auto& rule : settings_.rules)
    {
        rule->set_boid(this);
        apply_force(rule->force(_GD, _BD));
    }
}

void Boid::move()
{
    velocity_ += acceleration_;

    // Limit speed.
    if (velocity_.Length() > settings_.max_speed)
    {
        velocity_.Normalize();
        velocity_ *= settings_.max_speed;
    }

    pos_ += velocity_;
    acceleration_ *= 0;
}

void Boid::rotate()
{
    // TODO
}

void Boid::wrap()
{
    // TODO
}

void Boid::apply_force(Vector3 _force)
{
    acceleration_ += _force;
}

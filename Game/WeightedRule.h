#pragma once

class Rule;

struct WeightedRule
{
    WeightedRule(Rule* _rule, float _weight)
        : rule(_rule)
        , weight(_weight)
    {
    }

    Rule* rule = nullptr;
    float weight = 0;
};

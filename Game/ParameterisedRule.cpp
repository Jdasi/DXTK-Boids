#include "ParameterisedRule.h"

ParameterisedRule::ParameterisedRule(Rule* _rule, float _weight, std::vector<std::string> _valid_types)
    : rule_(_rule)
    , weight_(_weight)
    , valid_types_(_valid_types)
{
}

Rule* ParameterisedRule::get_rule() const
{
    return rule_;
}

float ParameterisedRule::get_weight() const
{
    return weight_;
}

// Adds the passed type to the rule's valid types vector, if it is not already there.
void ParameterisedRule::add_valid_type(const std::string& _type)
{
    auto entry = std::find(valid_types_.begin(), valid_types_.end(), _type);
    if (entry != valid_types_.end())
        return;

    valid_types_.push_back(_type);
}

// Returns true if the passed type exists within the rule's valid types.
bool ParameterisedRule::concerns_type(const std::string& _type)
{
    auto entry = std::find(valid_types_.begin(), valid_types_.end(), _type);
    if (entry != valid_types_.end())
        return true;

    return false;
}

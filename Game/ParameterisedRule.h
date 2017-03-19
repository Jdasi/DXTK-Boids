#pragma once
#include <vector>

class Rule;

class ParameterisedRule
{
public:
    ParameterisedRule(Rule* _rule, float _weight, std::vector<std::string> _valid_types);
    ~ParameterisedRule() = default;

    Rule* get_rule() const;
    float get_weight() const;

    void add_valid_type(const std::string& _type);
    bool concerns_type(const std::string& _type);

private:
    Rule* rule_;
    float weight_;
    std::vector<std::string> valid_types_;

};

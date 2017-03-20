#pragma once
#include <vector>

class Rule;

/* A class for holding a pointer to a Rule as well as settings that apply
 * to the Rule. The rule's weighting and concerned types are stored here.
 */
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
    Rule* rule_; // Fetched from BoidManager, who owns the actual rules.
    float weight_; // How important this force is to the boid.
    std::vector<std::string> valid_types_; // The boid types that this rule concerns.

};

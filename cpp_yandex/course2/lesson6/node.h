#pragma once

#include "date.h"

#include <memory>

enum class Comparison {
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    NotEqual,
};

enum class LogicalOperation {
    Or,
    And,
};

class Node {
public:
    virtual bool Evaluate(const Date& date, const string& event) const = 0;
};

class EmptyNode : public Node {
public:
    bool Evaluate(const Date& date, const string& event) const override;
};

class DateComparisonNode : public Node {
public:
    DateComparisonNode(const Comparison& cmp, const Date& date):
            cmp_(cmp), date_(date) {}

    bool Evaluate(const Date& date, const string& event) const override;

private:
    const Date date_;
    const Comparison cmp_;
};

class EventComparisonNode : public Node {
public:
    EventComparisonNode(const Comparison& cmp, const string& event):
            cmp_(cmp), event_(event) {}

    bool Evaluate(const Date& date, const string& event) const override;

private:
    const string event_;
    const Comparison cmp_;
};

class LogicalOperationNode : public Node {
public:
    LogicalOperationNode(const LogicalOperation& logical_operation,
                         const shared_ptr<Node>& left,
                         const shared_ptr<Node>& right):
            logical_operation_(logical_operation), left_(left), right_(right) {}

    bool Evaluate(const Date& date, const string& event) const override;

private:
    const LogicalOperation logical_operation_;
    const shared_ptr<Node> left_;
    const shared_ptr<Node> right_;
};
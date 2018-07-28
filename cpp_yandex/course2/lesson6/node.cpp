#include "node.h"

bool EmptyNode::Evaluate(const Date &date, const string &event) const {
    return true;
}

bool DateComparisonNode::Evaluate(const Date &date, const string &event) const {
    if (cmp_ == Comparison::Less) {
        return date < date_;
    } else if (cmp_ == Comparison::LessOrEqual) {
        return date <= date_;
    } else if (cmp_ == Comparison::Greater) {
        return date > date_;
    } else if (cmp_ == Comparison::GreaterOrEqual) {
        return date >= date_;
    } else if (cmp_ == Comparison::Equal) {
        return date == date_;
    } else if (cmp_ == Comparison::NotEqual) {
        return date != date_;
    }
    return false;
}

bool EventComparisonNode::Evaluate(const Date &date,
                                   const string &event) const {
    if (cmp_ == Comparison::Less) {
        return event < event_;
    } else if (cmp_ == Comparison::LessOrEqual) {
        return event <= event_;
    } else if (cmp_ == Comparison::Greater) {
        return event > event_;
    } else if (cmp_ == Comparison::GreaterOrEqual) {
        return event >= event_;
    } else if (cmp_ == Comparison::Equal) {
        return event == event_;
    } else if (cmp_ == Comparison::NotEqual) {
        return event != event_;
    }
    return false;
}

bool LogicalOperationNode::Evaluate(const Date &date,
                                    const string &event) const {
    if (logical_operation_ == LogicalOperation::Or) {
        return left_->Evaluate(date, event) || right_->Evaluate(date, event);
    } else if (logical_operation_ == LogicalOperation::And) {
        return left_->Evaluate(date, event) && right_->Evaluate(date, event);
    }
    return false;
}
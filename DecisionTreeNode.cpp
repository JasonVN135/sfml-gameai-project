// DecisionTreeNode.cpp
#include "DecisionTreeNode.h"
#include <iostream>

// === Action Implementation ===
Action::Action(const std::string& name) : actionName(name) {}

std::shared_ptr<DecisionTreeNode> Action::makeDecision() {
    return shared_from_this();
}

std::string Action::getName() const {
    return actionName;
}

// === Decision Implementation ===
Decision::Decision(std::shared_ptr<DecisionTreeNode> tNode, std::shared_ptr<DecisionTreeNode> fNode)
    : trueNode(tNode), falseNode(fNode) {}

    std::shared_ptr<DecisionTreeNode> Decision::makeDecision() {
    std::shared_ptr<DecisionTreeNode> branch = getBranch();
    return branch->makeDecision();
}

// === BoolDecision Implementation ===
BoolDecision::BoolDecision(std::shared_ptr<DecisionTreeNode> tNode, std::shared_ptr<DecisionTreeNode> fNode, std::function<bool()> getter)
    : Decision(tNode, fNode), valueGetter(getter) {}

std::shared_ptr<DecisionTreeNode> BoolDecision::getBranch() {
    bool value = valueGetter();
    if (value)
        return trueNode;
    else
        return falseNode;
}


// === FloatDecision Implementation ===
FloatDecision::FloatDecision(std::shared_ptr<DecisionTreeNode> tNode, std::shared_ptr<DecisionTreeNode> fNode, std::function<float()> getter,
                             float minVal, float maxVal)
    : Decision(tNode, fNode), valueGetter(getter), minValue(minVal), maxValue(maxVal) {}

std::shared_ptr<DecisionTreeNode> FloatDecision::getBranch() {
    float value = valueGetter();
    if (value >= minValue && value <= maxValue)
        return trueNode;
    else
        return falseNode;
}


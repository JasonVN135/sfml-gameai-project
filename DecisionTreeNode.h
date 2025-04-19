#ifndef DECISION_TREE_NODE
#define DECISION_TREE_NODE

#include <functional>
#include <iostream>
#include <memory>
#include <string>

// The Abstract Decision Tree node, these nodes are either Actions or Decisions 
class DecisionTreeNode {
public:
    virtual ~DecisionTreeNode() noexcept = default;
    virtual std::shared_ptr<DecisionTreeNode> makeDecision() = 0;
};

// The makeDecision method will return an output
class Action : public DecisionTreeNode, public std::enable_shared_from_this<Action> {
public:
    Action(const std::string& name);
    std::shared_ptr<DecisionTreeNode> makeDecision() override;
    std::string getName() const;

private:
    std::string actionName;
};

// Binary decision node, the makeDecision will not return an action,
// but try to make another decision based on some input
class Decision : public DecisionTreeNode {
public:
    Decision(std::shared_ptr<DecisionTreeNode> tNode, std::shared_ptr<DecisionTreeNode> fNode);
    virtual std::shared_ptr<DecisionTreeNode> getBranch() = 0;
    std::shared_ptr<DecisionTreeNode> makeDecision() override;
    virtual ~Decision() noexcept = default;

protected:
    std::shared_ptr<DecisionTreeNode> trueNode;
    std::shared_ptr<DecisionTreeNode> falseNode;
};

// Boolean-based decision node
class BoolDecision : public Decision {
public:
    BoolDecision(std::shared_ptr<DecisionTreeNode> tNode, std::shared_ptr<DecisionTreeNode> fNode, std::function<bool()> getter);
    std::shared_ptr<DecisionTreeNode> getBranch() override;
    ~BoolDecision() noexcept override = default;

private:
    std::function<bool()> valueGetter;
};

// Range-based float decision node
class FloatDecision : public Decision {
public:
    FloatDecision(std::shared_ptr<DecisionTreeNode> tNode, std::shared_ptr<DecisionTreeNode> fNode, std::function<float()> getter, float min, float max);
    std::shared_ptr<DecisionTreeNode> getBranch() override;
    ~FloatDecision() noexcept override = default;

private:
    std::function<float()> valueGetter;
    float minValue;
    float maxValue;
};

#endif // DECISION_TREE_NODE
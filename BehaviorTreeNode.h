#ifndef BEHAVIOR_TREE_NODE_H
#define BEHAVIOR_TREE_NODE_H

#include <vector>
#include <memory>
#include <functional>
#include <iostream>

/**
 * Success: Action or Condition is satisfied
 * Running: Action is currently running
 * Failure: Action or Condition is not satisfiable
 */
enum class BehaviorStatus {
    Success,
    Failure,
    Running
};

// Base class for the Behavior Tree Node
class BehaviorTreeNode {
public:
    /**
     * Base constructor
     */
    virtual ~BehaviorTreeNode() = default;

    /**
     * Base function to update the tree
     */
    virtual BehaviorStatus tick() = 0;
};

/**
 * Action node, performs an action, returns status of the action
 */
class ActionNode : public BehaviorTreeNode {
public:

    using ActionFunction = std::function<BehaviorStatus()>;

    /**
     * Constructor with the function to perform to yield a result
     */
    ActionNode(ActionFunction action);

    /**
     * Performs the action
     */
    BehaviorStatus tick() override;

private:
    /** The function to perform */
    ActionFunction actionFunc;
};

/**
 * The base composite node for nodes (Selector, Sequence, Parallel)
 */
class CompositeNode : public BehaviorTreeNode {
public:
    /**
     * Add a child BehaviorTreeNode to the CompositeNode
     */
    void addChild(std::shared_ptr<BehaviorTreeNode> child);

protected:
    /** the CompositeNode's children BehaviorNodes */
    std::vector<std::shared_ptr<BehaviorTreeNode>> children;
};

/**
 * Composite node that performs a sequence of Action nodes
 * If any Action fails, the Sequence returns Failure and the parent node can continue
 * If any Action is running, the sequence stops, but the parent node also stops moving to other children
 */
class Sequence : public CompositeNode {
public:

    /**
     * Sequence constructor
     * 
     * @param children the Children Nodes
     */
    Sequence(const std::vector<std::shared_ptr<BehaviorTreeNode>>& children);

    /**
     * Perform action
     */
    BehaviorStatus tick() override;

    /**
     * Reset the index of the Sequence
     */
    void reset();

private:
    /** Current index the sequence is looking at */
    size_t currentIndex = 0;
};

/**
 * Composite node for selecting a child node. 
 * Runs children in order until a child returns Success or Running.
 */
class Selector : public CompositeNode {
public:
    /**
     * Selector constructor
     * 
     * @param children the Children Nodes
     */
    Selector(const std::vector<std::shared_ptr<BehaviorTreeNode>>& children);

    /**
     * Perform action
     */
    BehaviorStatus tick() override;

    void reset();

private:
    /** Current index the selector is looking at */
    size_t currentIndex = 0;
};

/**
 * Composite node for running multiple children.
 */
class Parallel : public CompositeNode {
public:
    enum class Policy {
        RequireOne,   // Succeeds if one succeeds
        RequireAll    // Succeeds if all succeed
    };

    Parallel(Policy successPolicy, Policy failurePolicy, 
        const std::vector<std::shared_ptr<BehaviorTreeNode>>& children);

    BehaviorStatus tick() override;

private:
    Policy successPolicy;
    Policy failurePolicy;
};

#endif // BEHAVIOR_TREE_NODE_H
#include "BehaviorTreeNode.h"

ActionNode::ActionNode(ActionFunction action) : actionFunc(action) {}

BehaviorStatus ActionNode::tick() {
    return actionFunc();
}

void CompositeNode::addChild(std::shared_ptr<BehaviorTreeNode> child) {
    children.push_back(child);
}

Sequence::Sequence(const std::vector<std::shared_ptr<BehaviorTreeNode>>& children) {
    this->children = children;
}

BehaviorStatus Sequence::tick() {
    
    if (currentIndex < children.size()) {
        BehaviorStatus status = children[currentIndex]->tick();

        if (status == BehaviorStatus::Failure) {
            reset();
            return BehaviorStatus::Failure;
        }
        else if (status == BehaviorStatus::Running) {
            return BehaviorStatus::Running;
        }

        // If action succeeded, move to the next child
        ++currentIndex;
    }

    // If all actions are successful, return Success
    if (currentIndex == children.size()) {
        reset();
        return BehaviorStatus::Success;
    } else {
        return BehaviorStatus::Running;
    }
}

void Sequence::reset() {
    currentIndex = 0;
}

/**
 * Selector Constructor
 * Initializes with a list of children.
 */
Selector::Selector(const std::vector<std::shared_ptr<BehaviorTreeNode>>& children) {
    this->children = children;
}


BehaviorStatus Selector::tick() {
    if (currentIndex == children.size()) {
        reset();
    }
    if (currentIndex < children.size()) {
        BehaviorStatus status = children[currentIndex]->tick();

        if (status == BehaviorStatus::Success) {
            reset();
            return BehaviorStatus::Success;
        }
        else if (status == BehaviorStatus::Running) {
            return BehaviorStatus::Running;
        }

        // If the current child fails, move to the next one
        ++currentIndex;
    }

    // If all children failed, return Failure
    
    return BehaviorStatus::Failure;
}

void Selector::reset() {
    currentIndex = 0;
}

/**
 * Parallel Constructor
 * Initializes the success and failure policies and the list of children.
 */
Parallel::Parallel(Policy successPolicy, Policy failurePolicy, 
                   const std::vector<std::shared_ptr<BehaviorTreeNode>>& children)
    : successPolicy(successPolicy), failurePolicy(failurePolicy) {
    this->children = children;
}

/**
 * Ticks the Parallel node.
 * It evaluates all child nodes in parallel. If any child succeeds (based on the policy), 
 * the parent succeeds or fails accordingly.
 */
BehaviorStatus Parallel::tick() {
    bool runningOccurred = false;
    int successCount = 0;
    int failureCount = 0;

    for (auto& child : children) {
        BehaviorStatus status = child->tick();

        if (status == BehaviorStatus::Success) {
            ++successCount;
        } else if (status == BehaviorStatus::Failure) {
            ++failureCount;
        } else if (status == BehaviorStatus::Running) {
            runningOccurred = true;
        }

        // Early return: failure policy
        if (failurePolicy == Policy::RequireOne && status == BehaviorStatus::Failure) {
            return BehaviorStatus::Failure;
        }

        // Early return: success policy
        if (successPolicy == Policy::RequireOne && status == BehaviorStatus::Success) {
            return BehaviorStatus::Success;
        }
    }

    // If RequireAll for success and not all children succeeded
    if (successPolicy == Policy::RequireAll && successCount != (int) children.size()) {
        if (runningOccurred) return BehaviorStatus::Running;
        return BehaviorStatus::Failure;
    }

    // If RequireAll for failure and not all children failed
    if (failurePolicy == Policy::RequireAll && failureCount != (int) children.size()) {
        if (runningOccurred) return BehaviorStatus::Running;
        return BehaviorStatus::Success;
    }

    // If all succeeded or failed based on policies
    if (successPolicy == Policy::RequireAll && successCount == (int) children.size()) {
        return BehaviorStatus::Success;
    }

    if (failurePolicy == Policy::RequireAll && failureCount == (int) children.size()) {
        return BehaviorStatus::Failure;
    }

    // Default: if anyone is running, we're still running
    if (runningOccurred) {
        return BehaviorStatus::Running;
    }

    // Fallback: no one succeeded or failed by policy
    return BehaviorStatus::Failure;
}
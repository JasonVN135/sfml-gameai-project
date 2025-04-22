#ifndef DECISION_TREE_LEARNER_H
#define DECISION_TREE_LEARNER_H

#include <cmath>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <set>
#include <unordered_map>
#include "DecisionTreeNode.h"

/**
 * Struct to stor the data to construct the decision tree learner
 */
struct Entry {
    std::map<std::string, int> attributes; // conditions (0 or 1)
    std::string action; // class label (drink, chase, wander, etc.)
};

class DecisionTreeLearner {
public:

    DecisionTreeLearner(const std::map<std::string, std::function<bool()>>& getterMap);

    /**
     * Learn from the provided inputs
     */
    std::shared_ptr<DecisionTreeNode> learn(const std::vector<Entry>& entries, const std::set<std::string>& attributes);

private:

    std::map<std::string, std::function<bool()>> attributeGetterMap;

    /**
     * Constructs the leaf node based on the most frequent action
     */
    std::string mostCommonAction(const std::vector<Entry>& entries);

    /**
     * Calculates the entropy of the dataset 
     */
    double entropy(const std::vector<Entry>& entries);

    /**
     * Calculates the amount of information gained based on the attribute that was found
     */
    double informationGain(const std::vector<Entry>& entries, const std::string& attribute);

    /**
     * Split the dataset in groups of entries
     */
    std::map<int, std::vector<Entry>> splitByAttribute(const std::vector<Entry>& entries, const std::string& attribute);

    /**
     * Recursively construct the tree
     */
    std::shared_ptr<DecisionTreeNode> buildTree(const std::vector<Entry>& entries, const std::set<std::string>& action);

    /**
     * Get the number of times an action has occurred
     */
    int actionOccurrences(const std::vector<Entry>& entries, const std::string& attribute);
};

#endif // DECISION_TREE_LEARNER_H
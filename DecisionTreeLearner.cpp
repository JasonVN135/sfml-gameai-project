#include "DecisionTreeLearner.h"


DecisionTreeLearner::DecisionTreeLearner(const std::map<std::string, std::function<bool()>>& getterMap)
        : attributeGetterMap(getterMap) {}

std::shared_ptr<DecisionTreeNode> DecisionTreeLearner::learn(const std::vector<Entry>& entries, const std::set<std::string>& attributes) {

    return buildTree(entries, attributes);
}


/**
 * Constructs the leaf node based on the most frequent action
 */
std::string DecisionTreeLearner::mostCommonAction(const std::vector<Entry>& entries) {

    // Count the number of times each action occurs
    std::map<std::string, int> actionCounts;
    for (const auto entry : entries) {
        actionCounts[entry.action]++;
    }

    std::string highestAction;
    int highestCount = 0;
    // Find the action with the most occurrences
    for (const auto& [action, count] : actionCounts) {
        if (count > highestCount) {
            highestAction = action;
            highestCount = count;
        }
    }

    return highestAction;

}

/**
 * Calculates the entropy of the dataset 
 */
double DecisionTreeLearner::entropy(const std::vector<Entry>& entries) {
    // Store the number of occurences for each action
    std::unordered_map<std::string, int> actionCounts;

    // Get the counts for each action
    for (const auto entry : entries) {
        actionCounts[entry.action]++;
    }

    double entropy = 0.0f;
    int total = entries.size();

    // Calculate the Entropy
    for (const auto& [action, count] : actionCounts) {
        double p = static_cast<double> (count) / total;
        entropy -= p * std::log2(p);
    }
    
    return entropy;
}

/**
 * Calculates the amount of information gained based on the attribute that was found
 */
double DecisionTreeLearner::informationGain(const std::vector<Entry>& entries, const std::string& attribute) {

    // Get the entropy for the whole dataset
    double dataEntropy = entropy(entries);

    // Get the entries split by the attribute
    std::map<int, std::vector<Entry>> splitEntries = splitByAttribute(entries, attribute);

    // Get weighted average of the entry for each split
    double weightedEntropy = 0.0f;
    double total = entries.size();

    for (const auto& [value, subset] : splitEntries) {
        // The individual weight for the set of entries
        double subsetWeight = static_cast<double>(subset.size()) / total;
        weightedEntropy += subsetWeight * entropy(subset);
    }

    // Based on WeightedEntropy, calculate IG
    return dataEntropy - weightedEntropy;

}

/**
 * Split the dataset in groups of entries
 */
std::map<int, std::vector<Entry>> DecisionTreeLearner::splitByAttribute(const std::vector<Entry>& entries, const std::string& attribute) {

    // Create a map for possible attribute values and the list of entries based on the attribute
    std::map<int, std::vector<Entry>> splitEntries;

    // Split the entries based on the attribute
    for (const auto& entry : entries) {
        int attributeVal = entry.attributes.at(attribute);
        splitEntries[attributeVal].push_back(entry);
    }

    // Return the data
    return splitEntries;
}

/**
 * Recursively construct the tree based on the most common attribute
 */
std::shared_ptr<DecisionTreeNode> DecisionTreeLearner::buildTree(const std::vector<Entry>& entries, const std::set<std::string>& attributes) {

    // If the entries list is empty, return null pointer
    if (entries.empty()) {
        return nullptr;
    }

    // If the highest Action occurs at every entry, construct and return an Action Node of that name 
    std::string highestAction = mostCommonAction(entries);
    if (actionOccurrences(entries, highestAction) == (int) entries.size()) {
        return std::make_shared<Action>(highestAction);
    }

    // If there are no more attributes to use to split, return the highest occurring action
    if (attributes.empty()) {
        return std::make_shared<Action>(highestAction);
    }

    // Recursively call based ont he action that generates the highest information gained
    double bestGain = 1.0f;
    std::string highestAttribute;
    for (const auto& attribute : attributes) {
        double gain = informationGain(entries, attribute);
        if (gain > bestGain) {
            bestGain = gain;
            highestAttribute = attribute;
        }
    }

    // Split the data based on the best attribute for highest results
    std::map<int, std::vector<Entry>> splitEntries = splitByAttribute(entries, highestAttribute);
    const auto& trueSubset = splitEntries[1];
    const auto& falseSubset = splitEntries[0];
    
    // Remove the attribute from the remain attribute for further recursive calls
    std::set<std::string> remainingAttributes = attributes;
    remainingAttributes.erase(highestAttribute);

    // Recursively build the true and false branches
    auto trueBranch = buildTree(trueSubset, remainingAttributes);
    auto falseBranch = buildTree(falseSubset, remainingAttributes);

    // Build a BoolDecision using the attribute's value getter
    if (attributeGetterMap.find(highestAttribute) == attributeGetterMap.end()) {
        throw std::runtime_error("Missing attribute getter for: " + highestAttribute);
    }

    return std::make_shared<BoolDecision>(
        trueBranch,
        falseBranch,
        // Use the function from the map for testing
        attributeGetterMap.at(highestAttribute)
    );

}

int DecisionTreeLearner::actionOccurrences(const std::vector<Entry>& entries, const std::string& action) {
    int count = 0;

    for (const auto entry : entries) {
        if (entry.action == action) {
            count++;
        }
    }

    return count;   
}
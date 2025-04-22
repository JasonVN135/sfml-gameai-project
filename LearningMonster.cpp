#include "LearningMonster.h"
#include "SteeringBehavior.h"

LearningMonster::LearningMonster(const int id, const std::string& textureFile, const sf::Vector2f& startPos, const float vision, const std::string dataPath) 
: visionCircle(vision, (int) vision), visionDist(vision) {


    if (!texture.loadFromFile(textureFile)) {
        throw std::runtime_error("Failed to load text: " + textureFile);
    }

    visionCircle.setFillColor(sf::Color(0, 0, 0, 80));
    visionCircle.setOrigin(vision, vision);
    
    sprite.setTexture(texture);
    sprite.setOrigin(5, 3.5);

    // Initialize basic stats
    kinematic.id = id;

    kinematic.position = startPos;
    kinematic.velocity = sf::Vector2f(0, 0);

    kinematic.orientation = 0.0;
    kinematic.rotation = 0.0;

    kinematic.maxSpeed = 20;
    kinematic.maxRotation = M_PI / 4;

    sprite.setPosition(kinematic.position);

    thirst = 60.0;
    
    initializeAttributeGetters();
    constructDecisionTree(dataPath);



    currentAction = "";


}

LearningMonster::~LearningMonster() {
}

void LearningMonster::update(float deltaTime) {

    if (currentAction == "drinkWater") {
        sprite.setColor(sf::Color::Red);
    } else {
        sprite.setColor(sf::Color::Green);
    }

    auto decision = decisionTree->makeDecision();
    auto action = std::dynamic_pointer_cast<Action>(decision);

    if (action) {
        std::string actionName = action->getName();
        if (actionName == "wander" && currentAction != "wander") wander();
        else if (actionName == "chasePlayer" && currentAction != "chasePlayer") chasePlayer();
        else if (actionName == "attackTarget" && currentAction != "attackTarget") attackTarget();
        else if (actionName == "pathToWater" && currentAction != "pathToWater") pathToWater();
        else if (actionName == "drinkWater") drinkWater();

        currentAction = actionName;
    }

    

    targetKinematic = kinematic;

    if (target != nullptr) {
        targetPos = target->getKinematic().position;
        //printf("%f\n", breadcrumbs.at(0).getKinematic().position.y);
    }
    targetKinematic.position = targetPos;
    
    SteeringOutput total;
    
    for (auto& behavior : behaviors) {
        SteeringOutput output = behavior->update(kinematic, targetKinematic);
        total.linear += output.linear;
        total.angular += output.angular;
    }

    kinematic.position += kinematic.velocity * deltaTime;
    kinematic.orientation += kinematic.rotation * deltaTime;;

    kinematic.velocity += total.linear * deltaTime;
    kinematic.rotation += total.angular * deltaTime;

    // Max velocity if it tires to go over
    if (VectorUtils::vector2Length(kinematic.velocity) > kinematic.maxSpeed) {
        kinematic.velocity = VectorUtils::normalize(kinematic.velocity) * kinematic.maxSpeed;
    }

    // Max rotation if it tries to go over
    if (std::fabs(kinematic.rotation) > kinematic.maxRotation) {
        kinematic.rotation = (int) std::copysign(1.0f, kinematic.rotation) * kinematic.maxRotation;
    }

    setPosition(kinematic.position);
    setOrientation(kinematic.orientation);
    setKinematic(kinematic);

    visionCircle.setPosition(kinematic.position);

    std::cout << currentAction << std::endl;

    thirst -= deltaTime;
}
void LearningMonster::render(sf::RenderWindow& window) {
    window.draw(visionCircle);
    window.draw(sprite);
} 

void LearningMonster::setPosition(const sf::Vector2f &newPosition) {
    kinematic.position = newPosition;
    sprite.setPosition(kinematic.position);
}


void LearningMonster::setVelocity(const sf::Vector2f &newVelocity) {
    kinematic.velocity = newVelocity;
}


void LearningMonster::setOrientation(const float newOrientation) {
    kinematic.orientation = newOrientation;
    // Ensure the angle is in the range 0 to 2p
    kinematic.orientation = VectorUtils::mapToPiRange(kinematic.orientation);

    // Set rotation of sprite in degrees
    float rot_degrees = VectorUtils::radiansToDegrees(kinematic.orientation);
    sprite.setRotation(rot_degrees);
}


void LearningMonster::setRotation(const float newRotation) {
    kinematic.rotation = newRotation;
}


void LearningMonster::setKinematic(Kinematic &kin) {
    kinematic = kin;
}

void LearningMonster::setTargetKinematic(Kinematic &kin) {
    targetKinematic = kin;
}

Kinematic LearningMonster::getKinematic() {
    return kinematic;
}

void LearningMonster::addSteeringBehavior(std::unique_ptr<SteeringBehavior> behavior) {
    behaviors.push_back(std::move(behavior)); // Move into the vector
}

void LearningMonster::clearSteeringBehaviors() {
    behaviors.clear();
}


float LearningMonster::getThirst() {
    return thirst;
}


void LearningMonster::wander() {
    clearSteeringBehaviors();
    addSteeringBehavior(std::make_unique<Wander>(50, 15, 0.01, 0, 4, M_PI / 16, 0.1, M_PI / 32, M_PI / 4));
}

void LearningMonster::chasePlayer() {
    clearSteeringBehaviors();
    for (auto entity : Game::getInstance().getEntities()) {
        // If a target was found
        if (VectorUtils::vector2Length(entity->getKinematic().position - kinematic.position) < visionDist) {
            target = entity;
        }
    } 
    addSteeringBehavior(std::make_unique<Arrive>(15, 0.1, 10, 40));
    addSteeringBehavior(std::make_unique<Align>(M_PI / 6, 0.1, M_PI / 32, M_PI / 8));
}

void LearningMonster::attackTarget() {
    // Reset their positions
    target->setPosition(sf::Vector2f(300, 300));
    setPosition(sf::Vector2f(800, 700));

}

void LearningMonster::pathToWater() {
    clearSteeringBehaviors();
    Breadcrumb* water = Game::getInstance().getNearestWaterBreadcrumb(kinematic.position);
    targetPos = water->getPosition();
    addSteeringBehavior(std::make_unique<Arrive>(15, 0.1, 10, 30));
    addSteeringBehavior(std::make_unique<Align>(M_PI / 6, 0.1, M_PI / 32, M_PI / 8));
}

void LearningMonster::drinkWater() {
    thirst += 5;
}

bool LearningMonster::canSeeWater() {
    std::cout << "Check See Water";
    // Get closest water
    Breadcrumb* water = Game::getInstance().getNearestWaterBreadcrumb(kinematic.position);
    if (VectorUtils::vector2Length(water->getKinematic().position - kinematic.position) < visionDist) {
        targetPos = water->getKinematic().position;
        std::cout << " True" << std::endl;
        return true;
    }
    std::cout << " False" << std::endl;
    return false;
}
bool LearningMonster::isThirsty() {
    std::cout << "Check Is Thirsty";
    if (thirst < 40) {
        std::cout << " True" << std::endl;
        return true;
    }
    std::cout << " False" << std::endl;
    return false;
}
bool LearningMonster::canSeePlayer() {
    std::cout << "Check See Player";
    // If the Monster already had a target
    if (target != nullptr) {
        if (VectorUtils::vector2Length(targetPos - kinematic.position) < visionDist) {
            std::cout << " True" << std::endl;
            return true;
        }
        else {
            target = nullptr;
            std::cout << " False" << std::endl;
            return false;
        }
    }

    // If the Monster didn't have a target already, check to see if it can find one
    std::vector<Entity*> entities =  Game::getInstance().getEntities();
    for (auto entity : entities) {
        // If a target was found
        if (VectorUtils::vector2Length(entity->getKinematic().position - kinematic.position) < visionDist) {
            target = entity;
            std::cout << " True" << std::endl;
            return true;
        }
    }
    std::cout << " False" << std::endl;
    return false;
}
bool LearningMonster::isAtTarget() {
    std::cout << "Check At Target";
    if (VectorUtils::vector2Length(targetPos - kinematic.position) < 15) {
        std::cout << " True" << std::endl;
        return true;
    }
    std::cout << " False" << std::endl;
    return false;
}
bool LearningMonster::isGettingWater() {
    std::cout << "Check Is Drinking";
    if (currentAction == "drinkWater") {
        std::cout << " True" << std::endl;
        return true;
    }
    std::cout << " False" << std::endl;
    return false;
}

const std::map<std::string, std::function<bool()>>& LearningMonster::getAttributeGetterMap() const {
    return attributeGetterMap;
}


void LearningMonster::initializeAttributeGetters() {
    attributeGetterMap["canSeeWater"] = [this]() { return canSeeWater(); };
    attributeGetterMap["isThirsty"] = [this]() { return isThirsty(); };
    attributeGetterMap["canSeePlayer"] = [this]() { return canSeePlayer(); };
    attributeGetterMap["isAtTarget"] = [this]() { return isAtTarget(); };
    attributeGetterMap["isGettingWater"] = [this]() { return isGettingWater(); };
}

void LearningMonster::constructDecisionTree(std::string dataPath) {

    // Read the csv file
    std::ifstream file(dataPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open CSV file." << std::endl;
        return;
    }

    std::string line;
    std::vector<std::string> headers;

    // Read header attributes and action
    if (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string header;
        while (std::getline(ss, header, ',')) {
            headers.push_back(header);
        }
    }

    if (headers.empty()) {
        std::cerr << "No headers found in CSV file." << std::endl;
        return;
    }

    // The last header is the action
    std::string actionColumn = headers.back();
    // attributes name from 0 to n-1
    std::set<std::string> attributes(headers.begin(), headers.end() - 1);

    // Create a list of entires
    std::vector<Entry> entries;

    // Read data rows
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        Entry entry;
        int colIndex = 0;

        

        while (std::getline(ss, cell, ',')) {
            const std::string& columnName = headers[colIndex];
            if (columnName == actionColumn) {
                entry.action = cell;
            } else {
                // Convert "0" or "1" to int and store as string
                entry.attributes[columnName] = std::stoi(cell);
            }

            ++colIndex;
        }

        // Add the entry
        entries.push_back(entry);
    }

    file.close();

    // Now learn from data
    DecisionTreeLearner learner(attributeGetterMap);  // Assume you've set this earlier
    decisionTree = learner.learn(entries, attributes);

}
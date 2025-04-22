#include "Monster.h"
#include "SteeringBehavior.h"

Monster::Monster(const int id, const std::string& textureFile, const sf::Vector2f& startPos, const float vision) 
: visionCircle(vision, (int) vision), visionDist(vision), isWandering(false), isChasing(false), isGettingWater(false), isHeaderWritten(false) {

    if (!texture.loadFromFile(textureFile)) {
        throw std::runtime_error("Failed to load text: " + textureFile);
    }

    visionCircle.setFillColor(sf::Color(0, 0, 0, 80));
    visionCircle.setOrigin(vision, vision);
    
    sprite.setTexture(texture);
    sprite.setOrigin(10, 7);
    sprite.setColor(sf::Color::Green);
    
    // Initialize basic stats
    kinematic.id = id;

    kinematic.position = startPos;
    kinematic.velocity = sf::Vector2f(0, 0);

    kinematic.orientation = 0.0;
    kinematic.rotation = 0.0;

    kinematic.maxSpeed = 16;
    kinematic.maxRotation = M_PI / 6;

    sprite.setPosition(kinematic.position);

    thirst = 80.0;

    target = nullptr;

    // Condition Nodes
    auto isThirstyNode = std::make_shared<ActionNode>([&]() {
        return isThirsty() ? BehaviorStatus::Success : BehaviorStatus::Failure;
    });

    auto canSeeWaterNode = std::make_shared<ActionNode>([&]() {
        return canSeeWater() ? BehaviorStatus::Success : BehaviorStatus::Failure;
    });

    auto canSeePlayerNode = std::make_shared<ActionNode>([&]() {
        return canSeeTarget() ? BehaviorStatus::Success : BehaviorStatus::Failure;
    });

    // Action Nodes
    auto pathToWaterNode = std::make_shared<ActionNode>([&]() {
        return pathToWater();
    });

    auto drinkWaterNode = std::make_shared<ActionNode>([&]() {
        return drinkWater();
    });

    auto tintRedNode = std::make_shared<ActionNode>([&]() {
        sprite.setColor(sf::Color::Red);
        return BehaviorStatus::Success;  // Just tint once
    });

    auto drinkParallel = std::make_shared<Parallel>(
        // Require both to be successful
        Parallel::Policy::RequireAll,
        // Require one to be failure
        Parallel::Policy::RequireOne,
        std::vector<std::shared_ptr<BehaviorTreeNode>>{
            tintRedNode,
            drinkWaterNode
        }
    );
    
    auto resetColorNode = std::make_shared<ActionNode>([&]() {
        sprite.setColor(sf::Color::Green);
        return BehaviorStatus::Success;
    });


    auto chasePlayerNode = std::make_shared<ActionNode>([&]() {
        return chasePlayer();
    });

    auto attackPlayerNode = std::make_shared<ActionNode>([&]() {
        return attackTarget();
    });

    auto wanderNode = std::make_shared<ActionNode>([&]() {
        return wander();
    });

    auto thirstSequenceNode = std::make_shared<Sequence>(std::vector<std::shared_ptr<BehaviorTreeNode>>{
        isThirstyNode, canSeeWaterNode, pathToWaterNode, drinkParallel, resetColorNode
    });
    
    auto chaseSequenceNode = std::make_shared<Sequence>(std::vector<std::shared_ptr<BehaviorTreeNode>>{
        canSeePlayerNode, chasePlayerNode, attackPlayerNode
    });
    
    auto root = std::make_shared<Selector>(std::vector<std::shared_ptr<BehaviorTreeNode>>{
        thirstSequenceNode,
        chaseSequenceNode,
        wanderNode
    });

    // Assign to Monster
    behaviorTree = root;

    currentAction = "wander";


}

Monster::~Monster() {
}

void Monster::update(float deltaTime) {

    behaviorStatus = behaviorTree->tick();

    logState();

    targetKinematic = kinematic;

    targetKinematic.position = targetPos;

    if (target != nullptr) {
        targetPos = target->getKinematic().position;
        targetKinematic.position = target->getKinematic().position;
        //printf("%f\n", breadcrumbs.at(0).getKinematic().position.y);
    }
    
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

    thirst -= deltaTime;
}

void Monster::render(sf::RenderWindow& window) {
    window.draw(visionCircle);
    window.draw(sprite);
} 

void Monster::setPosition(const sf::Vector2f &newPosition) {
    kinematic.position = newPosition;
    sprite.setPosition(kinematic.position);
}


void Monster::setVelocity(const sf::Vector2f &newVelocity) {
    kinematic.velocity = newVelocity;
}


void Monster::setOrientation(const float newOrientation) {
    kinematic.orientation = newOrientation;
    // Ensure the angle is in the range 0 to 2p
    kinematic.orientation = VectorUtils::mapToPiRange(kinematic.orientation);

    // Set rotation of sprite in degrees
    float rot_degrees = VectorUtils::radiansToDegrees(kinematic.orientation);
    sprite.setRotation(rot_degrees);
}


void Monster::setRotation(const float newRotation) {
    kinematic.rotation = newRotation;
}


void Monster::setKinematic(Kinematic &kin) {
    kinematic = kin;
}

void Monster::setTargetKinematic(Kinematic &kin) {
    targetKinematic = kin;
}

Kinematic Monster::getKinematic() {
    return kinematic;
}

void Monster::addSteeringBehavior(std::unique_ptr<SteeringBehavior> behavior) {
    behaviors.push_back(std::move(behavior)); // Move into the vector
}

void Monster::clearSteeringBehaviors() {
    behaviors.clear();
}

bool Monster::isAtTarget() {
    if (VectorUtils::vector2Length(kinematic.position - targetPos) <= 15) {
        return true;
    }
    return false;
}

bool Monster::isThirsty() {
    if (thirst < 40) {
        return true;
    }
    return false;
}

bool Monster::canSeeWater() {
    // If the Monster already had a target
    if (isGettingWater == true) {
        if (VectorUtils::vector2Length(targetPos - kinematic.position) < visionDist) {
            return true;
        }
    }

    isGettingWater = false;
    // If the Monster didn't have water already
    sf::Vector2f waterPos = Game::getInstance().getNearestWaterBreadcrumb(kinematic.position)->getKinematic().position;
    if (VectorUtils::vector2Length(waterPos - kinematic.position) < visionDist) {
        targetPos = waterPos;
        return true;
    }
    
    return false;
}

bool Monster::canSeeTarget() {
    // If the Monster already had a target
    if (target != nullptr) {
        if (VectorUtils::vector2Length(targetPos - kinematic.position) < visionDist) {
            return true;
        }
        else {
            target = nullptr;
            return false;
        }
    }

    // If the Monster didn't have a target already, check to see if it can find one
    std::vector<Entity*> entities =  Game::getInstance().getEntities();
    for (auto entity : entities) {
        // If a target was found
        if (VectorUtils::vector2Length(entity->getKinematic().position - kinematic.position) < visionDist) {
            target = entity;
            return true;
        }
    }
    return false;
}

BehaviorStatus Monster::pathToWater() {
    // If we don't know the current location of water yet
    if (!isGettingWater) {
        isGettingWater = true;
        targetPos = Game::getInstance().getNearestWaterBreadcrumb(kinematic.position)->getPosition();
        clearSteeringBehaviors();
        addSteeringBehavior(std::make_unique<Arrive>(25, 0.5, 10, 50));
        addSteeringBehavior(std::make_unique<Align>(M_PI / 8, 0.5, M_PI / 32, M_PI / 8));
    }

    if (isAtTarget()) {
        currentAction = "pathToWater";
        return BehaviorStatus::Success;
    }
    else {
        currentAction = "pathToWater";
        return BehaviorStatus::Running;
        
    }
}

BehaviorStatus Monster::drinkWater() {
    // Check again we are at our target and confirm that we drank it
    if (isAtTarget()) {
        thirst += 5;

        if (thirst < 100) {
            currentAction = "drinkWater";
            return BehaviorStatus::Running;
        
        }
        else {
            isGettingWater = false;
            currentAction = "drinkWater";
            return BehaviorStatus::Success;
        }
    }
    else {
        isGettingWater = false;
        sprite.setColor(sf::Color::Green);
        return BehaviorStatus::Failure;
    }
}

BehaviorStatus Monster::chasePlayer() {
    if (!isChasing) {
        isChasing = true;
        clearSteeringBehaviors();
        addSteeringBehavior(std::make_unique<PositionMatching>(25, 0.1));
        addSteeringBehavior(std::make_unique<OrientationMatching>(M_PI * .1, 0.1));
    }

    // Check if we can still see the target
    if (!canSeeTarget()) {
        target = nullptr;
        isChasing = false;
        return BehaviorStatus::Failure;
    }

    // Becomes thirsty
    if (isThirsty()) {
        target = nullptr;
        isChasing = false;
        return BehaviorStatus::Failure;
    }

    // We can still see the target, but not reached yet
    if (!isAtTarget()) {
        currentAction = "chasePlayer";
        return BehaviorStatus::Running;
    }
    currentAction = "chasePlayer";
    return BehaviorStatus::Success;
}

BehaviorStatus Monster::attackTarget() {
    if (!canSeeTarget()) {
        target = nullptr;
        isChasing = false;
        return BehaviorStatus::Failure;
    }

    if (!isAtTarget()) {
        isChasing = false;
        currentAction = "attackTarget";
        return BehaviorStatus::Failure;
    }
    // Reset their positions
    target->setPosition(sf::Vector2f(300, 300));
    setPosition(sf::Vector2f(800, 700));
    isChasing = false;
    return BehaviorStatus::Success;
}

BehaviorStatus Monster::wander() {
    if (!isWandering) {
        clearSteeringBehaviors();
        addSteeringBehavior(std::make_unique<Wander>(50, 15, 0.01, 0, 4, M_PI / 16, 0.1, M_PI / 32, M_PI / 4));
    }

    if (isThirsty()) {
        isWandering = false;
        return BehaviorStatus::Failure;
    }
    
    if (canSeeTarget()) {
        isWandering = false;
        return BehaviorStatus::Failure;
    }

    currentAction = "wander";
    return BehaviorStatus::Running;
}

void Monster::logState() {
    static std::ofstream logFile("DataFiles/monsterData.csv", std::ios::app);

    if (!isHeaderWritten) {
        logFile << "isThirsty,canSeeWater,canSeePlayer,isAtTarget,action,status\n";
        isHeaderWritten = true;
    }

    int thirsty = isThirsty() ? 1 : 0;
    int seeWater = (VectorUtils::vector2Length(Game::getInstance().getNearestWaterBreadcrumb(kinematic.position)->getKinematic().position - kinematic.position) < 200) ? 1 : 0;
    int seePlayer = canSeeTarget() ? 1 : 0;
    int atTarget = isAtTarget() ? 1 : 0;
    int gettingWater = isGettingWater ? 1 : 0;
    
    std::cout << "LogData: " << thirsty << "," << gettingWater << "," << seeWater << "," << seePlayer << "," << atTarget << "," << currentAction << "\n" << std::endl;
    logFile << thirsty << "," << gettingWater << "," << seeWater << "," << seePlayer << "," << atTarget << "," << currentAction << "\n";
 }
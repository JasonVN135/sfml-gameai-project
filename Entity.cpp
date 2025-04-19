#include "Entity.h"
#include "SteeringBehavior.h"

Entity::Entity(const int id, const std::string& textureFile, const sf::Vector2f& startPos) {

    if (!texture.loadFromFile(textureFile)) {
        throw std::runtime_error("Failed to load text: " + textureFile);
    }
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

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(50, 750);

    sf::Vector2f bcSpawn(distrib(gen), distrib(gen));
    Breadcrumb* breadcrumb = new Breadcrumb(bcSpawn, 30);
    setBreadcrumb(breadcrumb);

    thirst = 100.0;
    drinking = false;

    auto wanderAction = std::make_shared<Action>("wander");
    auto pathToCenterAction = std::make_shared<Action>("pathToCenter");
    auto pathToWaterAction = std::make_shared<Action>("pathToWater");
    auto drinkAction = std::make_shared<Action>("drink");

    // Decisions
    auto isCloseToWall = std::make_shared<BoolDecision>(
        pathToCenterAction, wanderAction, [this]() {
            return this->isNearWall();
        });

    auto targetReached = std::make_shared<BoolDecision>(
        drinkAction, pathToWaterAction, [this]() {
            return this->isTargetReached();
        });

    auto isThirsty = std::make_shared<FloatDecision>(
        targetReached, isCloseToWall, [this]() {
            return this->getThirst();
        }, 0.0f, 40.0f);

    decisionTree = isThirsty;

    currentAction = "wander";


}

void Entity::update(float deltaTime) {

    targetKinematic.position = kinematic.position;

    targetKinematic.position = breadcrumb->getPosition();
    
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

    thirst -= deltaTime;

    auto decision = decisionTree->makeDecision();
    auto action = std::dynamic_pointer_cast<Action>(decision);

    if (action) {
        std::string actionName = action->getName();
        if (actionName == "wander" && currentAction != "wander") wander();
        else if (actionName == "pathToCenter" && currentAction != "pathToCenter") pathToCenter();
        else if (actionName == "pathToWater" && currentAction != "pathToWater") pathToWater();
        else if (actionName == "drink") drink();
    }
    
}

void Entity::render(sf::RenderWindow& window) {
    window.draw(sprite);
} 

void Entity::setPosition(const sf::Vector2f &newPosition) {
    kinematic.position = newPosition;
    sprite.setPosition(kinematic.position);
}


void Entity::setVelocity(const sf::Vector2f &newVelocity) {
    kinematic.velocity = newVelocity;
}


void Entity::setOrientation(const float newOrientation) {
    kinematic.orientation = newOrientation;
    // Ensure the angle is in the range 0 to 2p
    kinematic.orientation = VectorUtils::mapToPiRange(kinematic.orientation);

    // Set rotation of sprite in degrees
    float rot_degrees = VectorUtils::radiansToDegrees(kinematic.orientation);
    sprite.setRotation(rot_degrees);
}


void Entity::setRotation(const float newRotation) {
    kinematic.rotation = newRotation;
}


void Entity::setKinematic(Kinematic &kin) {
    kinematic = kin;
}

void Entity::setTargetKinematic(Kinematic &kin) {
    targetKinematic = kin;
}

Kinematic Entity::getKinematic() {
    return kinematic;
}

void Entity::addSteeringBehavior(std::unique_ptr<SteeringBehavior> behavior) {
    behaviors.push_back(std::move(behavior)); // Move into the vector
}

void Entity::clearSteeringBehaviors() {
    behaviors.clear();
}

Breadcrumb* Entity::getBreadcrumb() {
    return breadcrumb;
}

void Entity::setBreadcrumb(Breadcrumb *newBreadcrumb) {
    breadcrumb = newBreadcrumb;
}

float Entity::getThirst() {
    return thirst;
}

Breadcrumb* Entity::removeBreadcrumb() {
    Breadcrumb* temp = breadcrumb;
    breadcrumb = nullptr;
    return temp;
}


bool Entity::isTargetReached() {
    if (breadcrumb != nullptr) {
        return breadcrumb->isTargetReached(kinematic.position);
    }
    return false;
}


bool Entity::isNearWall() {
    sf::Vector2f entityPos = kinematic.position;

    if (getBreadcrumb != nullptr) {
        return false;
    }

    if (entityPos.x < 100 || entityPos.x > 900 || entityPos.y < 80 || entityPos.y > 720) {
        return true;
    }
    return false;
}

void Entity::wander() {
    clearSteeringBehaviors();
    removeBreadcrumb();
    addSteeringBehavior(std::make_unique<Wander>(50, 15, 0.01, 0, 4, M_PI / 16, 0.1, M_PI / 32, M_PI / 4));
}

void Entity::pathToCenter() {
    clearSteeringBehaviors();
    removeBreadcrumb();
    setBreadcrumb(new Breadcrumb(sf::Vector2f(500, 400), 10));
    addSteeringBehavior(std::make_unique<Wander>(50, 15, 0.01, 0, 4, M_PI / 16, 0.1, M_PI / 32, M_PI / 4));
}

void Entity::pathToWater() {
    clearSteeringBehaviors();
    removeBreadcrumb();
    Breadcrumb* water = Game::getInstance().getNearestWaterBreadcrumb(kinematic.position);
    setBreadcrumb(water);
    addSteeringBehavior(std::make_unique<Arrive>(15, 0.1, 10, 30));
    addSteeringBehavior(std::make_unique<Align>(M_PI / 6, 0.1, M_PI / 32, M_PI / 8));
}

void Entity::drink() {
    thirst = 100;
}
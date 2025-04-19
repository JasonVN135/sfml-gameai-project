#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>
#include "Kinematic.h"
#include "VectorUtils.h"
#include "SteeringOutput.h"
#include "Breadcrumb.h"
#include "DecisionTreeNode.h"

class SteeringBehavior;

class Entity {
private:

    /** The sprite */
    sf::Sprite sprite;
    /** The texture of the entity */
    sf::Texture texture;
    /** The entity's Kinematic */
    Kinematic kinematic;
    /** The Steering Behaviors the Entity will follow */
    std::vector<std::unique_ptr<SteeringBehavior>> behaviors;
    /** The list of breadcrumbs the Entity will try and chase */
    Breadcrumb* breadcrumb;
    /** The kinematic struct that entity will aim for */
    Kinematic targetKinematic;

    bool drinking;
    float thirst;

    std::shared_ptr<DecisionTreeNode> decisionTree;

    // Actions
    void wander();
    void pathToCenter();
    void pathToWater();
    void drink();

    std::string currentAction;
    
public:

    /**
     * The entity constructor that creates a generic entity
     * 
     * @oaram id The id of the Entity
     * @param textureFile The texture the entity will store
     * @param startPos The initial position of the entity
     */
    Entity(const int id, const std::string& textureFile, const sf::Vector2f& startPos);

    /**
     * The entity deconstructor
     */
    ~Entity() = default;
    
    

    /**
     * Update the entity's position
     * 
     * @param deltaTime time elapsed since last rerender
     */
    void update(float deltaTime);


    /**
     * Render the entity on the window
     * 
     * @param window The window to render the entity on
     */
    void render(sf::RenderWindow& window);

    
    /**
     * Get the kinematic of the entity
     * 
     * @return the kinematic struct
     */
    Kinematic getKinematic();

    /**
     * Set the position of the entity
     * 
     * @param newPosition The position to set
     */
    void setPosition(const sf::Vector2f& newPosition);

    /**
     * Set the velocity of the Entity
     * 
     * @param newVelocity The velocity to set
     */
    void setVelocity(const sf::Vector2f &newVelocity);

    /**
     * Set the rotation of the Entity
     * 
     * @param newOrientation The rotation to set
     */
    void setOrientation(const float newOrientation);

    /**
     * Set the rotation speed of the Entity
     * 
     * @param newRotation The rotation speed to set
     */
    void setRotation(const float newRotation);

    /**
     * Completely set the kinematic struct of the entity
     */
    void setKinematic(Kinematic &kin);

    /**
     * Set the target Kinematic
     */
    void setTargetKinematic(Kinematic &kin);

    /**
     * Add a steering behavior
     */
    void addSteeringBehavior(std::unique_ptr<SteeringBehavior> behavior);

    /**
     * Clear all steering behaviors
     */
    void clearSteeringBehaviors();


    /**
     * Get the breadcrumb the entity is trying to reach
     * 
     * @return the breadcrumb the entity is moving to
     */
    Breadcrumb* getBreadcrumb();

    /**
     * Set the breadcrumb the ai will follow
     */
    void setBreadcrumb(Breadcrumb *breadcrumb);

    float getThirst();

    /**
     * Remove a specific breadcrumb
     */
    Breadcrumb* removeBreadcrumb();

    /**
     * Check if sprite reached the breadcrumb
     */
    bool isTargetReached();

    bool isThirsty();

    bool isNearWall();
    
};



#endif // ENTITY_H
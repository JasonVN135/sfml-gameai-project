#ifndef MONSTER_H
#define MONSTER_H

#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include "BehaviorTreeNode.h"
#include "Kinematic.h"
#include "VectorUtils.h"
#include "SteeringOutput.h"
#include "Entity.h"
#include <iostream>

class SteeringBehavior;

class Monster {
private:

    /** The sprite */
    sf::Sprite sprite;
    /** The vision circle */
    sf::CircleShape visionCircle;
    /** The texture of the entity */
    sf::Texture texture;
    /** The entity's Kinematic */
    Kinematic kinematic;
    /** The Steering Behaviors the Entity will follow */
    std::vector<std::unique_ptr<SteeringBehavior>> behaviors;
    /** The kinematic struct that entity will aim for */
    Kinematic targetKinematic;
    /** The distance for monster vision */
    float visionDist;
    /** The thirst value of the Entity */
    float thirst;
    /** The decission tree for the entity */
    std::shared_ptr<BehaviorTreeNode> behaviorTree;
    /** The target for the Monster */
    Entity* target = nullptr;
    /** The target position */
    sf::Vector2f targetPos;
    /** Status for Wandering */
    bool isWandering;
    /** Status for Chasing */
    bool isChasing;
    /**Status for moving to water */
    bool isGettingWater;
    /** The current action the Monster is performing*/
    std::string currentAction;
    /** The recent behavior status */
    BehaviorStatus behaviorStatus;
    /** is Header written yet */
    bool isHeaderWritten;


public:

    /**
     * The entity constructor that creates a generic entity
     * 
     * @param id The id of the Entity
     * @param textureFile The texture the entity will store
     * @param startPos The initial position of the entity
     * @param visionDist The distance for vision
     */
    Monster(const int id, const std::string& textureFile, const sf::Vector2f& startPos, const float vision);

    /**
     * The entity deconstructor
     */
    ~Monster();
    

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
     * Get the kinematic of the entity
     * 
     * @return the kinematic struct
     */
    Kinematic getKinematic();

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
     * Check if the target has been reached
     */
    bool isAtTarget();

    // Conditionals

    /**
     * Return if the Monster is thirsty or not
     */
    bool isThirsty();

    /**
     * Return if the Monster can see the water
     */
    bool canSeeWater();

    /**
     * Return if the Monster can see the player
     */
    bool canSeeTarget();
    
    // Actions

    /**
     *  Returns the status of pathing to water
     */
    BehaviorStatus pathToWater();

    /**
     * Returns the status for drinking water
     */
    BehaviorStatus drinkWater();

    /**
     * Returns the status for chasing the player
     */
    BehaviorStatus chasePlayer();

    /**
     * Returns the status for attacking the target
     */
    BehaviorStatus attackTarget();

    /**
     * Returns the status for wandering
     */
    BehaviorStatus wander();

    /**
     * Log the action into a CSV File
     */
    void logState();
    
};



#endif // MONSTER_H
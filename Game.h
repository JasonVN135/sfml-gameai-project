#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <fstream>
#include <sstream>
#include "Breadcrumb.h"
#include "Entity.h"
#include "Monster.h"
#include "LearningMonster.h"
#include "SteeringBehavior.h"
#include "VelocityMatchStruct.h"

class Game {

private:

    /** The Render Window to View */
    sf::RenderWindow window;
    /** In Game clock for deltaTime */
    sf::Clock clock;
    /** Vector of entities */
    std::vector<Entity*> entities;
    /** Vector of monsters */
    std::vector<Monster*> monsters;
    /** Vector of learning monsters */
    std::vector<LearningMonster*> learningMonsters;
    /** Vector of breadcrumbs */
    std::vector<Breadcrumb*> waters;
    /** Number of entities that have spawned */
    int entityCount;
    /** Number of monsters that have spawned */
    int monsterCount;
    /** Number of learning monsters */
    int learningMonsterCount;
    /** The set arrive distance for all navNodes */
    float arriveDistance;
    /** The set slow distance for all navNodes */
    float slowDistance;
    /** Velocity Match Struct for Velocity Matching */
    VelocityMatchStruct velocityStruct;

    /**
     * The Game Class constructor. Initialize window, variables, etc.
     */
    Game();
    

    /**
     * Game deconstructor
     */
    ~Game();

    /**
     * Process user input events
     */
    void processEvents();


    /**
     * Update loop to update Entities
     * 
     * @param deltaTime Time since last update loop
     */
    void update(float deltaTime);

    /**
     * Render all objects in the scene in the window
     */
    void render();


public:

    enum STEERING_TYPE {
        POS_AND_ORI,
        VELOCITY,
        ARRIVE_AND_ALIGN,
        ARRIVE_AND_ALIGN_2,
        WANDER,
        WANDER_2,
        FLOCKING
    };

    /** is Header written yet for monster file */
    bool isHeaderWritten;


    /** The Current Steering Behavior Type */
    STEERING_TYPE currentSteeringType;

    /**
     * Singleton instance
     */
    static Game& getInstance();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    /**
     * Method to run the main game loop
     */
    void run();

    /**
     * Get the list of all entities
     */
    std::vector<Entity*> getEntities();

    /**
     * Get the list of all monsters
     */
    std::vector<Monster*> getMonsters();

    /**
     * Get the list off all learning monsters
     */
    std::vector<LearningMonster*> getLearningMonsters();

    /**
     * Method to spawn an AI Object.
     */
    void spawnEntity(float x, float y);

    /**
     * Method to spawn a Monster.
     */
    void spawnMonster(float x, float y);

    /**
     * Spawn LearningMonster
     */
    void spawnLearningMonster(float x, float y, std::string dataFile);

    /**
     * Check if the entity is out of bounds
     */
    void checkOutOfBounds();

    /**
     * Set the steering behavior based on user choice
     */
    void setSteeringBehavior(STEERING_TYPE steeringChoice);

    /**
     * Get the closest water Breadcrumb Node
     */
    Breadcrumb* getNearestWaterBreadcrumb(sf::Vector2f currentPos);

};





#endif // GAME_HPP
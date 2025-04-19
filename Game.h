#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <fstream>
#include <sstream>
#include "Breadcrumb.h"
#include "Entity.h"
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
    /** Vector of breadcrumbs */
    std::vector<Breadcrumb*> waterBreadcrumbs;
    /** Number of entities that have spawned */
    int entityCount;
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
    const float DROP_TIME = 4.0;

    enum STEERING_TYPE {
        POS_AND_ORI,
        VELOCITY,
        ARRIVE_AND_ALIGN,
        ARRIVE_AND_ALIGN_2,
        WANDER,
        WANDER_2,
        FLOCKING
    };

    enum SEARCH_TYPE {
        DIJKSTRA,
        ASTAR
    };


    /** The Current Steering Behavior Type */
    STEERING_TYPE currentSteeringType;
    /** The search algorithm to use */
    SEARCH_TYPE searchType;


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
     * Method to spawn an AI Object. Attach 4 Markers for the AI to path towards.
     */
    void spawnEntity(float x, float y);

    /**
     * Check if the entity is out of bounds
     */
    void checkOutOfBounds();


    /**
     * Set the steering behavior based on user choice
     */
    void setSteeringBehavior(STEERING_TYPE steeringChoice);


    /**
     * Set the velocity match for entities if using velocity match
     */
    void sendVelocityMatch(float deltaTime);

    Breadcrumb* getNearestWaterBreadcrumb(sf::Vector2f currentPos);

};





#endif // GAME_HPP
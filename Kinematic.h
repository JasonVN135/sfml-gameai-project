#ifndef KINEMATIC_H
#define KINEMATIC_H

#include <SFML/Graphics.hpp>

struct Kinematic {
    /** Id of the Kinematic owner */
    int id;
    /** The position of the entity */
    sf::Vector2f position;
    /** The velocity of the entity */
    sf::Vector2f velocity; 
    /**The rotation (orientation) of the entity */
    float orientation;
    /** The rotation speed of the entity */
    float rotation;
    /** The max speed of the entity */
    float maxSpeed;
    /** The  max angular rotation of the entity */
    float maxRotation;

    Kinematic() : id(-1), position(sf::Vector2f(0, 0)), velocity(sf::Vector2f(0, 0)), orientation(0), rotation(0), maxSpeed(0), maxRotation(0) {} // Default constructor

};


#endif
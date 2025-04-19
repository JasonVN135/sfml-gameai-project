#ifndef VELOCITY_MATCH_STRUCT_H
#define VELOCITY_MATCH_STRUCT_H

#include <SFML/Graphics.hpp>

struct VelocityMatchStruct {

    /** The linear acceleration of the entity */
    sf::Vector2f prevLoc;
    /** The angular acceleration of the entity */
    sf::Vector2f nextLoc;
    /** Time left */
    float timeLeft;

    VelocityMatchStruct() : prevLoc(sf::Vector2f(0, 0)), nextLoc(sf::Vector2f(0, 0)), timeLeft(0) {} // Default constructor

};


#endif
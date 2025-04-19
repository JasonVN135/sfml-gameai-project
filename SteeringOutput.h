#ifndef STEERING_OUTPUT_H
#define STEERING_OUTPUT_H

#include <SFML/Graphics.hpp>

struct SteeringOutput {

    /** The linear acceleration of the entity */
    sf::Vector2f linear;
    /** The angular acceleration of the entity */
    float angular;

    SteeringOutput() : linear(sf::Vector2f(0, 0)), angular(0) {} // Default constructor

};


#endif
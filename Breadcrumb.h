#ifndef BREADCRUMB_H
#define BREADCRUMB_H

#include <SFML/Graphics.hpp>
#include <cmath>

#include "Kinematic.h"
#include "VectorUtils.h"

class Breadcrumb : sf::CircleShape {

private:
    /** The Navigation Node's Kinematic */
    Kinematic kinematic;

public:

    /**
     * Constructor of the Breadcrumb
     * 
     * @param position The position of the Breadcrumb
     */
    Breadcrumb(const sf::Vector2f &position, const float radius);

    /**
     * Deconstructor of the Navigation Node
     */
    virtual ~Breadcrumb() = default;

    /**
     * Render the entity on the window
     * 
     * @param window The window to render the entity on
     */
    void render(sf::RenderWindow& window);

    /**
     * Get the position of the navigation Node
     * 
     * @return the position of the Breadcrumb
     */
    sf::Vector2f getPosition();

    /**
     * Return the kinematic of the breadcrumb
     * 
     * @return kinematic
     */
    Kinematic getKinematic();

    /**
     * Set the kinematic of the breadcrumb
     * 
     * @param kin The kinematic to set
     */
    void setKinematic(Kinematic &kin);

    /**
     * Check if the provided position is in reach of the Breadcrumb
     * 
     * @param entityPosition position to check if it's in range
     * @return if the position is in the Breadcrumb distance
     */
    bool isTargetReached(sf::Vector2f entityPosition);

};


#endif // NAV_NODE_H
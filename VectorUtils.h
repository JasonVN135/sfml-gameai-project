#ifndef VECTOR_UTIL_H
#define VECTOR_UTIL_H


#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <random>


class VectorUtils {
public:

    /**
     * Get the magnitude of a given Vector2f
     * 
     * @param vec The vector to calculate
     * @return the magnitude of the vecotr
     */
    static float vector2Length(sf::Vector2f vec);

    /**
     * Normalize a given vector
     * 
     * @param vec the vector to normalize
     * @return the normalized vector
     */
    static sf::Vector2f normalize(sf::Vector2f vec);

    /**
     * Map a float or orientation between -PI to PI
     *
     * @param orientation the value to map
     * @return the value mapped
     */
    static float mapToPiRange(const float orientation);


    /**
     * Turn a float, which represents an orientation to a normalized vector
     * 
     * @param orientation the orientation to map as a vector
     * @return the facing direction based on the float
     */
    static sf::Vector2f facingDirection(const float orientation);


    /**
     * Return a float represented as degrees instead of radians
     * 
     * @param radians the value to convert to degrees
     * @return the value as degrees
     */
    static float radiansToDegrees(const float radians);

    static float randomBinomial();

};

#endif // VECTOR_UTIL_H
#include "VectorUtils.h"


float VectorUtils::vector2Length(sf::Vector2f vec) {
    return std::sqrt((vec.x * vec.x) + (vec.y * vec.y));
}

sf::Vector2f VectorUtils::normalize(sf::Vector2f vec) {
    float magnitude = std::sqrt(vec.x * vec.x + vec.y * vec.y);
    if (magnitude != 0) {
        return vec / magnitude;
    }
    return sf::Vector2f(0.f, 0.f); // Return a zero vector if the input vector has zero length
}

float VectorUtils::mapToPiRange(const float orientation) {

    float orientationCopy = orientation;

    while (orientationCopy < -M_PI || orientationCopy > M_PI) {

        if (orientationCopy < -M_PI) {
            orientationCopy += (2 * M_PI);
        }
        else if (orientationCopy > M_PI) {
            orientationCopy -= (2 * M_PI);
        }
    }
    
    return orientationCopy;
}

sf::Vector2f VectorUtils::facingDirection(const float orientation) {
    return sf::Vector2f(cos(orientation), sin(orientation));
}

float VectorUtils::radiansToDegrees(const float radians) {
    return radians * (180 / M_PI);
}

float VectorUtils::randomBinomial() {
    std::random_device rd;  // Random device to seed the random engine
    std::default_random_engine generator(rd());  // Initialize the generator with the seed

    int n = 10; 
    double p = 0.5;  // Probability of success (50% success rate)
    
    // Define the binomial distribution
    std::binomial_distribution<int> dist(n, p);

    return ((float) dist(generator));

}
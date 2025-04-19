#include "Breadcrumb.h"

Breadcrumb::Breadcrumb(const sf::Vector2f &position, const float arriveDist) {
    this->setOrigin(arriveDist, arriveDist);
    kinematic.position = position;
    this->setRadius(5);
    this->setFillColor(sf::Color(68, 85, 200, 100));
    this->setPosition(position.x + 5, position.y + 5);
}

void Breadcrumb::render(sf::RenderWindow& window) {
    window.draw(*this);
} 


sf::Vector2f Breadcrumb::getPosition() {
    return kinematic.position;
}

Kinematic Breadcrumb::getKinematic() {
    return kinematic;
}

void Breadcrumb::setKinematic(Kinematic &kin) {
    kinematic = kin;
}

bool Breadcrumb::isTargetReached(sf::Vector2f agent_position) {
    
    sf::Vector2f distance_vec = kinematic.position - agent_position;
    float distance_length = VectorUtils::vector2Length(distance_vec);
    if (distance_length < getRadius()) {
        return true;
    }
    return false;
}
#include "SteeringBehavior.h"
#include "Entity.h"


PositionMatching::PositionMatching(const float maxAccel, const float time) {
    maxAcceleration = maxAccel;
    timeToTarget = time;

}

SteeringOutput PositionMatching::update(Kinematic &playerKinematic, const Kinematic &targetKinematic) {
    
    // Initialize a steeringOutput
    SteeringOutput steeringOutput;

    steeringOutput.linear = targetKinematic.position - playerKinematic.position;
    steeringOutput.linear = VectorUtils::normalize(steeringOutput.linear) * maxAcceleration;

    steeringOutput.angular = 0;

    return steeringOutput;
}

Arrive::Arrive(const float maxAccel, const float time, const float targetRad, const float slowRad) : PositionMatching(maxAccel, time) {
    targetRadius = targetRad;
    slowRadius = slowRad;
}

SteeringOutput Arrive::update(Kinematic &playerKinematic, const Kinematic &targetKinematic) {

    // Initialize a steering output
    SteeringOutput steeringOutput;

    // Get direction and distance from player to target
    sf::Vector2f direction = targetKinematic.position - playerKinematic.position;
    float distance = VectorUtils::vector2Length(direction);

    // Initialize a target speed variable
    float targetSpeed = 0.0;

    // If player arrived at target
    if (distance < targetRadius) {
        return steeringOutput;
    }
    
    // If player is outside of slow radius
    if (distance > slowRadius) {
        targetSpeed = playerKinematic.maxSpeed;
    }

    // Else, slow down the targetSpeed
    else {
        targetSpeed = playerKinematic.maxSpeed * distance / slowRadius;
    }

    // Target velocity combines speed and direction
    sf::Vector2f targetVelocity = direction;
    targetVelocity = VectorUtils::normalize(targetVelocity);
    targetVelocity *= targetSpeed;

    // Acceleration tries to get to the target Velocity
    steeringOutput.linear = targetVelocity - playerKinematic.velocity;
    steeringOutput.linear /= timeToTarget;

    // Check if acceleration is too fast
    if (VectorUtils::vector2Length(steeringOutput.linear) > maxAcceleration) {
        steeringOutput.linear = VectorUtils::normalize(steeringOutput.linear) * maxAcceleration;
    }

    //printf("%f\n", steeringOutput.linear.y);
    // Output steering
    steeringOutput.angular = 0;
    return steeringOutput;

}

OrientationMatching::OrientationMatching(const float maxAngular, const float time) {
    maxAngularAcceleration = maxAngular;
    timeToTarget = time;
}


SteeringOutput OrientationMatching::update(Kinematic &playerKinematic, const Kinematic &targetKinematic) {
    SteeringOutput steeringOutput;
    
    
    // The orientation difference between player and target
    sf::Vector2f targetDirection = VectorUtils::normalize(targetKinematic.position - playerKinematic.position);
    float targetOrientation = atan2(targetDirection.y, targetDirection.x);


    float rotationToTarget = targetOrientation - playerKinematic.orientation;

    // Map the result to -PI to PI interval
    rotationToTarget = VectorUtils::mapToPiRange(rotationToTarget);
    float direction = rotationToTarget;
    if (std::fabs(direction) > 1) {
        direction = std::copysign(1.0f, rotationToTarget);
    }
    steeringOutput.angular = direction * maxAngularAcceleration;

    // Output steering
    steeringOutput.linear = sf::Vector2f(0, 0);

    return steeringOutput;
}

Align::Align(const float maxAngular, const float time, const float targetRad, const float slowRad) : OrientationMatching(maxAngular, time) {
    targetRadius = targetRad;
    slowRadius = slowRad;
}

SteeringOutput Align::update(Kinematic &playerKinematic, const Kinematic &targetKinematic) {
    SteeringOutput steeringOutput;

    // The orientation difference between player and target
    sf::Vector2f targetDirection = VectorUtils::normalize(targetKinematic.position - playerKinematic.position);
    float targetOrientation = atan2(targetDirection.y, targetDirection.x);
    
    float rotationToTarget = targetOrientation - playerKinematic.orientation;
    // Map the result to -PI to PI interval
    rotationToTarget = VectorUtils::mapToPiRange(rotationToTarget);
    float rotationSize = std::fabs(rotationToTarget);

    // The rotation amount to achieve
    float targetRotation = 0.0;

    // Check if reached correct rotation, return nothing
    if (rotationSize < targetRadius) {
        return steeringOutput;
    }

    // If outside slow radius, use max rotation speed
    if (rotationSize > slowRadius) {
        targetRotation = playerKinematic.maxRotation;
    }

    // Caluclate scaled rotation speed
    else {
        targetRotation = playerKinematic.maxRotation * rotationSize / slowRadius;
    }

    // final target combines speed and direction
    targetRotation *= rotationToTarget / rotationSize;


    // Acceleration tries to get to the target rotation
    steeringOutput.angular = targetRotation - playerKinematic.rotation;
    steeringOutput.angular /= timeToTarget;

    // Check if acceleration is too great
    float angularAcceleration = fabs(steeringOutput.angular);
    if (angularAcceleration > maxAngularAcceleration) {
        steeringOutput.angular = steeringOutput.angular / angularAcceleration * maxAngularAcceleration;
    }

    // Output steering
    steeringOutput.linear = sf::Vector2f(0, 0);
    return steeringOutput;
}


VelocityMatching::VelocityMatching(const float maxAccel, const float time) {
    maxAcceleration = maxAccel;
    timeToTarget = time;

}

SteeringOutput VelocityMatching::update(Kinematic &playerKinematic, const Kinematic &targetKinematic) {
    // Initialize a steeringOutput
    SteeringOutput steeringOutput;
    
    steeringOutput.linear = targetKinematic.velocity - playerKinematic.velocity;
    steeringOutput.linear /= timeToTarget;

    // Check if the acceleration is too fast
    if (VectorUtils::vector2Length(steeringOutput.linear) > maxAcceleration) {
        steeringOutput.linear = VectorUtils::normalize(steeringOutput.linear) * maxAcceleration;
    }

    steeringOutput.angular = 0;
    return steeringOutput;
}

RotationMatching::RotationMatching(const float maxAngular, const float time) {
    maxAngularAcceleration = maxAngular;
    timeToTarget = time;
}

SteeringOutput RotationMatching::update(Kinematic &playerKinematic, const Kinematic &targetKinematic) {
    SteeringOutput steeringOutput;

    // Set the target rotaiton
    float targetRotation = targetKinematic.rotation - playerKinematic.rotation;

    // Acceleration tries to get to the target rotation
    steeringOutput.angular = targetRotation;
    steeringOutput.angular /= timeToTarget;
    printf("%f\n", steeringOutput.angular);

    // Check if acceleration is too great
    float angularAcceleration = abs(steeringOutput.angular);
    if (angularAcceleration > maxAngularAcceleration) {
        steeringOutput.angular = steeringOutput.angular / angularAcceleration * maxAngularAcceleration;
    
    }

    // Output steering
    steeringOutput.linear = sf::Vector2f(0, 0);

    return steeringOutput;
}

Wander::Wander(const float offset, const float radius, const float rate, const float ori, const float maxAccel, const float maxAngularAccel, const float time, const float targetRad, const float slowRad) : Align(maxAngularAccel, time, targetRad, slowRad) {
    wanderOffset = offset;
    wanderRadius = radius;
    wanderRate = rate;
    wanderOrientation = ori;
    maxAcceleration = maxAccel;
}

SteeringOutput Wander::update(Kinematic &playerKinematic, const Kinematic &targetKinematic) {

    SteeringOutput steeringOutput;

    // Calculate target to delegate face
    wanderOrientation += VectorUtils::randomBinomial()  + wanderRate;

    // Calculate the combined target orientation
    float targetOrientation = wanderOrientation + playerKinematic.orientation;

    // Calculate the center of the wander circle
    sf::Vector2f target = playerKinematic.position + wanderOffset * VectorUtils::facingDirection(playerKinematic.orientation);

    target += wanderRadius * VectorUtils::facingDirection(targetOrientation);
    
    Kinematic newKin = targetKinematic;
    newKin.position = target;

    steeringOutput = Align::update(playerKinematic, newKin);

    // 3. Now set the linear acceleration to be at full
    // acceleration in the direction of the orientation
    steeringOutput.linear = maxAcceleration * VectorUtils::facingDirection(playerKinematic.orientation);

    
    return steeringOutput;
}

Flocking::Flocking(const float turnFact, const float visRange, const float protectRange, const float centerFact, const float avoidFact, const float matchFact, const float maxAccel) {
    turnFactor = turnFact;
    visualRange = visRange;
    protectedRange = protectRange;
    centeringFactor = centerFact;
    avoidFactor = avoidFact;
    matchingFactor = matchFact;
    maxAcceleration = maxAccel;
}

SteeringOutput Flocking::update(Kinematic &playerKinematic, const Kinematic &targetKinematic) {
    SteeringOutput steeringOutput;

    float xPosAvg = 0.0;
    float yPosAvg = 0.0;
    float xVelAvg = 0.0;
    float yVelAvg = 0.0;
    int neighboringEntities = 0;
    float closeDx = 0.0;
    float closeDy = 0.0;

    std::vector<Entity*> entities = Game::getInstance().getEntities();

    // For every other boid in the flock...
    for (auto entity : entities) {
        if (entity->getKinematic().id == playerKinematic.id) {
            continue;
        }
        printf("%d\n", entity->getKinematic().id);

        // Compute differences in x and y coordinates
        float dx = playerKinematic.position.x - entity->getKinematic().position.x;
        float dy = playerKinematic.position.y - entity->getKinematic().position.y;

        // Check if the other boid is within visual range
        if (std::fabs(dx) < visualRange and std::fabs(dy) < visualRange) {
            // Calculate squared distance
            float squaredDistance = dx * dx + dy * dy;

            // If within protected range, apply separation behavior
            if (squaredDistance < protectedRange * protectedRange) {
                closeDx += dx;
                closeDy += dy;
            }
            // Otherwise, apply alignment and cohesion
            else if (squaredDistance < visualRange * visualRange) {
                xPosAvg += entity->getKinematic().position.x;
                yPosAvg += entity->getKinematic().position.y;
                xVelAvg += entity->getKinematic().velocity.x;
                yVelAvg += entity->getKinematic().velocity.y;
                neighboringEntities += 1;
            }
        }
    }
    float ax = 0.0;
    float ay = 0.0;

    // Apply cohesion and alignment if neighbors are detected
    if (neighboringEntities > 0) {
        xPosAvg /= neighboringEntities;
        yPosAvg /= neighboringEntities;
        xVelAvg /= neighboringEntities;
        yVelAvg /= neighboringEntities;

        // Calculate cohesion steering force (move toward center of mass)
        ax += (xPosAvg - playerKinematic.position.x) * centeringFactor;
        ay += (yPosAvg - playerKinematic.position.y) * centeringFactor;

        // Calculate alignment steering force (match velocity)
        ax += (xVelAvg - playerKinematic.velocity.x) * centeringFactor;
        ay += (yVelAvg - playerKinematic.velocity.y) * centeringFactor;
    }

    // Apply separation steering force
    ax += closeDx * avoidFactor;
    ay += closeDy * avoidFactor;    

    // Boundary avoidance
    if (playerKinematic.position.x < 0) {
        ax += turnFactor;
    }
    if (playerKinematic.position.x > 800) {
        ax -= turnFactor;
    }
    if (playerKinematic.position.y < 0) {
        ay += turnFactor;
    }
    if (playerKinematic.position.y > 600) {
        ay -= turnFactor;
    }

    steeringOutput.linear = sf::Vector2f(ax, ay);
    printf("%f %f\n", steeringOutput.linear.x, steeringOutput.linear.y);
    return steeringOutput;

}
#ifndef STEERING_BEHAVIOR_H
#define STEERING_BEHAVIOR_H

#include <cmath> 
#include "Kinematic.h"
#include "SteeringOutput.h"
#include "VectorUtils.h"
#include "Game.h"

class Entity;


class SteeringBehavior {

public:
    /**
     * Virtual constructor for steering behavior
     */
    virtual ~SteeringBehavior() = default;

     /**
     * Virtual update method for Matching classes
     * 
     * @param playerKinematic The player's kinematic structure
     * @param targetKinematic The target's kinematic structure
     */
    virtual SteeringOutput update(Kinematic &playerKinematic, const Kinematic &targetKinematic) = 0;
};

class PositionMatching : public SteeringBehavior {

public:

    /** Holds the max acceleration of the character */
    float maxAcceleration = 1;


    /** Holds the time over which to achieve target speed */
    float timeToTarget = .1;

    PositionMatching(const float maxAccel, const float time);

    /**
     * Match the player's orientation to the target's position
     * 
     * @param playerKinematic The player's kinematic structure
     * @param targetKinematic The target's kinematic structure
     */
    SteeringOutput update(Kinematic &playerKinematic, const Kinematic &targetKinematic) override;

};

class Arrive : public PositionMatching {

public:

    /** Holds the radius for arriving at the target */
    float targetRadius = 10;

    /** Holds the radius for beginning to slow down */
    float slowRadius;

    Arrive(const float maxAccel, const float time, const float targetRad, const float slowRad);

    /**
     * Match the player's position to the target's position using
     * the Align algorithm
     * 
     * @param playerKinematic The player's kinematic structure
     * @param targetKinematic The target's kinematic structure
     */
    SteeringOutput update(Kinematic &playerKinematic, const Kinematic &targetKinematic) override;

};

class OrientationMatching : public SteeringBehavior {

public:

    /** The max angular acceleration */
    float maxAngularAcceleration = M_PI / 16;

    /** The time for the player to reach target speed */
    float timeToTarget = 0.1;

    OrientationMatching(const float maxAngular, const float time);


    /**
     * Match the player's orientation to the target's orientation
     * 
     * @param playerKinematic The player's kinematic structure
     * @param targetKinematic The target's kinematic structure
     */
    SteeringOutput update(Kinematic &playerKinematic, const Kinematic &targetKinematic) override;

};

class Align : public OrientationMatching{

public:

    /** The distance the player considers to reach the target */
    float targetRadius;

    /** Holds the radius for beginning to slow down */
    float slowRadius;

    Align(const float maxAngular, const float time, const float targetRad, const float slowRad);


    /**
     * Match the player's orientation to the target's orientation
     * using the Align algorithm
     * 
     * @param playerKinematic The player's kinematic structure
     * @param targetKinematic The target's kinematic structure
     */
    SteeringOutput update(Kinematic &playerKineamtic, const Kinematic &targetKinematic) override;

};


class VelocityMatching : public SteeringBehavior {

public:

    /** Holds the max acceleration of the character */
    float maxAcceleration;

    /** The time for the player to reach target speed */
    float timeToTarget;


    VelocityMatching(const float maxAccel, const float time);


    /**
     * Match the player's orientation to the target's velocity
     * 
     * @param playerKinematic The player's kinematic structure
     * @param targetKinematic The target's kinematic structure
     */
    SteeringOutput update(Kinematic &playerKinematic, const Kinematic &targetKinematic) override;

};

class RotationMatching : public SteeringBehavior {

public:

    /** The max angular acceleration */
    float maxAngularAcceleration;

    /** The time for the player to reach target speed */
    float timeToTarget;


    RotationMatching(const float maxAngular, const float time);


    /**
     * Match the player's orientation to the target's rotation
     * 
     * @param playerKinematic The player's kinematic structure
     * @param targetKinematic The target's kinematic structure
     */
    SteeringOutput update(Kinematic &playerKinematic, const Kinematic &targetKinematic) override;

};

class Wander : public Align {

public:

    float wanderOffset;

    float wanderRadius;

    float wanderRate;

    float wanderOrientation;

    float maxAcceleration;

    Wander(const float offset, const float radius, const float rate, const float ori, const float maxAccel, const float maxAngularAccel, const float time, const float targetRad, const float slowRad);

    SteeringOutput update(Kinematic &playerKinematic, const Kinematic &targetKinematic) override;
};

class Flocking : public SteeringBehavior {

public:

    float turnFactor;

    float visualRange;

    float protectedRange;

    float centeringFactor;

    float avoidFactor;

    float matchingFactor;

    float maxAcceleration;

    /**
     * @param turnFact
     * @param visRange
     * @param protectRange
     * @param centerFact
     * @param avoidFact
     * @param matchFact
     * @param maxAccel
     */
    Flocking(const float turnFact, const float visRange, const float protectRange, const float centerFact, const float avoidFact, const float matchFact, const float maxAccel);

    SteeringOutput update(Kinematic &playerKinematic, const Kinematic &targetKinematic) override;
};

#endif // End STEERING_BEHVIOR_H
# Homework 2

This project is a SFML-based application designed to demonstrate Steering Behaviors. 
Below is an overview of the structure and instructions for compilation and execution. Addition descriptions
of the project structure can be found below.

---

## Requirements

- Ubuntu 20.04
- SFML library installed (version 2.6.2 or compatible)


## Setup

1. Open a terminal
2. Run the following command lines to ensure proper installation of required software
    - sudo apt update && sudo apt -y upgrade
    - sudo apt -y install build-essential libsfml-dev


## Compilation

1. Remove any compiled objects and the executable.
2. Open a desktop terminal
3. Navigate to the directory that contains the project
4. Execute the following command lines in the terminal
    - make
    - ./main

---

## Project Structure

### Source Files

- main.cpp: The entry point of the program. Its sole purpose is to call the main loop in Game.cpp and exit when requested.
- Game.cpp: Handles the main game loop and manages the spawning of Entites and creation of the Graph. Handles inputs from the users to determine which graph to display and which search algorithm to use.
- Entity.cpp: A general-purpose entity class for implementation. Contains:
    - sf::Sprite and sf::Texture for visual representation
    - Key values for movement
    - breadcrumbs: A list of breadcrumbs that the entity may intend to path to
    - steeringBehaviors: A list of different steering behaviors to calculate for correct movement
- Breadcrumb.cpp: Represents navigation points used by Entity for pathfind. Contains positional data and an acceptance threshold.
- SteeringBehavior.cpp: A virtual class. Additional classes can derive from it to create various different steering behaviors
    - PositionMatch - Parent: SteeringBehavior
    - Arrive - Parent: PositionMatching
    - OrientationMatching - Parent: SteeringBehavior
    - Align - Parent: OrientationMatching
    - Wander - Parent: Align
    - VelocityMatching - Parent: SteeringBehavior
    - RotationMatching - Parent: SteeringBehavior
    - Flocking - Parent: SteeringBehavior
- VectorUtils.cpp: A static class to hold various algebra calculatation to be used by any class
- Graph.cpp: A structure that contains information used to form a graph, including vertices and edges.

### Header Files
- Each .cpp file (except main.cpp) has a corresponding .h file that defines its classes and functions
- SteeringOutput, Kinematic, and VelocityMatchStruct are structs used to hold specific data.

### Inputs
- Num0: Create the default USA border map
- Num1: Create a randomly generated graph with 500 vertices
- NUm2: Create a randomly generated graph with 5000 vertices
- Num3: Have the Entity use Dijkstra's Algorithm
- Num4: Have the Entity use Astar Algorithm with the Manhattan Heuristic
- Num5: Have the Entity use Astar Algorithm with the Euclidian Heuristic
- Num6: Have the Entity use Astar Algorithm with the Squared Euclidian Heuristic
- Num7: Create a room based graph
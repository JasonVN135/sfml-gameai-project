# Homework 2

This project is a SFML-based application designed to demonstrate Steering Behaviors. 
Below is an overview of the structure and instructions for compilation and execution. Addition descriptions
of the project structure can be found below.

---

## Requirements

- Ubuntu 20.04
- SFML library installed (version 2.6.2 or compatible)


## Setup

1. Open a terminal that runs Ubuntu 20.04
2. Run the following command lines to ensure proper installation of required software
    - sudo apt update && sudo apt -y upgrade
    - sudo apt -y install build-essential libsfml-dev


## Compilation

1. Open a desktop terminal
2. Navigate to the directory that contains the project
3. Remove any compiled objects and the executable.
    - make clean
4. Execute the following command lines in the terminal
    - make
    - ./main

## Notes on running
- Option Num2 can technially be run before Num1, though the AI won't do anything with proper information
- Num2 can be run after Num1, but performance is dictate by how long the Num1's Monster has been running for and the variety of information it has gained.

---

## Project Structure

### Source Files

- main.cpp: The entry point of the program. Its sole purpose is to call the main loop in Game.cpp and exit when requested.
- Game.cpp: Handles the main game loop and manages the spawning of Entites and creation of the Graph. Handles inputs from the users to determine which graph to display and which search algorithm to use.

- AIs:
    - Entity.cpp: The entity class that acts as the base user of DecisionTree
    - Monster:cpp: The class that utilizes the BecisionTree. Fills out logs of its state to a csv file for the 
    - LearningMonster.cpp: The class that reads the logs recorded by Monster.cpp, constructs a DecisionTree based on it, and acts on the DecisionTree it constructed
- Structures
    - DecisionTree.cpp: Holds node functionality for creating a decisionTree
        - Action: The Action Node that the Entity will perform
        - Decision: An Abstract class to handle which direction to navigate down a tree
        - BoolDecison: A Decision that decides when its condition returns true or false
        - FloatDecision: A Decision that decides based on the condition value falling between two predefined values.
    - BehaviorTree.cpp: Holds node functionality for creating a behaviorTree
        - BehaviorStatus struct: The Status of a Behavior Node's tick
        - ActionNode: The Action the Entity will perform
        - CompositeNode: AN Abstract class to hold the various compsite nodes base functionality
        - SequenceNode: Perform its children nodes in order.
        - SelectorNode: Returns the status of the first successful or running node, stops looking at further nodes if successful or running.
        - ParallelNode: Runs its children at the same time, storing failure and success conditions.


### Header Files
- Each .cpp file (except main.cpp) has a corresponding .h file that defines its classes and functions
- SteeringOutput, Kinematic, and VelocityMatchStruct are structs used to hold specific data.

### Inputs
- Num0: Create a single Entity with a set DecisionTree
- Num1: Create a single Entity with a set DecisionTree, and a Monster with a set BehaviorTree
- Num2: Create a single Entity with a set DecisionTree, and a LearningMonster with a logs from the Monster from Num1
- Num3: Create a single Entity with a set DecisionTree, and a LearningMonster with set logs from a previous Monster
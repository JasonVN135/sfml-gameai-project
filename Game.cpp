#include "Game.h"


Game::Game() : window(sf::VideoMode(1000, 1000), "SFML Window") {


    arriveDistance = 10.0;
    slowDistance = 30.0;

    //tileSize = 30;

    //graph = new Graph("DataFiles/vertices.csv", "DataFiles/edges.csv", false);

    searchType = DIJKSTRA;



    // Initialize the steering behavior
    spawnEntity(50, 50);
    currentSteeringType = ARRIVE_AND_ALIGN;
    setSteeringBehavior(currentSteeringType);

    
}


Game::~Game() {
    for (auto breadcrumb : waterBreadcrumbs) {
        delete breadcrumb;
    }
    for (auto entity : entities) {
        delete entity;
    }
}

Game& Game::getInstance() {
    static Game instance;
    return instance;
}


void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            }
        }
        
        if (event.type == sf::Event::KeyPressed) {
            // Preset Graph
            if (event.key.code == sf::Keyboard::Num0) {
                
            }
            else if (event.key.code == sf::Keyboard::Num1) {
                
            }
            else if (event.key.code == sf::Keyboard::Num2) {
               
            }
            else if (event.key.code == sf::Keyboard::Num3) {

            }
            else if (event.key.code == sf::Keyboard::Num4) {
 
            }
            else if (event.key.code == sf::Keyboard::Num5) {

            }
            else if (event.key.code == sf::Keyboard::Num6) {
               
            }
            else if (event.key.code == sf::Keyboard::Num7) {
                
            }
          
        }

        /*
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            printf("%d %d\n", sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
            sf::Vector2i mousePosition = sf::Mouse::getPosition();
            addNavNode(mousePosition);
        }*/
    }
}


void Game::update(float deltaTime) {

    if (currentSteeringType == VELOCITY) {
        sendVelocityMatch(deltaTime);
    }

    if (entities.size() == 0) {
        spawnEntity(0, 0);
    }

    for (unsigned int i = 0; i < entities.size(); i++) {
        Entity* entity = entities.at(i);
        entity->update(deltaTime);
    }
}


void Game::render() {

    // Clear the window 
    window.clear(sf::Color::White);

    // draw everything here
    for (auto breadcrumb : waterBreadcrumbs) {
        breadcrumb->render(window);
    }

    for (auto entity : entities) {
        entity->render(window);
    }

    // end the current frame
    window.display();
}


void Game::run() {

    // Game loop
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        render();
    }

}

std::vector<Entity*> Game::getEntities() {
    return entities;
}

void Game::spawnEntity(float x, float y) {

    // Load in the enemy
    Entity *newEntity = new Entity(entityCount, "Assets/low_res-sprite.png", sf::Vector2f(x, y));
    entityCount += 1;
    entities.push_back(newEntity);
}

void Game::checkOutOfBounds() {

    for (auto entity : entities) {
        sf::Vector2f entityPos = entity->getKinematic().position;
        if (entity->getKinematic().position.x < -5) {
            entity->setPosition(sf::Vector2f(800, entityPos.y));
        }
        else if (entity->getKinematic().position.x > 805) {
            entity->setPosition(sf::Vector2f(0, entityPos.y));
        }
        if (entity->getKinematic().position.y < -5) {
            entity->setPosition(sf::Vector2f(entityPos.x, 600));
        }
        else if (entity->getKinematic().position.y > 605) {
            entity->setPosition(sf::Vector2f(entityPos.x, 0));
        }
    }
}

void Game::setSteeringBehavior(STEERING_TYPE steeringChoice) {

    currentSteeringType = steeringChoice;

    if (steeringChoice == POS_AND_ORI) {
        for (auto entity : entities) {
            entity->removeBreadcrumb();
            entity->clearSteeringBehaviors();
            entity->addSteeringBehavior(std::make_unique<PositionMatching>(20, 0.1));
            entity->addSteeringBehavior(std::make_unique<OrientationMatching>(M_PI * .1, 0.1));
        }
            
    }
    
    else if (steeringChoice == VELOCITY) {
        velocityStruct.timeLeft = 1;
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosFloat = sf::Vector2f((float) mousePos.x, (float) mousePos.y);
        velocityStruct.prevLoc = mousePosFloat;
        velocityStruct.nextLoc = mousePosFloat;
        for (auto entity : entities) {
            entity->removeBreadcrumb();
            entity->addSteeringBehavior(std::make_unique<VelocityMatching>(50.0, 0.1));
            entity->addSteeringBehavior(std::make_unique<RotationMatching>(M_PI / 8, 0.1));
        }
    }
    else if (steeringChoice == ARRIVE_AND_ALIGN) {
        for (auto entity : entities) {
            entity->removeBreadcrumb();
            entity->clearSteeringBehaviors();
            entity->addSteeringBehavior(std::make_unique<Arrive>(15, 0.1, arriveDistance, slowDistance));
            entity->addSteeringBehavior(std::make_unique<Align>(M_PI / 6, 0.1, M_PI / 32, M_PI / 8));
        }
    }
    else if (steeringChoice == ARRIVE_AND_ALIGN_2) {
        for (auto entity : entities) {
            entity->removeBreadcrumb();
            entity->clearSteeringBehaviors();
            entity->addSteeringBehavior(std::make_unique<Arrive>(32, 0.1, arriveDistance, slowDistance * 2));
            entity->addSteeringBehavior(std::make_unique<Align>(M_PI / 3, 0.1, M_PI / 32, M_PI * .8));
        }
    }
    else if (steeringChoice == WANDER) {
        for (auto entity : entities) {
            entity->removeBreadcrumb();
            entity->clearSteeringBehaviors();
            entity->addSteeringBehavior(std::make_unique<Wander>(50, 15, 0.01, 0, 8, M_PI / 16, 0.1, M_PI / 32, M_PI / 4));
        }
    }
    else if (steeringChoice == WANDER_2) {
        for (auto entity : entities) {
            entity->removeBreadcrumb();
            entity->clearSteeringBehaviors();
            entity->addSteeringBehavior(std::make_unique<Wander>(50, 40, 0.01, 0, 8, M_PI / 16, 0.1, M_PI / 32, M_PI / 4));
        }
    }
    else if (steeringChoice == FLOCKING) {
        if (!entities.empty()) {
            // Delete all other entities
            for (size_t i = 0; i < entities.size(); ++i) {
                delete entities[i]; // Free memory
            }

            // Resize the vector
            entities.clear();
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(200, 500);

        for (int i = 0; i < 30; i++) {
            spawnEntity(dist(gen), dist(gen));
        }

        for (auto entity : entities) {
            entity->removeBreadcrumb();
            entity->clearSteeringBehaviors();
            entity->addSteeringBehavior(std::make_unique<Arrive>(8, 0.1, arriveDistance, slowDistance));
            entity->addSteeringBehavior(std::make_unique<Align>(M_PI / 3, 0.1, M_PI / 32, M_PI * .8));
            entity->addSteeringBehavior(std::make_unique<Flocking>(.3, 100, 20, .01, .7, .5, 12));
        }
    }

}


void Game::sendVelocityMatch(float deltaTime) {
    
    if (velocityStruct.timeLeft > 0) {
        velocityStruct.timeLeft -= deltaTime;
    }
    else {
        velocityStruct.timeLeft = 1;
        for (auto entity : entities) {
            Kinematic kin;
            kin.velocity = velocityStruct.nextLoc - velocityStruct.prevLoc;
            kin.rotation = M_PI;
            entity->setTargetKinematic(kin);
            velocityStruct.prevLoc = velocityStruct.nextLoc;
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePosFloat = sf::Vector2f((float) mousePos.x, (float) mousePos.y);
            velocityStruct.nextLoc = mousePosFloat;
        }
    }

}


Breadcrumb* Game::getNearestWaterBreadcrumb(sf::Vector2f currentPos) {


    if (waterBreadcrumbs.size() <= 0) {
        return nullptr;
    }

    Breadcrumb* closestBreadcrumb = waterBreadcrumbs.at(0);
    float shortestDist = VectorUtils::vector2Length(currentPos - waterBreadcrumbs.at(0)->getPosition());

    for (auto breadcrumb : waterBreadcrumbs) {
        if (VectorUtils::vector2Length(currentPos -breadcrumb->getPosition()) < shortestDist) {
            closestBreadcrumb = breadcrumb;
        }
    }
    return closestBreadcrumb;
}
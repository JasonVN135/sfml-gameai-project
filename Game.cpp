#include "Game.h"


Game::Game() : window(sf::VideoMode(1000, 800), "SFML Window") {


    arriveDistance = 10.0;
    slowDistance = 50.0;

    // Override Already existing Data
    std::ofstream clearFile("DataFiles/monsterData.csv", std::ios::trunc);
    isHeaderWritten = false;

    // Initialize the steering behavior
    spawnEntity(300, 300);
    currentSteeringType = ARRIVE_AND_ALIGN;

    waters.push_back(new Breadcrumb(sf::Vector2f(180, 180), 10));
    waters.push_back(new Breadcrumb(sf::Vector2f(840, 450), 10));
    waters.push_back(new Breadcrumb(sf::Vector2f(270, 690), 10));
    waters.push_back(new Breadcrumb(sf::Vector2f(560, 370), 10));
    waters.push_back(new Breadcrumb(sf::Vector2f(720, 220), 10));
    waters.push_back(new Breadcrumb(sf::Vector2f(360, 750), 10));

    for (auto water : waters) {
        water->setFillColor(sf::Color(68, 86, 240, 100));
    }
}


Game::~Game() {
    for (auto breadcrumb : waters) {
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

        
        if (event.type == sf::Event::KeyPressed) {
            // Entity Decision Tree
            if (event.key.code == sf::Keyboard::Num0) {
                for (size_t i = 0; i < entities.size(); ++i) {
                    delete entities[i]; // Free memory
                }
                for (size_t i = 0; i < monsters.size(); ++i) {
                    delete monsters[i]; // Free memory
                }
                for (size_t i = 0; i < learningMonsters.size(); ++i) {
                    delete learningMonsters[i]; // Free memory
                }
                entities.clear();
                monsters.clear();
                learningMonsters.clear();
                spawnEntity(300, 300);
            }
            // Monster Behavior Tree
            else if (event.key.code == sf::Keyboard::Num1) {
                for (size_t i = 0; i < entities.size(); ++i) {
                    delete entities[i]; // Free memory
                }
                for (size_t i = 0; i < monsters.size(); ++i) {
                    delete monsters[i]; // Free memory
                }
                for (size_t i = 0; i < learningMonsters.size(); ++i) {
                    delete learningMonsters[i]; // Free memory
                }
                entities.clear();
                monsters.clear();
                learningMonsters.clear();
                spawnEntity(300, 300);
                spawnMonster(800, 600);
            }
            // Learning Monster Decision Tree
            else if (event.key.code == sf::Keyboard::Num2) {
                for (size_t i = 0; i < entities.size(); ++i) {
                    delete entities[i]; // Free memory
                }
                for (size_t i = 0; i < monsters.size(); ++i) {
                    delete monsters[i]; // Free memory
                }
                for (size_t i = 0; i < learningMonsters.size(); ++i) {
                    delete learningMonsters[i]; // Free memory
                }
                entities.clear();
                monsters.clear();
                learningMonsters.clear();
                spawnEntity(300, 300);
                spawnLearningMonster(900, 100, "DataFiles/monsterData.csv");
            }
            // Learning Monster Decision Tree Preset Data
            else if (event.key.code == sf::Keyboard::Num3) {
                for (size_t i = 0; i < entities.size(); ++i) {
                    delete entities[i]; // Free memory
                }
                for (size_t i = 0; i < monsters.size(); ++i) {
                    delete monsters[i]; // Free memory
                }
                for (size_t i = 0; i < learningMonsters.size(); ++i) {
                    delete learningMonsters[i]; // Free memory
                }
                entities.clear();
                monsters.clear();
                learningMonsters.clear();
                spawnEntity(300, 300);
                spawnLearningMonster(900, 100, "DataFiles/setMonsterData.csv");
            }
        }
    }
}


void Game::update(float deltaTime) {

    checkOutOfBounds();

    if (entities.size() == 0) {
        spawnEntity(0, 0);
    }

    for (unsigned int i = 0; i < entities.size(); i++) {
        Entity* entity = entities.at(i);
        entity->update(deltaTime);
    }

    for (unsigned int i = 0; i < monsters.size(); i++) {
        Monster* monster = monsters.at(i);
        monster->update(deltaTime);
    }

    for (unsigned int i = 0; i < learningMonsters.size(); i++) {
        LearningMonster* learningMonster = learningMonsters.at(i);
        learningMonster->update(deltaTime);
    }
}


void Game::render() {

    // Clear the window 
    window.clear(sf::Color::White);

    // draw everything here
    for (auto breadcrumb : waters) {
        breadcrumb->render(window);
    }

    for (auto entity : entities) {
        entity->render(window);
    }

    for (auto monster : monsters) {
        monster->render(window);
    }

    for (auto learningMonster : learningMonsters) {
        learningMonster->render(window);
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

std::vector<Monster*> Game::getMonsters() {
    return monsters;
}

std::vector<LearningMonster*> Game::getLearningMonsters() {
    return learningMonsters;
}

void Game::spawnEntity(float x, float y) {

    // Load in the enemy
    Entity *newEntity = new Entity(entityCount, "Assets/low_res-sprite.png", sf::Vector2f(x, y));
    entityCount += 1;
    entities.push_back(newEntity);
}

void Game::spawnMonster(float x, float y) {

    Monster *newMonster = new Monster(monsterCount, "Assets/monster-sprite.png", sf::Vector2f(x, y), 200);
    monsterCount += 1;
    monsters.push_back(newMonster);
}

void Game::spawnLearningMonster(float x, float y, std::string dataFile) {

    LearningMonster *newMonster = new LearningMonster(learningMonsterCount, "Assets/monster-sprite.png", sf::Vector2f(x, y), 200, dataFile);
    learningMonsterCount += 1;
    learningMonsters.push_back(newMonster);
}

void Game::checkOutOfBounds() {

    for (auto entity : entities) {
        sf::Vector2f entityPos = entity->getKinematic().position;
        if (entity->getKinematic().position.x < -5) {
            entity->setPosition(sf::Vector2f(1000, entityPos.y));
        }
        else if (entity->getKinematic().position.x > 1005) {
            entity->setPosition(sf::Vector2f(0, entityPos.y));
        }
        if (entity->getKinematic().position.y < -5) {
            entity->setPosition(sf::Vector2f(entityPos.x, 800));
        }
        else if (entity->getKinematic().position.y > 805) {
            entity->setPosition(sf::Vector2f(entityPos.x, 0));
        }
    }
    for (auto monster : monsters) {
        sf::Vector2f monsterPos = monster->getKinematic().position;
        if (monster->getKinematic().position.x < -5) {
            monster->setPosition(sf::Vector2f(800, monsterPos.y));
        }
        else if (monster->getKinematic().position.x > 1005) {
            monster->setPosition(sf::Vector2f(0, monsterPos.y));
        }
        if (monster->getKinematic().position.y < -5) {
            monster->setPosition(sf::Vector2f(monsterPos.x, 800));
        }
        else if (monster->getKinematic().position.y > 805) {
            monster->setPosition(sf::Vector2f(monsterPos.x, 0));
        }
    }
    for (auto monster : learningMonsters) {
        sf::Vector2f monsterPos = monster->getKinematic().position;
        if (monster->getKinematic().position.x < -5) {
            monster->setPosition(sf::Vector2f(800, monsterPos.y));
        }
        else if (monster->getKinematic().position.x > 1005) {
            monster->setPosition(sf::Vector2f(0, monsterPos.y));
        }
        if (monster->getKinematic().position.y < -5) {
            monster->setPosition(sf::Vector2f(monsterPos.x, 800));
        }
        else if (monster->getKinematic().position.y > 805) {
            monster->setPosition(sf::Vector2f(monsterPos.x, 0));
        }
    }
}

void Game::setSteeringBehavior(STEERING_TYPE steeringChoice) {

    currentSteeringType = steeringChoice;

    if (steeringChoice == POS_AND_ORI) {
        for (auto entity : entities) {
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
            entity->addSteeringBehavior(std::make_unique<VelocityMatching>(50.0, 0.1));
            entity->addSteeringBehavior(std::make_unique<RotationMatching>(M_PI / 8, 0.1));
        }
    }
    else if (steeringChoice == ARRIVE_AND_ALIGN) {
        for (auto entity : entities) {
            entity->clearSteeringBehaviors();
            entity->addSteeringBehavior(std::make_unique<Arrive>(15, 0.1, arriveDistance, slowDistance));
            entity->addSteeringBehavior(std::make_unique<Align>(M_PI / 6, 0.1, M_PI / 32, M_PI / 8));
        }
    }
    else if (steeringChoice == ARRIVE_AND_ALIGN_2) {
        for (auto entity : entities) {
            entity->clearSteeringBehaviors();
            entity->addSteeringBehavior(std::make_unique<Arrive>(32, 0.1, arriveDistance, slowDistance * 2));
            entity->addSteeringBehavior(std::make_unique<Align>(M_PI / 3, 0.1, M_PI / 32, M_PI * .8));
        }
    }
    else if (steeringChoice == WANDER) {
        for (auto entity : entities) {
            entity->clearSteeringBehaviors();
            entity->addSteeringBehavior(std::make_unique<Wander>(50, 15, 0.01, 0, 8, M_PI / 16, 0.1, M_PI / 32, M_PI / 4));
        }
    }
    else if (steeringChoice == WANDER_2) {
        for (auto entity : entities) {
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
            entity->clearSteeringBehaviors();
            entity->addSteeringBehavior(std::make_unique<Arrive>(8, 0.1, arriveDistance, slowDistance));
            entity->addSteeringBehavior(std::make_unique<Align>(M_PI / 3, 0.1, M_PI / 32, M_PI * .8));
            entity->addSteeringBehavior(std::make_unique<Flocking>(.3, 100, 20, .01, .7, .5, 12));
        }
    }

}

Breadcrumb* Game::getNearestWaterBreadcrumb(sf::Vector2f currentPos) {

    if (waters.size() <= 0) {
        return nullptr;
    }

    Breadcrumb* closestBreadcrumb = waters.at(0);
    float shortestDist = VectorUtils::vector2Length(currentPos - waters.at(0)->getPosition());

    for (auto breadcrumb : waters) {
        float dist = VectorUtils::vector2Length(currentPos -breadcrumb->getPosition());
        if (dist < shortestDist) {
            closestBreadcrumb = breadcrumb;
            shortestDist = dist;
        }
    }
    return closestBreadcrumb;
}
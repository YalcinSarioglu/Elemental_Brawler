#include "GameManager.h"

void GameManager::init() {
    // Initialization logic here
}

void GameManager::update(float deltaTime) {
    for (auto& entity : entities) {
        entity->update(deltaTime);
    }
}

void GameManager::draw() const {
    for (const auto& entity : entities) {
        entity->draw();
    }
}

void GameManager::addEntity(std::shared_ptr<Entity> entity) {
    entities.push_back(entity);
}

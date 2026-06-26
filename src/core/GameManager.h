#pragma once
#include <vector>
#include <memory>
#include "entities/Entity.h"

class GameManager {
private:
    std::vector<std::shared_ptr<Entity>> entities;

public:
    void init();
    void run();
    void update(float deltaTime);
    void draw() const;
    void addEntity(std::shared_ptr<Entity> entity);
};

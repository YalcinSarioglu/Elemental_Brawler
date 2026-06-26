#include "core/GameManager.h"
#include "raymath.h"
#include "raylib.h"
#include "entities/FighterBall.h"
#include <cmath>

void GameManager::init() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Elemental Brawler");
    SetTargetFPS(60);

    // Add initial balls
    auto fireBall = std::make_shared<FighterBall>(Vector2{200, 300}, 20.0f, RED, Element::FIRE);
    auto iceBall = std::make_shared<FighterBall>(Vector2{600, 300}, 25.0f, BLUE, Element::ICE);

    float speed = 400.0f;
    float angle1 = GetRandomValue(0, 360) * DEG2RAD;
    fireBall->velocity = { cosf(angle1) * speed, sinf(angle1) * speed };
    
    float angle2 = GetRandomValue(0, 360) * DEG2RAD;
    iceBall->velocity = { cosf(angle2) * speed, sinf(angle2) * speed };

    addEntity(fireBall);
    addEntity(iceBall);
}

void GameManager::run() {
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        update(deltaTime);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        draw();
        
        DrawText("Elemental Brawler", 10, 10, 20, DARKGRAY);

        EndDrawing();
    }
    CloseWindow();
}

void GameManager::update(float deltaTime) {
    // 1. Update all entities
    for (auto& entity : entities) {
        entity->update(deltaTime);
    }

    // 2. Resolve collisions
    for (size_t i = 0; i < entities.size(); ++i) {
        for (size_t j = i + 1; j < entities.size(); ++j) {
            auto& a = entities[i];
            auto& b = entities[j];

            float dist = Vector2Distance(a->position, b->position);
            float min_dist = a->radius + b->radius;

            if (dist < min_dist && dist > 0.0f) {
                // Overlapping! Push them apart
                float overlap = min_dist - dist;
                Vector2 normal = Vector2Normalize(Vector2Subtract(a->position, b->position));

                // Push proportional to radius (or simply equal push)
                Vector2 pushA = Vector2Scale(normal, overlap * 0.5f);
                Vector2 pushB = Vector2Scale(normal, -overlap * 0.5f);

                a->position = Vector2Add(a->position, pushA);
                b->position = Vector2Add(b->position, pushB);

                // Simple elastic collision (swap velocities along the normal)
                Vector2 relativeVelocity = Vector2Subtract(a->velocity, b->velocity);
                float speed = Vector2DotProduct(relativeVelocity, normal);

                if (speed < 0) {
                    // They are moving towards each other
                    const float restitution = 1.0f; // 1.0 means perfectly elastic (no speed loss)
                    float impulse = -(1.0f + restitution) * speed / 2.0f; // Assuming equal mass

                    Vector2 impulseVec = Vector2Scale(normal, impulse);
                    a->velocity = Vector2Add(a->velocity, impulseVec);
                    b->velocity = Vector2Subtract(b->velocity, impulseVec);
                }
            }
        }
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

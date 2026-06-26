#include "raylib.h"
#include "GameManager.h"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Elemental Brawler");

    SetTargetFPS(60);

    GameManager gameManager;
    gameManager.init();

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        gameManager.update(deltaTime);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        gameManager.draw();

        DrawText("Elemental Brawler Starting...", 10, 10, 20, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

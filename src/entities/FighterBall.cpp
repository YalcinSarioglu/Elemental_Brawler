#include "entities/FighterBall.h"
#include <algorithm>

FighterBall::FighterBall(Vector2 pos, float rad, Color col, Element element)
    : Entity(pos, rad, col), health(100.0f), baseSpeed(200.0f),
      currentElement(element) {}

void FighterBall::update(float deltaTime) {
  // Update position
  position.x += velocity.x * deltaTime;
  position.y += velocity.y * deltaTime;

  // Bouncing logic (hardcoded bounds 800x600 for now)
  const float screenWidth = 800.0f;
  const float screenHeight = 600.0f;
  const float bounciness = 1.0f; // 1.0 means perfectly elastic (no speed loss)

  // Floor collision
  if (position.y + radius >= screenHeight) {
    position.y = screenHeight - radius;
    velocity.y *= -bounciness;
  }
  // Ceiling collision
  else if (position.y - radius <= 0) {
    position.y = radius;
    velocity.y *= -bounciness;
  }

  // Right wall
  if (position.x + radius >= screenWidth) {
    position.x = screenWidth - radius;
    velocity.x *= -bounciness;
  }
  // Left wall
  else if (position.x - radius <= 0) {
    position.x = radius;
    velocity.x *= -bounciness;
  }
}

void FighterBall::draw() const {
  DrawCircleV(position, radius, color);
  // Draw a small outline to make it pop
  DrawCircleLines(position.x, position.y, radius, BLACK);

  // Draw Health Bar
  int barWidth = 40;
  int barHeight = 6;
  int barX = position.x - barWidth / 2;
  int barY = position.y - radius - 15;

  // Background (red)
  DrawRectangle(barX, barY, barWidth, barHeight, RED);

  // Foreground (green) based on health percentage (assuming max health is 100)
  float healthPercent = std::max(0.0f, health) / 100.0f;
  DrawRectangle(barX, barY, barWidth * healthPercent, barHeight, GREEN);

  // Outline
  DrawRectangleLines(barX, barY, barWidth, barHeight, BLACK);
}

void FighterBall::applyDamage(float amount) {
  health -= amount;
  if (health < 0)
    health = 0;
}

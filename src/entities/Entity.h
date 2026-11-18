#pragma once
#include "raylib.h"

class Entity
{
public:
  Vector2 position;
  Vector2 velocity;
  float radius;
  Color color;

  Entity(Vector2 pos, float rad, Color col);
  virtual ~Entity() = default;

  virtual void update(float deltaTime) = 0;
  virtual void draw() const = 0;

  // Dead entities get removed by GameManager. Plain entities never die.
  virtual bool isAlive() const { return true; }
};

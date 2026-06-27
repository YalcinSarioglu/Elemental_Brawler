#pragma once
#include "entities/Entity.h"

enum class Element { FIRE, ICE, EARTH, WIND, NEUTRAL };

class FighterBall : public Entity {
public:
  float health;
  float baseSpeed;
  Element currentElement;

  FighterBall(Vector2 pos, float rad, Color col, Element element);
  ~FighterBall() override = default;

  void update(float deltaTime) override;
  void draw() const override;

  // Combat methods to be implemented later
  void applyDamage(float amount);
};

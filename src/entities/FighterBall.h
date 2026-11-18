#pragma once
#include "entities/Entity.h"
#include "weapons/Weapon.h"
#include <memory>

enum class Element
{
  FIRE,
  ICE,
  EARTH,
  WIND,
  NEUTRAL
};

constexpr const char *toString(Element e)
{
  switch (e)
  {
  case Element::FIRE:
    return "FIRE";
  case Element::ICE:
    return "ICE";
  case Element::EARTH:
    return "EARTH";
  case Element::WIND:
    return "WIND";
  case Element::NEUTRAL:
    return "NEUTRAL";
  }
  return "UNKNOWN";
}

class FighterBall : public Entity
{
public:
  float health;
  float baseSpeed;
  Element currentElement;
  int id = 0;                     // unique per round, set by GameManager
  std::unique_ptr<Weapon> weapon; // owned by this ball; may be empty (no weapon)

  FighterBall(Vector2 pos, float rad, Color col, Element element);
  ~FighterBall() override = default;

  void update(float deltaTime) override;
  void draw() const override;
  bool isAlive() const override { return health > 0.0f; }

  void updateWeapon(float deltaTime, WeaponContext &ctx);
  void applyDamage(float amount);
};

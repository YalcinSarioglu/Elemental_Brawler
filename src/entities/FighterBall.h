#pragma once
#include "entities/Entity.h"
#include "entities/Hit.h"
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

// Active status effects. Plain data: a timer > 0 means the effect is active.
struct StatusEffects
{
  float burnTimer = 0.0f;
  float chillTimer = 0.0f;
};

class FighterBall : public Entity
{
public:
  float health;
  float baseSpeed;
  Element currentElement;
  int id = 0;                     // unique per round, set by GameManager
  std::unique_ptr<Weapon> weapon; // owned by this ball; may be empty (no weapon)
  StatusEffects effects;

  FighterBall(Vector2 pos, float rad, Color col, Element element);
  ~FighterBall() override = default;

  void update(float deltaTime) override;
  void draw() const override;
  bool isAlive() const override { return health > 0.0f; }

  void updateWeapon(float deltaTime, WeaponContext &ctx);
  void applyDamage(float amount);
  void applyHit(const Hit &hit);

  bool isBurning() const { return effects.burnTimer > 0.0f; }
  bool isChilled() const { return effects.chillTimer > 0.0f; }

  // 1.0 = normal. Below 1.0 while chilled. Used for movement AND attacks.
  float slowFactor() const { return isChilled() ? chillSlowFactor : 1.0f; }

  static constexpr float burnDamagePerSecond = 1.5f;
  static constexpr float chillSlowFactor = 0.5f;

private:
  void tickEffects(float deltaTime);
};

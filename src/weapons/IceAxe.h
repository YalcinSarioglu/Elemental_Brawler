#pragma once
#include "raylib.h"
#include "weapons/Weapon.h"

// Melee weapon: an axe that orbits around its owner and whacks any enemy it
// touches, knocking them away.
class IceAxe : public Weapon
{
public:
  void update(FighterBall &owner, float deltaTime, WeaponContext &ctx) override;
  void draw(const FighterBall &owner) const override;

private:
  static constexpr float spinSpeed = 5.0f;    // radians per second
  static constexpr float handleLength = 32.0f; // gap between body edge and axe head
  static constexpr float headRadius = 13.0f;
  static constexpr float damage = 22.0f;
  static constexpr float hitCooldown = 0.4f; // seconds, so one swing = one hit

  float angle = 0.0f;
  float cooldownTimer = 0.0f;

  Vector2 headPosition(const FighterBall &owner) const;
};

#pragma once
#include "raylib.h"
#include "weapons/Weapon.h"

// Ranged weapon: shoots a fireball at the nearest enemy every `cooldown` seconds.
class FireStaff : public Weapon
{
public:
  void update(FighterBall &owner, float deltaTime, WeaponContext &ctx) override;
  void draw(const FighterBall &owner) const override;

private:
  static constexpr float cooldown = 1.0f;          // seconds between shots
  static constexpr float projectileSpeed = 450.0f; // pixels per second
  static constexpr float projectileRadius = 6.0f;
  static constexpr float damage = 6.0f;
  static constexpr float lifetime = 2.0f; // seconds before a fireball fizzles out

  float timer = cooldown;
  Vector2 aimDir{1.0f, 0.0f}; // last aim direction, also used to draw the staff
};

#pragma once
#include <memory>
#include <vector>

// Forward declarations: Weapon only needs to know these types exist.
// Keeps headers light and avoids circular includes with FighterBall.h.
class FighterBall;
class Projectile;

// What a weapon is allowed to see and do during its update.
struct WeaponContext
{
  const std::vector<std::shared_ptr<FighterBall>> &fighters; // read-only view
  std::vector<Projectile> &spawnQueue; // new projectiles go here, not into the live list
};

// Strategy interface: every weapon type implements its own behaviour.
// A FighterBall owns exactly one Weapon (composition, not inheritance).
class Weapon
{
public:
  virtual ~Weapon() = default;

  virtual void update(FighterBall &owner, float deltaTime, WeaponContext &ctx) = 0;
  virtual void draw(const FighterBall &owner) const = 0;
};

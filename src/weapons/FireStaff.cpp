#include "weapons/FireStaff.h"
#include "entities/FighterBall.h"
#include "entities/Projectile.h"
#include "raymath.h"

namespace
{
// Closest living fighter that is not the owner, or nullptr if none.
const FighterBall *findNearestEnemy(const FighterBall &owner,
                                    const std::vector<std::shared_ptr<FighterBall>> &fighters)
{
  const FighterBall *nearest = nullptr;
  float bestDistSq = 0.0f;

  for (const auto &f : fighters)
  {
    if (f->id == owner.id || !f->isAlive())
    {
      continue;
    }
    // Squared distance: same ordering as real distance, but no sqrt
    const float distSq = Vector2DistanceSqr(owner.position, f->position);
    if (nearest == nullptr || distSq < bestDistSq)
    {
      nearest = f.get();
      bestDistSq = distSq;
    }
  }
  return nearest;
}
} // namespace

void FireStaff::update(FighterBall &owner, float deltaTime, WeaponContext &ctx)
{
  const FighterBall *target = findNearestEnemy(owner, ctx.fighters);
  if (target == nullptr)
  {
    return;
  }

  aimDir = Vector2Normalize(Vector2Subtract(target->position, owner.position));

  timer -= deltaTime;
  if (timer > 0.0f)
  {
    return;
  }
  timer = cooldown;

  // Spawn just outside the owner's body so the shot never starts inside it
  const Vector2 spawnPos =
      Vector2Add(owner.position, Vector2Scale(aimDir, owner.radius + projectileRadius + 2.0f));
  const Vector2 velocity = Vector2Scale(aimDir, projectileSpeed);

  ctx.spawnQueue.emplace_back(spawnPos, velocity, projectileRadius, ORANGE, damage, owner.id,
                              lifetime);
}

void FireStaff::draw(const FighterBall &owner) const
{
  // A short staff pointing at the current target, with a glowing tip
  const Vector2 start = Vector2Add(owner.position, Vector2Scale(aimDir, owner.radius * 0.6f));
  const Vector2 tip = Vector2Add(owner.position, Vector2Scale(aimDir, owner.radius + 12.0f));
  DrawLineEx(start, tip, 4.0f, BROWN);
  DrawCircleV(tip, 4.0f, ORANGE);
}

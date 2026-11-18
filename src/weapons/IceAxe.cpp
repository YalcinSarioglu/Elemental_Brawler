#include "weapons/IceAxe.h"
#include "entities/FighterBall.h"
#include "raymath.h"
#include <cmath>

Vector2 IceAxe::headPosition(const FighterBall &owner) const
{
  const float dist = owner.radius + handleLength;
  return {owner.position.x + cosf(angle) * dist, owner.position.y + sinf(angle) * dist};
}

void IceAxe::update(FighterBall &owner, float deltaTime, WeaponContext &ctx)
{
  // Keep the angle in [0, 2*pi) so the float never grows without bound
  angle = fmodf(angle + spinSpeed * deltaTime, 2.0f * PI);

  if (cooldownTimer > 0.0f)
  {
    cooldownTimer -= deltaTime;
    return;
  }

  const Vector2 head = headPosition(owner);

  for (const auto &enemy : ctx.fighters)
  {
    if (enemy->id == owner.id || !enemy->isAlive())
    {
      continue;
    }

    if (CheckCollisionCircles(head, headRadius, enemy->position, enemy->radius))
    {
      enemy->applyDamage(damage);

      // Knockback: send the enemy flying away from the axe head, same speed.
      // Keeping the speed (not adding to it) stops balls from speeding up forever.
      const Vector2 away = Vector2Normalize(Vector2Subtract(enemy->position, head));
      const float speed = Vector2Length(enemy->velocity);
      enemy->velocity = Vector2Scale(away, speed);

      cooldownTimer = hitCooldown;
      break; // one hit per swing
    }
  }
}

void IceAxe::draw(const FighterBall &owner) const
{
  const Vector2 head = headPosition(owner);
  const Vector2 dir = {cosf(angle), sinf(angle)};
  const Vector2 handleStart = Vector2Add(owner.position, Vector2Scale(dir, owner.radius * 0.8f));

  DrawLineEx(handleStart, head, 4.0f, DARKBROWN);  // handle
  DrawCircleV(head, headRadius, SKYBLUE);          // icy head
  DrawCircleLinesV(head, headRadius, DARKBLUE);    // outline
  DrawCircleV(head, headRadius * 0.4f, RAYWHITE);  // shine

  // Faint swing trail while it's ready to hit, so you can read the timing
  if (cooldownTimer <= 0.0f)
  {
    DrawCircleLinesV(owner.position, owner.radius + handleLength, Fade(SKYBLUE, 0.3f));
  }
}

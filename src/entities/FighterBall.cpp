#include "entities/FighterBall.h"
#include <algorithm>
#include <cmath>

FighterBall::FighterBall(Vector2 pos, float rad, Color col, Element element)
    : Entity(pos, rad, col), health(100.0f), baseSpeed(200.0f),
      currentElement(element) {}

void FighterBall::update(float deltaTime)
{
  tickEffects(deltaTime);

  // Update position. The velocity itself never changes when chilled: only how
  // far we move this frame. So there's nothing to "restore" when chill ends.
  const float moveScale = slowFactor() * deltaTime;
  position.x += velocity.x * moveScale;
  position.y += velocity.y * moveScale;

  // Bouncing logic (hardcoded bounds 800x600 for now)
  const float screenWidth = 800.0f;
  const float screenHeight = 600.0f;
  const float bounciness = 1.0f; // 1.0 means perfectly elastic (no speed loss)

  // Floor collision
  if (position.y + radius >= screenHeight)
  {
    position.y = screenHeight - radius;
    velocity.y *= -bounciness;
  }
  // Ceiling collision
  else if (position.y - radius <= 0)
  {
    position.y = radius;
    velocity.y *= -bounciness;
  }

  // Right wall
  if (position.x + radius >= screenWidth)
  {
    position.x = screenWidth - radius;
    velocity.x *= -bounciness;
  }
  // Left wall
  else if (position.x - radius <= 0)
  {
    position.x = radius;
    velocity.x *= -bounciness;
  }
}

void FighterBall::draw() const
{
  DrawCircleV(position, radius, color);

  // Chilled: frosty overlay on the body
  if (isChilled())
  {
    DrawCircleV(position, radius, Fade(SKYBLUE, 0.55f));
    DrawCircleLinesV(position, radius + 2.0f, RAYWHITE);
  }

  // Burning: flickering orange rings around the body
  if (isBurning())
  {
    const float flicker = 0.5f + 0.4f * sinf(effects.burnTimer * 30.0f);
    DrawCircleLinesV(position, radius + 3.0f, Fade(ORANGE, flicker));
    DrawCircleLinesV(position, radius + 5.0f, Fade(RED, flicker * 0.7f));
  }

  if (weapon)
  {
    weapon->draw(*this);
  }

  // Draw a small outline to make it pop
  DrawCircleLinesV(position, radius, BLACK);

  // Draw Health Bar (all float, so no implicit narrowing to int)
  const float barWidth = 40.0f;
  const float barHeight = 6.0f;
  const Rectangle bar{position.x - barWidth / 2.0f, position.y - radius - 15.0f,
                      barWidth, barHeight};

  // Background (red)
  DrawRectangleRec(bar, RED);

  // Foreground (green) based on health percentage (assuming max health is 100)
  const float healthPercent = std::max(0.0f, health) / 100.0f;
  Rectangle fill = bar;
  fill.width = barWidth * healthPercent;
  DrawRectangleRec(fill, GREEN);

  // Outline
  DrawRectangleLinesEx(bar, 1.0f, BLACK);
}

void FighterBall::updateWeapon(float deltaTime, WeaponContext &ctx)
{
  if (weapon)
  {
    // Chilled weapons get less time per frame, so cooldowns run slower and
    // the axe spins slower. No weapon needs to know about chill at all.
    weapon->update(*this, deltaTime * slowFactor(), ctx);
  }
}

void FighterBall::applyDamage(float amount)
{
  health -= amount;
  if (health < 0)
    health = 0;
}

void FighterBall::applyHit(const Hit &hit)
{
  applyDamage(hit.damage);

  // Refresh, don't stack: a new hit resets the timer to the longer value.
  // Stacking would let several quick hits pile up and melt the target.
  effects.burnTimer = std::max(effects.burnTimer, hit.burnSeconds);
  effects.chillTimer = std::max(effects.chillTimer, hit.chillSeconds);
}

void FighterBall::tickEffects(float deltaTime)
{
  if (isBurning())
  {
    // Only count the part of this frame the burn was actually active, so the
    // total burn damage is exact (burnSeconds * dps) whatever the frame rate.
    const float activeTime = std::min(deltaTime, effects.burnTimer);
    applyDamage(burnDamagePerSecond * activeTime);
    effects.burnTimer -= activeTime;
  }

  if (isChilled())
  {
    effects.chillTimer = std::max(0.0f, effects.chillTimer - deltaTime);
  }
}

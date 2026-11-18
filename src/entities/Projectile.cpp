#include "entities/Projectile.h"

Projectile::Projectile(Vector2 pos, Vector2 vel, float rad, Color col, float dmg, int owner,
                       float life)
    : Entity(pos, rad, col), damage(dmg), ownerId(owner), lifetime(life)
{
  velocity = vel;
}

void Projectile::update(float deltaTime)
{
  position.x += velocity.x * deltaTime;
  position.y += velocity.y * deltaTime;

  lifetime -= deltaTime;

  const float w = static_cast<float>(GetScreenWidth());
  const float h = static_cast<float>(GetScreenHeight());
  const bool offScreen = position.x < -radius || position.x > w + radius ||
                         position.y < -radius || position.y > h + radius;

  if (lifetime <= 0.0f || offScreen)
  {
    alive = false;
  }
}

void Projectile::draw() const
{
  DrawCircleV(position, radius * 1.8f, Fade(color, 0.3f)); // glow
  DrawCircleV(position, radius, color);                     // body
  DrawCircleV(position, radius * 0.5f, YELLOW);             // hot core
}

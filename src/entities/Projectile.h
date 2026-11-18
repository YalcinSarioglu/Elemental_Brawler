#pragma once
#include "entities/Entity.h"

// A shot fired by a weapon. Flies straight, hits one enemy, then dies.
// Stored by value in GameManager (no heap allocation per shot).
class Projectile : public Entity
{
public:
  Projectile(Vector2 pos, Vector2 vel, float rad, Color col, float dmg, int owner,
             float life);

  void update(float deltaTime) override;
  void draw() const override;
  bool isAlive() const override { return alive; }

  void kill() { alive = false; }
  float getDamage() const { return damage; }
  int getOwnerId() const { return ownerId; }

private:
  float damage;
  int ownerId;    // id of the fighter who fired it (so it can't hit its owner)
  float lifetime; // seconds left
  bool alive = true;
};

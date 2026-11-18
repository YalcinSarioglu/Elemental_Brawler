#pragma once
#include "entities/FighterBall.h"
#include "entities/Projectile.h"
#include <memory>
#include <string>
#include <vector>

enum class GameState
{
  Playing,
  GameOver
};

class GameManager
{
private:
  // One container per type: no runtime type checks needed in the hot loops.
  std::vector<std::shared_ptr<FighterBall>> fighters;
  std::vector<Projectile> projectiles;        // stored by value, contiguous in memory
  std::vector<Projectile> pendingProjectiles; // spawned this frame, merged after updates

  GameState state = GameState::Playing;
  std::string resultText; // e.g. "FIRE WINS!" or "DRAW!"
  int nextFighterId = 0;

  void spawnFighters();
  void addFighter(std::shared_ptr<FighterBall> fighter);

  void updateWeapons(float deltaTime);
  void updateProjectiles(float deltaTime);
  void resolveFighterCollisions();
  void resolveProjectileHits();
  void removeDeadEntities();
  void checkGameOver();
  void drawGameOver() const;

public:
  void init();
  void run();
  void update(float deltaTime);
  void draw() const;
};

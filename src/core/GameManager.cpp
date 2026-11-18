#include "core/GameManager.h"
#include "raylib.h"
#include "raymath.h"
#include "weapons/FireStaff.h"
#include "weapons/IceAxe.h"
#include <cmath>
#include <iterator>

void GameManager::init()
{
  const int screenWidth = 800;
  const int screenHeight = 600;

  InitWindow(screenWidth, screenHeight, "Elemental Brawler");
  SetTargetFPS(60);

  spawnFighters();
}

// (Re)starts a round: clears everything and spawns a fresh set of fighters.
void GameManager::spawnFighters()
{
  fighters.clear();
  projectiles.clear();
  pendingProjectiles.clear();
  nextFighterId = 0;
  state = GameState::Playing;
  resultText.clear();

  // All fighters share one size, so only weapons and effects decide the fight
  const float fighterRadius = 22.0f;

  auto fireBall =
      std::make_shared<FighterBall>(Vector2{200, 300}, fighterRadius, RED, Element::FIRE);
  auto iceBall =
      std::make_shared<FighterBall>(Vector2{600, 300}, fighterRadius, BLUE, Element::ICE);

  // Each element brings its own weapon
  fireBall->weapon = std::make_unique<FireStaff>(); // ranged
  iceBall->weapon = std::make_unique<IceAxe>();     // melee

  const float speed = 400.0f;
  const float angle1 = static_cast<float>(GetRandomValue(0, 360)) * DEG2RAD;
  fireBall->velocity = {cosf(angle1) * speed, sinf(angle1) * speed};

  const float angle2 = static_cast<float>(GetRandomValue(0, 360)) * DEG2RAD;
  iceBall->velocity = {cosf(angle2) * speed, sinf(angle2) * speed};

  addFighter(fireBall);
  addFighter(iceBall);
}

void GameManager::addFighter(std::shared_ptr<FighterBall> fighter)
{
  fighter->id = nextFighterId++;
  fighters.push_back(std::move(fighter));
}

void GameManager::run()
{
  while (!WindowShouldClose())
  {
    float deltaTime = GetFrameTime();
    update(deltaTime);

    BeginDrawing();
    ClearBackground(RAYWHITE);

    draw();

    DrawText("Elemental Brawler", 10, 10, 20, DARKGRAY);

    EndDrawing();
  }
  CloseWindow();
}

void GameManager::update(float deltaTime)
{
  if (state == GameState::GameOver)
  {
    // Round is frozen until the player restarts.
    if (IsKeyPressed(KEY_R))
    {
      spawnFighters();
    }
    return;
  }

  // 1. Move fighters (and bounce off walls)
  for (auto &fighter : fighters)
  {
    fighter->update(deltaTime);
  }

  // 2. Weapons aim and fire. New shots go into pendingProjectiles.
  updateWeapons(deltaTime);

  // 3. Move existing projectiles, then add this frame's new ones.
  updateProjectiles(deltaTime);

  // 4. Collisions and damage
  resolveFighterCollisions();
  resolveProjectileHits();

  // 5. Remove everything that died this frame (after the loops, never inside)
  removeDeadEntities();

  // 6. Did the round end?
  checkGameOver();
}

void GameManager::updateWeapons(float deltaTime)
{
  WeaponContext ctx{fighters, pendingProjectiles};
  for (auto &fighter : fighters)
  {
    fighter->updateWeapon(deltaTime, ctx);
  }
}

void GameManager::updateProjectiles(float deltaTime)
{
  for (auto &projectile : projectiles)
  {
    projectile.update(deltaTime);
  }

  // Merge the spawn queue now that no loop is iterating over `projectiles`.
  // push_back during a loop over the same vector could reallocate it and
  // invalidate the loop's iterators/references (undefined behaviour).
  projectiles.insert(projectiles.end(), std::make_move_iterator(pendingProjectiles.begin()),
                     std::make_move_iterator(pendingProjectiles.end()));
  pendingProjectiles.clear();
}

void GameManager::resolveFighterCollisions()
{
  for (size_t i = 0; i < fighters.size(); ++i)
  {
    for (size_t j = i + 1; j < fighters.size(); ++j)
    {
      auto &a = fighters[i];
      auto &b = fighters[j];

      float dist = Vector2Distance(a->position, b->position);
      float min_dist = a->radius + b->radius;

      if (dist < min_dist && dist > 0.0f)
      {
        // Overlapping! Push them apart
        float overlap = min_dist - dist;
        Vector2 normal = Vector2Normalize(Vector2Subtract(a->position, b->position));

        // Push proportional to radius (or simply equal push)
        Vector2 pushA = Vector2Scale(normal, overlap * 0.5f);
        Vector2 pushB = Vector2Scale(normal, -overlap * 0.5f);

        a->position = Vector2Add(a->position, pushA);
        b->position = Vector2Add(b->position, pushB);

        // Simple elastic collision (swap velocities along the normal)
        Vector2 relativeVelocity = Vector2Subtract(a->velocity, b->velocity);
        float speed = Vector2DotProduct(relativeVelocity, normal);

        if (speed < 0)
        {
          // They are moving towards each other, meaning a collision just
          // occurred!
          const float restitution = 1.0f;                       // 1.0 means perfectly elastic (no speed loss)
          float impulse = -(1.0f + restitution) * speed / 2.0f; // Assuming equal mass

          Vector2 impulseVec = Vector2Scale(normal, impulse);
          a->velocity = Vector2Add(a->velocity, impulseVec);
          b->velocity = Vector2Subtract(b->velocity, impulseVec);

          // Ramming damage. No cast needed: the container already holds FighterBalls.
          a->applyDamage(15.0f);
          b->applyDamage(15.0f);
        }
      }
    }
  }
}

void GameManager::resolveProjectileHits()
{
  for (auto &projectile : projectiles)
  {
    if (!projectile.isAlive())
    {
      continue;
    }

    for (auto &fighter : fighters)
    {
      if (fighter->id == projectile.getOwnerId() || !fighter->isAlive())
      {
        continue; // can't hit yourself or a ball that's already dead
      }

      if (CheckCollisionCircles(projectile.position, projectile.radius, fighter->position,
                                fighter->radius))
      {
        fighter->applyHit(projectile.getHit());
        projectile.kill(); // one hit per projectile
        break;
      }
    }
  }
}

void GameManager::removeDeadEntities()
{
  // C++20 std::erase_if: removes every element matching the predicate in one
  // pass. Safe because nothing else is iterating over these vectors right now.
  std::erase_if(fighters,
                [](const std::shared_ptr<FighterBall> &f)
                { return !f->isAlive(); });
  std::erase_if(projectiles, [](const Projectile &p)
                { return !p.isAlive(); });
}

void GameManager::checkGameOver()
{
  if (fighters.size() > 1)
  {
    return; // fight continues
  }

  state = GameState::GameOver;

  if (fighters.empty())
  {
    resultText = "DRAW!"; // last two died on the same hit
    return;
  }

  resultText = std::string(toString(fighters.front()->currentElement)) + " WINS!";
}

void GameManager::draw() const
{
  for (const auto &fighter : fighters)
  {
    fighter->draw();
  }

  for (const auto &projectile : projectiles)
  {
    projectile.draw();
  }

  if (state == GameState::GameOver)
  {
    drawGameOver();
  }
}

void GameManager::drawGameOver() const
{
  const int w = GetScreenWidth();
  const int h = GetScreenHeight();

  // Dim the arena
  DrawRectangle(0, 0, w, h, Fade(BLACK, 0.5f));

  // Centered result text
  const int titleSize = 50;
  const int titleWidth = MeasureText(resultText.c_str(), titleSize);
  DrawText(resultText.c_str(), (w - titleWidth) / 2, h / 2 - 40, titleSize, RAYWHITE);

  const char *hint = "Press R to restart";
  const int hintSize = 20;
  const int hintWidth = MeasureText(hint, hintSize);
  DrawText(hint, (w - hintWidth) / 2, h / 2 + 25, hintSize, LIGHTGRAY);
}

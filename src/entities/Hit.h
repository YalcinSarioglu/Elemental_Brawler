#pragma once

// Everything a single attack does to its target.
// Every weapon describes its attack with the same struct, and the target
// decides how to react (FighterBall::applyHit).
struct Hit
{
  float damage = 0.0f;
  float burnSeconds = 0.0f;  // > 0: sets the target on fire
  float chillSeconds = 0.0f; // > 0: slows the target's movement and attacks
};

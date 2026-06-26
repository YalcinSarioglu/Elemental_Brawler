#include "Entity.h"

Entity::Entity(Vector2 pos, float rad, Color col)
    : position(pos), radius(rad), color(col), velocity({0.0f, 0.0f}) {}

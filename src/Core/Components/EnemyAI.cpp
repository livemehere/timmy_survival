#include "EnemyAI.hpp"
#include "../GameObject.hpp"
#include "raylib.h"
#include "raymath.h"

void EnemyAI::Update(float dt) {
  // follow target
  Vector2 myPos = gameObject->position;
  Vector2 targetPos = target->position;

  Vector2 dir = Vector2Subtract(targetPos, myPos);
  dir = Vector2Normalize(dir);

  gameObject->position.x += dir.x * speed * dt;
  gameObject->position.y += dir.y * speed * dt;
}

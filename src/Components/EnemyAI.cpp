#include "EnemyAI.hpp"
#include "../Core/GameObject.hpp"
#include "Render/SpriteRenderer.hpp"
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

  float len = Vector2Length(dir);
  auto sprite = gameObject->GetComponent<SpriteRenderer>();
  if (sprite) {
    if (len > 0) {
      // enemy moving
      sprite->Play("Walk");
      sprite->flipX = dir.x < 0;
    } else {
      // enemy idle
      sprite->Play("Idle");
    }
  }
}

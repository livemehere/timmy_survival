#include "Magnet.hpp"

#include "../../Core/GameObject.hpp"
#include "raymath.h"

void Magnet::Update(float dt) {
  if (!targetPos) {
    return;
  }

  Vector2 dir = Vector2Subtract(*targetPos, gameObject->position);
  float distance = Vector2Length(dir);

  if (distance > 1.0f) {
    Vector2 normDir = Vector2Normalize(dir);
    gameObject->position =
        Vector2Add(gameObject->position, Vector2Scale(normDir, speed * dt));
  } else {
    if (onCollect) {
      onCollect();
    }
  }
}

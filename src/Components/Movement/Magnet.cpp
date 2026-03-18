#include "Magnet.hpp"

#include "../../Core/GameObject.hpp"
#include "raymath.h"

#include <algorithm>

void Magnet::Update(float dt) {
  if (!targetPos) {
    isMagnetizing = false;
    return;
  }

  isMagnetizing = true;

  Vector2 dir = Vector2Subtract(*targetPos, gameObject->position);
  float distance = Vector2Length(dir);

  if (distance > 10.0f) {
    speed = std::min(maxSpeed, speed + acceleration * dt);
    Vector2 normDir = Vector2Normalize(dir);
    float step = std::min(distance, speed * dt);
    gameObject->position =
        Vector2Add(gameObject->position, Vector2Scale(normDir, step));
  } else {
    isMagnetizing = false;
    if (onCollect) {
      onCollect();
    }
  }
}

#include "./FollowMovement.hpp"
#include "../../Core/GameObject.hpp"
#include "raymath.h"

void FollowMovement::Update(float dt) {
  if (!target)
    return;

  Vector2 myPos = gameObject->position;
  Vector2 targetPos = Vector2Add(target->position, offset);

  if (isLerping) {

    Vector2 dir = Vector2Subtract(targetPos, myPos);
    dir = Vector2Normalize(dir);
    gameObject->position =
        Vector2Lerp(gameObject->position, targetPos, speed * dt);
  } else {
    gameObject->position = targetPos;
  }
}

#include "./Follow.hpp"
#include "../../Core/GameObject.hpp"
#include "../Render/SpriteRenderer.hpp"
#include "raymath.h"

void Follow::Update(float dt) {
  if (!target)
    return;

  Vector2 myPos = gameObject->position;
  Vector2 targetPos = Vector2Add(target->position, offset);

  if (flipWithTarget) {
    auto targetSprite = target->GetComponent<SpriteRenderer>();
    if (targetSprite) {
      if (targetSprite->flipX) {
        targetPos.x -= offset.x * 2; // flip offset horizontally
      }
    }
  }

  if (isLerping) {
    Vector2 dir = Vector2Subtract(targetPos, myPos);
    dir = Vector2Normalize(dir);
    gameObject->position =
        Vector2Lerp(gameObject->position, targetPos, speed * dt);
  } else {
    gameObject->position = targetPos;
  }
}

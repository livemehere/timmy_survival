#include "BoxCollider.hpp"
#include "../Core/GameObject.hpp"
#include "raylib.h"

BoxCollider::BoxCollider(float width, float height, Vector2 offset)
    : Collider(ColliderType::BOX, offset), size({width, height}) {}

Rectangle BoxCollider::GetRect() {
  return {gameObject->position.x + offset.x, gameObject->position.y + offset.y,
          size.x, size.y};
}

void BoxCollider::Draw() {
  if (!Collider::debugMode)
    return;

  auto rect = GetRect();
  DrawRectangleRec(rect, Fade(RED, 0.5f));
  DrawRectangleLinesEx(rect, 2.0f, RED);
}

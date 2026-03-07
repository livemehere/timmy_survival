#include "BoxCollider.hpp"
#include "../GameObject.hpp"
#include "raylib.h"

Rectangle BoxCollider::GetRect() {
  return {gameObject->position.x + offset.x, gameObject->position.y + offset.y,
          size.x, size.y};
}

void BoxCollider::Draw() {
  auto rect = GetRect();
  DrawRectangleRec(rect, Fade(RED, 0.5f));
  DrawRectangleLinesEx(rect, 2.0f, RED);
}

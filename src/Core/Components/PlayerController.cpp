#include "raylib.h"

#include "../GameObject.hpp"
#include "PlayerController.hpp"

void PlayerController::Update(float dt) {
  if (IsKeyDown(KEY_W))
    gameObject->position.y -= speed * dt;
  if (IsKeyDown(KEY_S))
    gameObject->position.y += speed * dt;
  if (IsKeyDown(KEY_A))
    gameObject->position.x -= speed * dt;
  if (IsKeyDown(KEY_D))
    gameObject->position.x += speed * dt;
}

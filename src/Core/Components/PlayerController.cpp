#include "SpriteRenderer.hpp"
#include "raylib.h"
#include "raymath.h"

#include "../GameObject.hpp"
#include "PlayerController.hpp"

void PlayerController::Update(float dt) {
  Vector2 movement = {0, 0};
  if (IsKeyDown(KEY_W))
    movement.y -= 1;
  if (IsKeyDown(KEY_S))
    movement.y += 1;
  if (IsKeyDown(KEY_A))
    movement.x -= 1;
  if (IsKeyDown(KEY_D))
    movement.x += 1;

  float len = Vector2Length(movement);

  if (len > 0) {
    movement = Vector2Normalize(movement);
    gameObject->position =
        Vector2Add(gameObject->position, Vector2Scale(movement, speed * dt));
  };

  auto sprite = gameObject->GetComponent<SpriteRenderer>();
  if (sprite) {
    if (len > 0) {
      sprite->Play("Walk");
      sprite->flipX = movement.x < 0;
    } else {
      sprite->Play("Idle");
    }
  }
}

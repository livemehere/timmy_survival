#include "Velocity.hpp"

#include "../Core/GameObject.hpp"
#include "raymath.h"

Velocity::Velocity(Vector2 velocity, float damping)
    : velocity(velocity), damping(damping) {}

void Velocity::Update(float dt) {
  if (velocity.x == 0.0f && velocity.y == 0.0f) {
    return;
  }

  gameObject->position = Vector2Add(gameObject->position, Vector2Scale(velocity, dt));

  if (damping <= 0.0f) {
    return;
  }

  velocity = Vector2Lerp(velocity, {0.0f, 0.0f}, damping * dt);

  if (Vector2LengthSqr(velocity) < 1.0f) {
    velocity = {0.0f, 0.0f};
  }
}

#include "OrbitBlade.hpp"

#include "../../Core/Collider.hpp"
#include "../../Core/GameObject.hpp"
#include "../Health.hpp"
#include "../Movement/Velocity.hpp"
#include "raymath.h"

void OrbitBlade::OnTriggerEnter(Collider *other) {
  if (other->gameObject->layer != Layer::ENEMY) {
    return;
  }

  auto health = other->gameObject->GetComponent<Health>();
  if (!health) {
    return;
  }

  if (!health->TakeDamage(damage, gameObject, hitCooldown)) {
    return;
  }

  if (knockbackForce <= 0.0f) {
    return;
  }

  auto velocity = other->gameObject->GetComponent<Velocity>();
  if (!velocity) {
    return;
  }

  Vector2 dir = Vector2Subtract(other->gameObject->position, gameObject->position);
  if (Vector2LengthSqr(dir) <= 0.0f) {
    return;
  }

  velocity->Apply(Vector2Scale(Vector2Normalize(dir), knockbackForce));
}

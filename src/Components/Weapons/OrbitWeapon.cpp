#include "OrbitWeapon.hpp"

#include "../../Core/Collider.hpp"
#include "../../Core/GameObject.hpp"
#include "../Health.hpp"
#include "../Render/SpriteRenderer.hpp"
#include "raymath.h"
#include <cmath>

namespace {
void UpdateOrbitTransform(GameObject *weapon, GameObject *owner, float orbitRadius,
                          float angleDegrees) {
  float angleRadians = angleDegrees * DEG2RAD;
  Vector2 orbitOffset = {std::cos(angleRadians) * orbitRadius,
                         std::sin(angleRadians) * orbitRadius};
  weapon->position = Vector2Add(owner->position, orbitOffset);

  auto sprite = weapon->GetComponent<SpriteRenderer>();
  if (sprite) {
    sprite->rotation = angleDegrees + 90.0f;
  }
}
} // namespace

void OrbitWeapon::Start() {
  if (!owner) {
    return;
  }

  UpdateOrbitTransform(gameObject, owner, orbitRadius, angleDegrees);
}

void OrbitWeapon::Update(float dt) {
  if (!owner || !owner->isAlive) {
    return;
  }

  angleDegrees += orbitSpeedDegrees * dt;
  UpdateOrbitTransform(gameObject, owner, orbitRadius, angleDegrees);
}

void OrbitWeapon::OnTriggerEnter(Collider *other) {
  if (other->gameObject->layer != Layer::ENEMY) {
    return;
  }

  auto health = other->gameObject->GetComponent<Health>();
  if (!health) {
    return;
  }

  health->TakeDamage(damage, gameObject);
}

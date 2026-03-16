#include "DamageZone.hpp"

#include "../../Core/GameObject.hpp"
#include "../../Core/World.hpp"
#include "../Health.hpp"
#include "../Movement/Velocity.hpp"
#include "raymath.h"

namespace {
Vector2 GetZoneKnockbackDirection(Vector2 from, Vector2 to) {
  Vector2 dir = Vector2Subtract(to, from);
  if (Vector2LengthSqr(dir) <= 0.0f) {
    return {0.0f, -1.0f};
  }

  return Vector2Normalize(dir);
}
} // namespace

void DamageZone::Update(float dt) {
  lifetimeTimer.Update(dt);

  if (!lifetimeTimer.IsRunning()) {
    gameObject->Destroy();
    return;
  }

  Trigger();
}

void DamageZone::Draw() {
  float fade = 1.0f - lifetimeTimer.GetProgress();
  DrawCircleV(gameObject->position, radius, Fade(GREEN, 0.12f + fade * 0.1f));
  DrawCircleLinesV(gameObject->position, radius, Fade(LIME, 0.5f + fade * 0.4f));
  DrawCircleLinesV(gameObject->position, radius * 0.6f,
                   Fade(YELLOW, 0.2f + fade * 0.3f));
}

void DamageZone::Trigger() {
  for (auto *enemy : gameObject->world->GetObjectsByLayer(Layer::ENEMY)) {
    if (!enemy || !enemy->isAlive) {
      continue;
    }

    float distSqr = Vector2DistanceSqr(gameObject->position, enemy->position);
    if (distSqr > radius * radius) {
      continue;
    }

    auto health = enemy->GetComponent<Health>();
    if (!health || !health->TakeDamage(damage, gameObject, tickInterval)) {
      continue;
    }

    auto velocity = enemy->GetComponent<Velocity>();
    if (velocity && knockbackForce > 0.0f) {
      Vector2 dir = GetZoneKnockbackDirection(gameObject->position, enemy->position);
      velocity->Apply(Vector2Scale(dir, knockbackForce));
    }
  }
}

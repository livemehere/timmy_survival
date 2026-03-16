#include "DamageZone.hpp"

#include "../../Core/GameObject.hpp"
#include "../../Core/World.hpp"
#include "../Health.hpp"
#include "../Movement/Velocity.hpp"
#include "raymath.h"

namespace {
bool IsInsideEllipse(Vector2 point, Vector2 center, float width, float height) {
  float halfWidth = width * 0.5f;
  float halfHeight = height * 0.5f;
  if (halfWidth <= 0.0f || halfHeight <= 0.0f) {
    return false;
  }

  float dx = (point.x - center.x) / halfWidth;
  float dy = (point.y - center.y) / halfHeight;
  return (dx * dx) + (dy * dy) <= 1.0f;
}

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
  float halfWidth = width * 0.5f;
  float halfHeight = height * 0.5f;
  float fade = 1.0f - lifetimeTimer.GetProgress();
  DrawEllipse(static_cast<int>(gameObject->position.x),
              static_cast<int>(gameObject->position.y), halfWidth, halfHeight,
              Fade(GREEN, 0.12f + fade * 0.1f));
  DrawEllipseLines(static_cast<int>(gameObject->position.x),
                   static_cast<int>(gameObject->position.y), halfWidth,
                   halfHeight, Fade(LIME, 0.5f + fade * 0.4f));
  DrawEllipseLines(static_cast<int>(gameObject->position.x),
                   static_cast<int>(gameObject->position.y), halfWidth * 0.6f,
                   halfHeight * 0.6f, Fade(YELLOW, 0.2f + fade * 0.3f));
}

void DamageZone::Trigger() {
  for (auto *enemy : gameObject->world->GetObjectsByLayer(Layer::ENEMY)) {
    if (!enemy || !enemy->isAlive) {
      continue;
    }

    if (!IsInsideEllipse(enemy->position, gameObject->position, width, height)) {
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

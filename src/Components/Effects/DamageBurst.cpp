#include "DamageBurst.hpp"

#include "../../Core/GameObject.hpp"
#include "../../Core/World.hpp"
#include "../../Managers/CameraManager.hpp"
#include "../../Managers/GameManager.hpp"
#include "../Health.hpp"
#include "../Movement/Velocity.hpp"
#include "raymath.h"

namespace {
Vector2 GetKnockbackDirection(Vector2 from, Vector2 to) {
  Vector2 dir = Vector2Subtract(to, from);
  if (Vector2LengthSqr(dir) <= 0.0f) {
    return {0.0f, -1.0f};
  }

  return Vector2Normalize(dir);
}

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
} // namespace

void DamageBurst::Update(float dt) {
  hitTimer.Update(dt);
  lifetimeTimer.Update(dt);

  if (!didBurst && !hitTimer.IsRunning()) {
    Trigger();
  }

  if (!lifetimeTimer.IsRunning()) {
    gameObject->Destroy();
  }
}

void DamageBurst::Draw() {
  float halfWidth = width * 0.5f;
  float halfHeight = height * 0.5f;
  float lifeProgress = lifetimeTimer.GetProgress();

  if (!didBurst) {
    float telegraphProgress = hitTimer.GetTargetTime() > 0.0f
                                  ? hitTimer.GetProgress()
                                  : 1.0f;
    float alpha = 0.2f + telegraphProgress * 0.5f;
    DrawEllipse(static_cast<int>(gameObject->position.x),
                static_cast<int>(gameObject->position.y), halfWidth, halfHeight,
                Fade(WHITE, alpha * 0.1f));
    DrawEllipseLines(static_cast<int>(gameObject->position.x),
                     static_cast<int>(gameObject->position.y), halfWidth,
                     halfHeight, Fade(SKYBLUE, alpha));
    return;
  }

  float fade = 1.0f - lifeProgress;
  DrawEllipse(static_cast<int>(gameObject->position.x),
              static_cast<int>(gameObject->position.y), halfWidth, halfHeight,
              Fade(SKYBLUE, fade * 0.22f));
  DrawEllipseLines(static_cast<int>(gameObject->position.x),
                   static_cast<int>(gameObject->position.y), halfWidth,
                   halfHeight, Fade(WHITE, fade));
}

void DamageBurst::Trigger() {
  didBurst = true;

  for (auto *enemy : gameObject->world->GetObjectsByLayer(Layer::ENEMY)) {
    if (!enemy || !enemy->isAlive) {
      continue;
    }

    if (!IsInsideEllipse(enemy->position, gameObject->position, width, height)) {
      continue;
    }

    auto health = enemy->GetComponent<Health>();
    if (!health || !health->TakeDamage(damage, gameObject, hitCooldown)) {
      continue;
    }

    auto velocity = enemy->GetComponent<Velocity>();
    if (velocity && knockbackForce > 0.0f) {
      Vector2 dir = GetKnockbackDirection(gameObject->position, enemy->position);
      velocity->Apply(Vector2Scale(dir, knockbackForce));
    }
  }

  CameraManager::Get().Shake(10.0f, 0.18f);
  GameManager::Get().AddShock(gameObject->position);
}

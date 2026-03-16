#include "LightningStrike.hpp"

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
} // namespace

void LightningStrike::Update(float dt) {
  hitTimer.Update(dt);
  lifetimeTimer.Update(dt);

  if (!didStrike && !hitTimer.IsRunning()) {
    Trigger();
  }

  if (!lifetimeTimer.IsRunning()) {
    gameObject->Destroy();
  }
}

void LightningStrike::Draw() {
  float lifeProgress = lifetimeTimer.GetProgress();

  if (!didStrike) {
    float telegraphProgress = hitTimer.GetTargetTime() > 0.0f
                                  ? hitTimer.GetProgress()
                                  : 1.0f;
    float alpha = 0.2f + telegraphProgress * 0.5f;
    DrawCircleLinesV(gameObject->position, radius, Fade(SKYBLUE, alpha));
    DrawCircleV(gameObject->position, radius * 0.3f, Fade(WHITE, alpha * 0.2f));

    Vector2 top = Vector2Add(gameObject->position, {0.0f, -90.0f});
    DrawLineEx(top, gameObject->position, 3.0f, Fade(WHITE, 0.25f + alpha * 0.6f));
    return;
  }

  float fade = 1.0f - lifeProgress;
  Vector2 top = Vector2Add(gameObject->position, {0.0f, -110.0f});
  DrawLineEx(top, gameObject->position, 6.0f, Fade(WHITE, fade));
  DrawCircleV(gameObject->position, radius, Fade(SKYBLUE, fade * 0.2f));
  DrawCircleLinesV(gameObject->position, radius, Fade(WHITE, fade));
}

void LightningStrike::Trigger() {
  didStrike = true;

  for (auto *enemy : gameObject->world->GetObjectsByLayer(Layer::ENEMY)) {
    if (!enemy || !enemy->isAlive) {
      continue;
    }

    float distSqr = Vector2DistanceSqr(gameObject->position, enemy->position);
    if (distSqr > radius * radius) {
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

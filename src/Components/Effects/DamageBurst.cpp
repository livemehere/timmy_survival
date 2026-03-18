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

  if (!didBurst) {
    // [1] Telegraph phase: targeting from sky
    float telegraphProgress =
        hitTimer.GetTargetTime() > 0.0f ? hitTimer.GetProgress() : 1.0f;
    float alpha = telegraphProgress; // Gradually becomes visible

    // Targeting reticle (slight rotation)
    float rot = GetTime() * 90.0f;
    DrawPolyLinesEx(gameObject->position, 4, halfWidth, rot, 2.0f,
                    Fade(SKYBLUE, alpha * 0.5f));

    // Energy particles converging to center
    for (int i = 0; i < 4; i++) {
      float angle = rot * DEG2RAD + (i * PI / 2.0f);
      float dist = halfWidth * (1.0f - telegraphProgress); // To center
      Vector2 particlePos = {gameObject->position.x + cosf(angle) * dist,
                             gameObject->position.y + sinf(angle) * dist};
      DrawCircleV(particlePos, 3.0f, Fade(WHITE, alpha));
    }
    return;
  }

  // ==========================================
  // [2] Burst phase: BOOM!
  // ==========================================
  float lifeProgress = lifetimeTimer.GetProgress(); // 0.0 ~ 1.0
  float fade = 1.0f - lifeProgress;
  float flashAlpha = fade * fade * fade; // Super bright initially, fades fast

  // 1. Intense flash glow
  if (flashAlpha > 0.05f) {
    DrawCircleGradient(static_cast<int>(gameObject->position.x),
                       static_cast<int>(gameObject->position.y),
                       width * flashAlpha * 1.5f,
                       Fade(WHITE, flashAlpha * 0.8f), Fade(SKYBLUE, 0.0f));
  }

  // 2. Main lightning bolt (jagged & branches)
  if (flashAlpha > 0.1f) {
    // From sky height (y - 800) to target
    Vector2 skyPos = {gameObject->position.x, gameObject->position.y - 800.0f};
    Vector2 currentPos = skyPos;

    const int segments = 12; // Number of zigzag points
    for (int i = 1; i <= segments; i++) {
      float t = (float)i / segments;
      Vector2 nextPos = Vector2Lerp(skyPos, gameObject->position, t);

      // Less jitter as it approaches target
      if (i < segments) {
        float jitterX = GetRandomValue(-50, 50) * (1.0f - t * 0.5f);
        nextPos.x += jitterX;
      }

      // Lightning outer glow (blue, thick)
      DrawLineEx(currentPos, nextPos, 20.0f * flashAlpha,
                 Fade(SKYBLUE, flashAlpha * 0.5f));
      // Lightning core (white, thin)
      DrawLineEx(currentPos, nextPos, 8.0f * flashAlpha,
                 Fade(WHITE, flashAlpha));

      // Intermittent branch lightning
      if (GetRandomValue(0, 10) > 6) {
        Vector2 branchEnd = {nextPos.x + GetRandomValue(-80, 80),
                             nextPos.y + GetRandomValue(20, 100)};
        DrawLineEx(nextPos, branchEnd, 10.0f * flashAlpha,
                   Fade(SKYBLUE, flashAlpha * 0.4f));
        DrawLineEx(nextPos, branchEnd, 4.0f * flashAlpha,
                   Fade(WHITE, flashAlpha * 0.8f));
      }

      currentPos = nextPos;
    }
  }

  // 3. Ground shockwave ring
  // Uses Raylib's DrawRing for thick spreading ring
  float shockwaveRadius =
      halfWidth * (1.0f + lifeProgress * 2.0f); // Up to 3x size
  float shockwaveThick = 15.0f * fade;          // Gradually thinner

  if (shockwaveThick > 0.5f) {
    DrawRing(gameObject->position, shockwaveRadius - shockwaveThick,
             shockwaveRadius, 0.0f, 360.0f, 36, Fade(SKYBLUE, fade * 0.7f));
    DrawRing(gameObject->position, shockwaveRadius - (shockwaveThick * 0.3f),
             shockwaveRadius, 0.0f, 360.0f, 36, Fade(WHITE, fade * 0.9f));
  }

  // 4. Lingering sparks on ground
  for (int i = 0; i < 5; i++) {
    float angle = i * (PI / 2.5f) + (lifeProgress * 5.0f); // Spinning
    float r = halfWidth * 0.6f +
              sinf(GetTime() * 30.0f + i) * 15.0f; // Jittery movement

    // Perspective: compress y-axis for 3D effect
    Vector2 sparkPos = {gameObject->position.x + cosf(angle) * r,
                        gameObject->position.y + sinf(angle) * r * 0.6f};

    DrawCircleV(sparkPos, 3.0f * fade, Fade(WHITE, fade));
    DrawCircleV(sparkPos, 6.0f * fade, Fade(SKYBLUE, fade * 0.5f));
  }
}

void DamageBurst::Trigger() {
  didBurst = true;

  for (auto *enemy : gameObject->world->GetObjectsByLayer(Layer::ENEMY)) {
    if (!enemy || !enemy->isAlive) {
      continue;
    }

    if (!IsInsideEllipse(enemy->position, gameObject->position, width,
                         height)) {
      continue;
    }

    auto health = enemy->GetComponent<Health>();
    if (!health || !health->TakeDamage(damage, gameObject, hitCooldown)) {
      continue;
    }

    auto velocity = enemy->GetComponent<Velocity>();
    if (velocity && knockbackForce > 0.0f) {
      Vector2 dir =
          GetKnockbackDirection(gameObject->position, enemy->position);
      velocity->Apply(Vector2Scale(dir, knockbackForce));
    }
  }

  CameraManager::Get().Shake(10.0f, 0.18f);
  GameManager::Get().AddShock(gameObject->position);
}

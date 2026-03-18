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
  float baseRadius = std::max(width, height) * 0.5f;
  float progress = lifetimeTimer.GetProgress(); // 0.0 ~ 1.0
  float fade = 1.0f - progress;                 // 1.0 ~ 0.0

  // [1] Pop-in animation (Ease-Out)
  // During the very beginning of lifetime (first 15%), it scales from 0 to 1x.
  float popInProgress = std::min(1.0f, progress * 6.6f);
  float easeOut = 1.0f - powf(1.0f - popInProgress, 3.0f);

  // Final scale set to 1.2x for slightly larger appearance.
  float currentRadius = baseRadius * (easeOut * 1.2f);

  // [2] Fade-out
  // Gradually becomes transparent when less than 30% lifetime remains.
  float alphaMult = (fade < 0.3f) ? (fade / 0.3f) : 1.0f;

  // [3] Continuous effect: keeps rotating strangely and pulsating
  float time = GetTime();
  float baseRotation = time * 45.0f; // 45 degrees rotation per second

  // --- Layer 1: Widest green 5-gon on the ground (clockwise rotation) ---
  DrawPoly(gameObject->position, 5, currentRadius, baseRotation,
           Fade(GREEN, 0.25f * alphaMult));
  // Outline is slightly thicker to clearly mark the damage zone boundary.
  DrawPolyLinesEx(gameObject->position, 5, currentRadius, baseRotation,
                  5.0f * easeOut, Fade(LIME, 0.6f * alphaMult));

  // --- Layer 2: Middle gas layer (faster counter-clockwise rotation + pulsating)
  // ---
  float midScale = 0.7f + sinf(time * 6.0f) * 0.05f; // Wobble effect
  DrawPoly(gameObject->position, 5, currentRadius * midScale,
           -baseRotation * 1.5f, Fade(DARKGREEN, 0.4f * alphaMult));

  // --- Layer 3: Most lethal central poison core (purple, irregular vibration) ---
  float innerScale = 0.35f + cosf(time * 10.0f) * 0.05f;
  DrawPoly(gameObject->position, 5, currentRadius * innerScale,
           baseRotation * 2.0f, Fade(PURPLE, 0.55f * alphaMult));

  // --- Detail: Mini poison gas particles bubbling around the damage zone ---
  for (int i = 0; i < 5; i++) {
    float bubbleOffset = time * 3.0f + i * 1.5f;
    float angle = (i * 72.0f + sinf(bubbleOffset) * 30.0f) * DEG2RAD;

    // Fluid feel: particles pushed outward from center then back in
    float r = currentRadius * (0.4f + 0.5f * fabs(sinf(bubbleOffset * 1.2f)));

    // Slight elliptical (perspective) feel by compressing y-axis to 0.7x
    Vector2 bubblePos = {gameObject->position.x + cosf(angle) * r,
                         gameObject->position.y + sinf(angle) * r * 0.7f};

    // Particle size also grows and shrinks repeatedly
    float bubbleRadius = 12.0f * (1.0f - fabs(sinf(bubbleOffset * 1.2f)));

    // Particles unified as cute mini 5-gons!
    DrawPoly(bubblePos, 5, bubbleRadius, time * 120.0f,
             Fade(LIME, 0.7f * alphaMult));
  }
}

void DamageZone::Trigger() {
  for (auto *enemy : gameObject->world->GetObjectsByLayer(Layer::ENEMY)) {
    if (!enemy || !enemy->isAlive) {
      continue;
    }

    if (!IsInsideEllipse(enemy->position, gameObject->position, width,
                         height)) {
      continue;
    }

    auto health = enemy->GetComponent<Health>();
    if (!health || !health->TakeDamage(damage, gameObject, tickInterval)) {
      continue;
    }

    auto velocity = enemy->GetComponent<Velocity>();
    if (velocity && knockbackForce > 0.0f) {
      Vector2 dir =
          GetZoneKnockbackDirection(gameObject->position, enemy->position);
      velocity->Apply(Vector2Scale(dir, knockbackForce));
    }
  }
}

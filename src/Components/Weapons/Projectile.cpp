#include "Projectile.hpp"

#include "../../Core/GameObject.hpp"
#include "../../Core/World.hpp"
#include "../../Managers/CameraManager.hpp"
#include "../../Utils/MathUtils.hpp"
#include "../Colliders/CircleCollider.hpp"
#include "../Health.hpp"
#include "../Movement/Velocity.hpp"
#include "../Render/SpriteRenderer.hpp"
#include "../Particle.hpp"
#include "raymath.h"
#include <algorithm>
#include <cfloat>
#include <cmath>

GameObject *Projectile::FindNextTarget() const {
  GameObject *nearestEnemy = nullptr;
  float nearestDistSq = FLT_MAX;

  for (auto *obj : gameObject->world->GetObjectsByLayer(Layer::ENEMY)) {
    if (!obj || !obj->isAlive || hitTargets.contains(obj)) {
      continue;
    }

    float distSq = Vector2LengthSqr(Vector2Subtract(obj->position, gameObject->position));
    if (distSq < nearestDistSq) {
      nearestDistSq = distSq;
      nearestEnemy = obj;
    }
  }

  return nearestEnemy;
}

void Projectile::RedirectToTarget(GameObject *target) {
  auto velocity = gameObject->GetComponent<Velocity>();
  if (!velocity) {
    return;
  }

  currentTarget = target;
  if (!currentTarget || !currentTarget->isAlive) {
    return;
  }

  Vector2 toTarget = Vector2Subtract(currentTarget->position, gameObject->position);
  if (Vector2LengthSqr(toTarget) <= 0.001f) {
    return;
  }

  velocity->velocity = Vector2Scale(Vector2Normalize(toTarget), speed);
}

namespace {
void RotateProjectileSprite(GameObject *gameObject, Vector2 velocity,
                            float rotationOffset, bool rotateToVelocity) {
  if (!rotateToVelocity || Vector2LengthSqr(velocity) <= 0.001f) {
    return;
  }

  auto sprite = gameObject->GetComponent<SpriteRenderer>();
  if (!sprite) {
    return;
  }

  sprite->rotation = atan2f(velocity.y, velocity.x) * RAD2DEG + rotationOffset;
}
} // namespace

void Projectile::Update(float dt) {
  lifeTimer.Update(dt);
  if (!lifeTimer.IsRunning()) {
    gameObject->Destroy();
    return;
  }

  tailTimer.Update(dt);
  if (tailTimer.DidCompleteThisFrame()) {
    history.push_front(gameObject->position);
    if (history.size() > maxHistory) {
      history.pop_back();
    }
  }

  // Homing logic
  if (isHoming) {
    auto velocity = gameObject->GetComponent<Velocity>();
    if (velocity) {
      if ((!currentTarget || !currentTarget->isAlive) && pierce > 0) {
        currentTarget = FindNextTarget();
      }

      // Rotate toward the target if available
      if (currentTarget) {
        Vector2 currentDir = Vector2LengthSqr(velocity->velocity) > 0.001f
                                 ? Vector2Normalize(velocity->velocity)
                                 : Vector2Normalize(Vector2Subtract(currentTarget->position,
                                                                    gameObject->position));
        Vector2 toTarget = Vector2Normalize(
            Vector2Subtract(currentTarget->position, gameObject->position));

        // Rotate from the current direction toward the target direction
        float currentAngle = atan2f(currentDir.y, currentDir.x);
        float targetAngle = atan2f(toTarget.y, toTarget.x);

        // Calculate angle difference (-PI to PI)
        float angleDiff = targetAngle - currentAngle;
        while (angleDiff > PI) angleDiff -= 2 * PI;
        while (angleDiff < -PI) angleDiff += 2 * PI;

        // Clamp to the maximum turn speed
        float maxTurn = homingTurnRate * dt;
        float newAngle = currentAngle + std::clamp(angleDiff, -maxTurn, maxTurn);

        // Set velocity using the new direction
        Vector2 newDir = {cosf(newAngle), sinf(newAngle)};
        velocity->velocity = Vector2Scale(newDir, speed);
        RotateProjectileSprite(gameObject, velocity->velocity,
                               this->rotationOffset,
                               this->rotateToVelocity);
      }
    }
  }
}

void Projectile::OnTriggerEnter(Collider *other) {
  if (other->gameObject->layer != Layer::ENEMY) {
    return;
  }

  GameObject *hitTarget = other->gameObject;
  auto health = hitTarget->GetComponent<Health>();
  if (health && health->TakeDamage(damage, gameObject)) {
    pierce--;
    hitTargets.insert(hitTarget);
    currentTarget = nullptr;

    auto velocity = hitTarget->GetComponent<Velocity>();
    if (velocity) {
      Vector2 dir = Vector2Subtract(hitTarget->position, gameObject->position);
      dir = Vector2Normalize(dir);
      velocity->Apply(Vector2Scale(dir, knockbackForce));
    }

    SpawnParticles(gameObject->position, MathUtils::GetRandom(3, 5));
    CameraManager::Get().Shake(2.0f, 0.1f);
  }

  if (pierce <= 0) {
    gameObject->Destroy();
    return;
  }

  if (isHoming) {
    RedirectToTarget(FindNextTarget());
    auto velocity = gameObject->GetComponent<Velocity>();
    if (velocity) {
      RotateProjectileSprite(gameObject, velocity->velocity,
                             this->rotationOffset,
                             this->rotateToVelocity);
    }
  }
}

void Projectile::Draw() {
  BeginBlendMode(BLEND_ADDITIVE);
  int size = static_cast<int>(history.size());
  auto collider = gameObject->GetComponent<CircleCollider>();
  Vector2 prevPos = gameObject->position;

  for (size_t i = 0; i < history.size(); i++) {
    Vector2 pos = history[i];
    float radiusRatio = static_cast<float>(size - static_cast<int>(i)) / size;
    float radius = collider->radius * radiusRatio;
    DrawLineEx(prevPos, pos, radius, Fade(color, radiusRatio));
    prevPos = pos;
  }

  EndBlendMode();
}

void Projectile::SpawnParticles(Vector2 pos, int count) {
  for (int i = 0; i < count; i++) {
    auto obj = gameObject->world->CreateObject("particle");
    obj->position = pos;

    float speed = MathUtils::GetRandom(2.0f, 10.0f);
    Vector2 velocity = {std::cos(MathUtils::GetRandom(0.0f, 2 * PI)) * speed,
                        std::sin(MathUtils::GetRandom(0.0f, 2 * PI)) * speed};
    float lifetime = MathUtils::GetRandom(0.3f, 1.0f);
    float size = MathUtils::GetRandom(3.0f, 6.0f);
    float friction = MathUtils::GetRandom(7.0f, 10.0f);

    obj->AddComponent<Particle>(velocity, lifetime, LIGHTGRAY, size, friction);
  }
}

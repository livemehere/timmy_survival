#include "Projectile.hpp"

#include "../../Core/GameObject.hpp"
#include "../../Core/World.hpp"
#include "../../Managers/CameraManager.hpp"
#include "../../Utils/MathUtils.hpp"
#include "../Colliders/CircleCollider.hpp"
#include "../Health.hpp"
#include "../Movement/Velocity.hpp"
#include "../Particle.hpp"
#include "raymath.h"
#include <cmath>

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
}

void Projectile::OnTriggerEnter(Collider *other) {
  if (other->gameObject->layer != Layer::ENEMY) {
    return;
  }

  if (hitObjects.find(other->gameObject) != hitObjects.end()) {
    return;
  }

  auto health = other->gameObject->GetComponent<Health>();
  if (health) {
    hitObjects.insert(other->gameObject);

    pierce--;
    health->TakeDamage(damage);

    auto velocity = other->gameObject->GetComponent<Velocity>();
    if (velocity) {
      Vector2 dir =
          Vector2Subtract(other->gameObject->position, gameObject->position);
      dir = Vector2Normalize(dir);
      velocity->Apply(Vector2Scale(dir, knockbackForce));
    }

    SpawnParticles(gameObject->position, MathUtils::GetRandom(3, 5));
    CameraManager::Get().Shake(2.0f, 0.1f);
  }

  if (pierce <= 0) {
    gameObject->Destroy();
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

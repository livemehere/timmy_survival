#include "FireWeapon.hpp"

#include "../../Core/GameObject.hpp"
#include "../../Core/World.hpp"
#include "../Colliders/CircleCollider.hpp"
#include "../Movement/Velocity.hpp"
#include "Projectile.hpp"
#include "raymath.h"

void FireWeapon::Update(float dt) {
  timer.Update(dt);

  if (timer.DidCompleteThisFrame()) {
    Activate();
  }
}

void FireWeapon::Activate() {
  GameObject *target = FindNearestEnemy();
  if (!target) {
    return;
  }

  Vector2 dir = Vector2Subtract(target->position, gameObject->position);
  dir = Vector2Normalize(dir);

  auto obj = gameObject->world->CreateObject("projectile");
  obj->layer = Layer::PROJECTILE;
  obj->position = gameObject->position;
  obj->AddComponent<Projectile>(damage, projectileLifetime, 1, 150.0f);
  Vector2 power = Vector2Scale(dir, projectileSpeed);
  obj->AddComponent<Velocity>(power, 0.0f);

  auto col = obj->AddComponent<CircleCollider>(projectileRadius);
  col->isTrigger = true;
}

GameObject *FireWeapon::FindNearestEnemy() {
  Vector2 myPos = gameObject->position;
  GameObject *nearestEnemy = nullptr;
  float minDistSqr = range * range;

  for (auto &obj : gameObject->world->objects) {
    if (obj->layer != Layer::ENEMY || !obj->isAlive) {
      continue;
    }

    float distSqr = Vector2DistanceSqr(obj->position, myPos);
    if (distSqr <= minDistSqr) {
      minDistSqr = distSqr;
      nearestEnemy = obj.get();
    }
  }

  return nearestEnemy;
}

void FireWeapon::Draw() {
  DrawCircleLines(gameObject->position.x, gameObject->position.y, range, GREEN);
}

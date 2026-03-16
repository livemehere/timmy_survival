#include "FireWeapon.hpp"

#include "../../Core/GameObject.hpp"
#include "../../Core/World.hpp"
#include "../../Prefabs/Prefabs.hpp"
#include "../Render/SpriteRenderer.hpp"
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

  Vector2 power = Vector2Scale(dir, projectileSpeed);
  auto obj = Prefabs::CreateProjectile(*gameObject->world, gameObject->position,
                                       power, projectileDefinition, damage);
  if (projectileDefinition.rotateToVelocity) {
    auto sprite = obj->GetComponent<SpriteRenderer>();
    if (sprite) {
      sprite->rotation =
          atan2f(dir.y, dir.x) * RAD2DEG + projectileDefinition.rotationOffset;
    }
  }
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
  if (showRange) {
    DrawCircleLines(gameObject->position.x, gameObject->position.y, range,
                    GREEN);
  }
}

#pragma once

#include "../GameObject.hpp"
#include "../World.hpp"
#include "LinearMovement.hpp"
#include "Projectile.hpp"
#include "Weapon.hpp"
#include "raymath.h"
#include <iostream>

class FireWeapon : public Weapon {
public:
  float projectileSpeed;
  float projectileLifetime;
  float projectileRadius;
  float range;

  FireWeapon(float damage, float cooldown, float projectileSpeed,
             float projectileLifetime, float projectileRadius, float range)
      : Weapon(damage, cooldown), projectileSpeed(projectileSpeed),
        projectileLifetime(projectileLifetime),
        projectileRadius(projectileRadius), range(range) {}

  void Activate() override {

    std::cout << "FireWeapon activated! Damage: " << damage << std::endl;

    GameObject *target = FindNearestEnemy();
    if (!target) {
      return;
    }
    Vector2 dir = Vector2Subtract(target->position, gameObject->position);
    dir = Vector2Normalize(dir);

    auto obj = gameObject->world->CreateObject("projectile");
    obj->layer = Layer::PROJECTILE;
    obj->position = gameObject->position;
    obj->AddComponent<Projectile>(damage, projectileLifetime);
    obj->AddComponent<LinearMovement>(dir, projectileSpeed);

    auto col = obj->AddComponent<CircleCollider>(projectileRadius);
    col->isTrigger = true;
  }

  GameObject *FindNearestEnemy() {
    Vector2 myPos = gameObject->position;
    GameObject *nearestEnemy = nullptr;

    float minDistSqr = range * range;

    // 1. find nearest enemy within range
    for (auto &obj : gameObject->world->objects) {
      if (obj->layer == Layer::ENEMY && obj->isAlive) {

        float distSqr = Vector2DistanceSqr(obj->position, myPos);
        if (distSqr <= minDistSqr) {
          minDistSqr = distSqr;
          nearestEnemy = obj.get();
        }
      }
    }

    return nearestEnemy;
  }

  void Draw() override {

    DrawCircle(gameObject->position.x, gameObject->position.y, range,
               Fade(RED, 0.05f));
    DrawCircleLines(gameObject->position.x, gameObject->position.y, range, RED);
  }
};

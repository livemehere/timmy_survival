#pragma once

#include "../GameObject.hpp"
#include "../World.hpp"
#include "LinearMovement.hpp"
#include "Projectile.hpp"
#include "Weapon.hpp"
#include <iostream>

class FireWeapon : public Weapon {
public:
  float projectileSpeed;
  float projectileLifetime;
  float projectileRadius;

  FireWeapon(float damage, float cooldown, float projectileSpeed,
             float projectileLifetime, float projectileRadius)
      : Weapon(damage, cooldown), projectileSpeed(projectileSpeed),
        projectileLifetime(projectileLifetime),
        projectileRadius(projectileRadius) {}

  void Activate() override {

    std::cout << "FireWeapon activated! Damage: " << damage << std::endl;

    Vector2 dir = {1, 0};

    auto obj = gameObject->world->CreateObject("projectile");
    obj->layer = Layer::PROJECTILE;
    obj->position = gameObject->position;
    obj->AddComponent<Projectile>(damage, projectileLifetime);
    obj->AddComponent<LinearMovement>(dir, projectileSpeed);

    auto col = obj->AddComponent<CircleCollider>(projectileRadius);
    col->isTrigger = true;
  }
};

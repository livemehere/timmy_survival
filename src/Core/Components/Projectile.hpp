#pragma once

#include <iostream>
#include <unordered_set>

#include "../Components/Health.hpp"
#include "../GameObject.hpp"
#include "../Timer.hpp"
#include "Collider.hpp"

class Projectile : public Component {
public:
  Timer lifeTimer;
  float damage;
  int pierce = 1;

  std::unordered_set<GameObject *> hitObjects;

  Projectile(float damage, float lifetime, int pierce = 1)
      : lifeTimer(lifetime, false), damage(damage), pierce(pierce) {}

  void Update(float dt) override {
    if (lifeTimer.Update(dt)) {
      gameObject->Destroy();
    };
  }

  void OnTriggerEnter(Collider *other) override {

    if (other->gameObject->layer != Layer::ENEMY)
      return;

    if (hitObjects.find(other->gameObject) != hitObjects.end())
      return;

    std::cout << "Projectile hit: " << other->gameObject->name << std::endl;

    auto health = other->gameObject->GetComponent<Health>();
    if (health) {

      hitObjects.insert(other->gameObject);

      pierce--;
      health->TakeDamage(damage);
    }

    if (pierce <= 0) {
      gameObject->Destroy();
    }
  }
};

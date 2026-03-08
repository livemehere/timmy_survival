#pragma once

#include <iostream>

#include "../Component.hpp"
#include "../GameObject.hpp"
#include "../Timer.hpp"
#include "Collider.hpp"

class Projectile : public Component {
public:
  Timer lifeTimer;
  float damage;
  int pierce = 1;

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

    pierce--;
    std::cout << "Projectile hit: " << other->gameObject->name << std::endl;
    if (pierce <= 0) {
      gameObject->Destroy();
      other->gameObject->Destroy();
    }
  }
};

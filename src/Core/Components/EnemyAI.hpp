#pragma once

#include "../Component.hpp"

class EnemyAI : public Component {
public:
  GameObject *target = nullptr;
  float speed;

  EnemyAI(GameObject *target, float speed) : target(target), speed(speed) {}

  void Update(float dt) override;
};

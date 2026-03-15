#pragma once

#include "../Core/Component.hpp"

class PlayerController : public Component {
public:
  float speed;

  PlayerController(float speed) : speed(speed) {}
  void Update(float dt) override;
};

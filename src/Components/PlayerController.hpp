#pragma once

#include "../Core/Component.hpp"

class PlayerController : public Component {
public:
  float speed;

  explicit PlayerController(float speed) : speed(speed) {}
  void Update(float dt) override;
};

#pragma once

#include "../Component.hpp"

class PlayerController : public Component {
public:
  float speed;

  PlayerController(float speed) { this->speed = speed; }
  void Update(float dt) override;
};

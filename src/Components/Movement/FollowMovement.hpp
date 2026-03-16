#pragma once

#include "../../Core/Component.hpp"
#include "raylib.h"

class FollowMovement : public Component {
public:
  GameObject *target;
  bool isLerping;
  float speed;
  Vector2 offset;

  FollowMovement(GameObject *target, Vector2 offset = {0.0f, 0.0f},
                 float speed = 5.0f, bool isLerping = true)
      : target(target), speed(speed), offset(offset), isLerping(isLerping) {}

  void Update(float dt) override;
};

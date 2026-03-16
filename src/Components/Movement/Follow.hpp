#pragma once

#include "../../Core/Component.hpp"
#include "raylib.h"

class Follow : public Component {
public:
  GameObject *target;
  bool isLerping;
  float speed;
  Vector2 offset;
  bool flipWithTarget;

  Follow(GameObject *target, Vector2 offset = {0.0f, 0.0f}, float speed = 5.0f,
         bool isLerping = true, bool flipWithTarget = true)
      : target(target), speed(speed), offset(offset), isLerping(isLerping),
        flipWithTarget(flipWithTarget) {}

  void Update(float dt) override;
};

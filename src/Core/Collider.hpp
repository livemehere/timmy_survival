#pragma once

#include "Component.hpp"
#include "raylib.h"

enum class ColliderType { BOX, CIRCLE };

class Collider : public Component {
public:
  ColliderType type;
  Vector2 offset = {0, 0};

  Collider(ColliderType type, Vector2 offset) : type(type), offset(offset) {}
  virtual ~Collider() = default;
};

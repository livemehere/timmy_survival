#pragma once
#include "../../Core/Collider.hpp"
#include <optional>

class CircleCollider : public Collider {
public:
  float radius;
  std::optional<ColliderLabel> label = std::nullopt;

  CircleCollider(float r, Vector2 offset = {0, 0})
      : Collider(ColliderType::CIRCLE, offset), radius(r) {}

  Vector2 GetCenter() const;

  void Draw() override;
};

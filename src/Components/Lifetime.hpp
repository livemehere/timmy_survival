#pragma once

#include "../Core/Component.hpp"
#include "../Core/Timer.hpp"

class Lifetime : public Component {
public:
  Timer timer;

  explicit Lifetime(float duration) : timer(duration, false) {}

  void Update(float dt) override;
};

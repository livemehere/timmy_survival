#pragma once

#include "../Core/Component.hpp"
#include "../Core/Timer.hpp"

class Lifetime : public Component {
public:
  Timer timer;

  explicit Lifetime(float duration);

  void Update(float dt) override;
};

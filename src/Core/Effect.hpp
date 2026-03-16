#pragma once

#include "Component.hpp"

class Effect : public Component {
public:
  virtual void Trigger() = 0;
};

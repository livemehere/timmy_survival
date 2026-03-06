#pragma once

class GameObject;

class Component {
public:
  GameObject *gameObject = nullptr;

  virtual ~Component() = default;
  virtual void Update(float dt) {}
  virtual void Draw() {}
};

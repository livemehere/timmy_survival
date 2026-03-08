#pragma once

class Collider;

class GameObject;

class Component {
public:
  GameObject *gameObject = nullptr;

  virtual ~Component() = default;
  virtual void Update(float dt) {}
  virtual void Draw() {}
  virtual void OnTriggerEnter(Collider *other) {}
};

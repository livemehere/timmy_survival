#pragma once
#include "Component.hpp"
#include "raylib.h"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

class World;

enum class Layer {
  DEFAULT = 0,
  PLAYER = 1,
  ENEMY = 2,
  PROJECTILE = 3,
  UI = 4,
};

class GameObject {
public:
  World *world = nullptr;

  Layer layer = Layer::DEFAULT;
  std::string name;
  Vector2 position = {0.0f, 0.0f};
  std::vector<std::shared_ptr<Component>> components;
  bool isAlive = true;

  GameObject(std::string name, World *world) {
    this->name = name;
    this->world = world;
  }

  template <typename T, typename... Args> T *AddComponent(Args &&...args) {
    auto comp = std::make_shared<T>(std::forward<Args>(args)...);
    comp->gameObject = this;
    components.push_back(comp);
    return comp.get();
  }

  template <typename T> T *GetComponent() {
    for (auto &comp : components) {
      T *ptr = dynamic_cast<T *>(comp.get());
      if (ptr != nullptr)
        return ptr;
    }
    return nullptr;
  }

  void Update(float dt) {
    for (auto &comp : components) {
      comp->Update(dt);
    }
  }

  void Draw() {
    for (auto &comp : components) {
      comp->Draw();
    }
    DrawPos();
  }

  void DrawPos() {}

  void Destroy() { isAlive = false; }
};

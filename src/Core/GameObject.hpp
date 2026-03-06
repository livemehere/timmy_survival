#pragma once
#include "Component.hpp"
#include "raylib.h"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

class World;

class GameObject {
public:
  World *world = nullptr;

  std::string name;
  Vector2 position = {0.0f, 0.0f};
  std::vector<std::shared_ptr<Component>> components;

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
  }
};

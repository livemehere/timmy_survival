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
  ITEM = 5,
};

class GameObject {
public:
  World *world = nullptr;

  Layer layer = Layer::DEFAULT;
  std::string name;
  Vector2 position = {0.0f, 0.0f};
  std::vector<std::unique_ptr<Component>> components;
  bool isAlive = true;

  GameObject(std::string name, World *world) : name(name), world(world) {}

  template <typename T, typename... Args> T *AddComponent(Args &&...args) {
    auto comp = std::make_unique<T>(std::forward<Args>(args)...);

    T *ptr = comp.get();

    ptr->gameObject = this;
    components.push_back(std::move(comp));
    ptr->Start();

    return ptr;
  }

  template <typename T> T *GetComponent() {
    for (auto &comp : components) {
      T *ptr = dynamic_cast<T *>(comp.get());
      if (ptr != nullptr)
        return ptr;
    }
    return nullptr;
  }

  void Start() {
    for (auto &comp : components) {
      comp->Start();
    }
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

  void DrawUI() {
    for (auto &comp : components) {
      comp->DrawUI();
    }
  }

  void OnDestroy() {
    for (auto &comp : components) {
      comp->OnDestroy();
    }
  }

  void Destroy() { isAlive = false; }
};

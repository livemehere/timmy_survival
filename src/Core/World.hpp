#pragma once

#include "GameObject.hpp"
#include <memory>
#include <string>
#include <vector>

class World {
public:
  std::vector<std::shared_ptr<GameObject>> objects;

  GameObject *CreateObject(std::string name) {
    auto obj = std::make_shared<GameObject>(name, this);
    objects.push_back(obj);
    return obj.get();
  }

  void Update(float dt) {
    for (auto &obj : objects) {
      obj->Update(dt);
    }
  }

  void Draw() {
    for (auto &obj : objects) {
      obj->Draw();
    }
  }
};

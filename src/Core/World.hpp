#pragma once

#include "Components/BoxCollider.hpp"
#include "GameObject.hpp"
#include <memory>
#include <string>
#include <vector>

class World {
public:
  std::vector<std::shared_ptr<GameObject>> objects;

  GameObject *CreateObject(std::string name);
  void Update(float dt);
  void Draw();
  std::vector<BoxCollider *> GetAllColliders();
  void ResolveCollisions();
};

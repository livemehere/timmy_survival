#pragma once

#include "raylib.h"
#include <string>
#include <unordered_map>

class ResourceManager {
private:
  std::unordered_map<std::string, Texture2D> cache;

  ResourceManager() = default;
  ~ResourceManager() = default;

  void UnloadAll();

public:
  static ResourceManager &Get();

  ResourceManager(const ResourceManager &) = delete;
  ResourceManager &operator=(const ResourceManager &) = delete;

  void Init();
  void Clear();
  Texture2D *GetTexture(const std::string &path);
};

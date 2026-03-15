#pragma once

#include "raylib.h"
#include <iostream>
#include <string>
#include <unordered_map>

class ResourceManager {
private:
  std::unordered_map<std::string, Texture2D> cache;

  ResourceManager() {}
  ~ResourceManager() {}

  void UnloadAll() {
    for (auto &pair : cache) {
      UnloadTexture(pair.second);
    }
    cache.clear();
    std::cout << "All resources unloaded." << std::endl;
  }

public:
  static ResourceManager &Get() {
    static ResourceManager instance;
    return instance;
  }

  ResourceManager(const ResourceManager &) = delete;
  ResourceManager &operator=(const ResourceManager &) = delete;

  void Init() { GetTexture("../assets/source.png"); }

  void Clear() { UnloadAll(); }

  Texture2D *GetTexture(const std::string &path) {
    if (cache.find(path) == cache.end()) {
      Texture2D texture = LoadTexture(path.c_str());
      if (texture.id == 0) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return nullptr;
      }
      SetTextureFilter(texture, TEXTURE_FILTER_POINT);
      cache[path] = texture;
      std::cout << "Loaded texture: " << path << std::endl;
    }

    return &cache[path];
  };
};

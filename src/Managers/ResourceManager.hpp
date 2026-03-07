#pragma once

#include "raylib.h"
#include <iostream>
#include <string>
#include <unordered_map>

class ResourceManager {
private:
  std::unordered_map<std::string, Texture2D> textures;

  ResourceManager() {}
  ~ResourceManager() { UnloadAll(); }

public:
  static ResourceManager &GetInstance() {
    static ResourceManager instance;
    return instance;
  }

  ResourceManager(const ResourceManager &) = delete;
  ResourceManager &operator=(const ResourceManager &) = delete;

  Texture2D *GetTexture(const std::string &path) {
    if (textures.find(path) == textures.end()) {
      Texture2D texture = LoadTexture(path.c_str());
      if (texture.id == 0) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return nullptr;
      }
      SetTextureFilter(texture, TEXTURE_FILTER_POINT);
      textures[path] = texture;
      std::cout << "Loaded texture: " << path << std::endl;
    }

    return &textures[path];
  };

  void UnloadAll() {
    for (auto &pair : textures) {
      UnloadTexture(pair.second);
    }
    textures.clear();
    std::cout << "All resources unloaded." << std::endl;
  }
};

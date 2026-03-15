#include "ResourceManager.hpp"
#include <iostream>

ResourceManager &ResourceManager::Get() {
  static ResourceManager instance;
  return instance;
}

void ResourceManager::UnloadAll() {
  for (auto &pair : cache) {
    std::cout << "Unloading texture: " << pair.first << std::endl;
    UnloadTexture(pair.second);
  }
  cache.clear();
  std::cout << "All resources unloaded." << std::endl;
}

void ResourceManager::Init() { GetTexture("../assets/source.png"); }

void ResourceManager::Clear() { UnloadAll(); }

Texture2D *ResourceManager::GetTexture(const std::string &path) {
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
}

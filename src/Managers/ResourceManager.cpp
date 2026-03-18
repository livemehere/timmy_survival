#include "ResourceManager.hpp"
#include <filesystem>
#include <iostream>

namespace {
std::filesystem::path GetSourceRoot() {
#ifdef TIMMY_SOURCE_DIR
  return std::filesystem::path(TIMMY_SOURCE_DIR);
#else
  return std::filesystem::current_path();
#endif
}
} // namespace

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

void ResourceManager::Init() { GetTexture("assets/source.png"); }

void ResourceManager::Clear() { UnloadAll(); }

std::string ResourceManager::ResolvePath(const std::string &path) {
#ifdef __EMSCRIPTEN__
  return path;
#else
  const std::filesystem::path requested(path);
  if (requested.is_absolute() && std::filesystem::exists(requested)) {
    return requested.string();
  }

  const std::filesystem::path appDir(GetApplicationDirectory());
  const std::filesystem::path appRelative = appDir / requested;
  if (std::filesystem::exists(appRelative)) {
    return appRelative.string();
  }

  const std::filesystem::path bundleResourcesRelative =
      appDir / ".." / "Resources" / requested;
  if (std::filesystem::exists(bundleResourcesRelative)) {
    return bundleResourcesRelative.lexically_normal().string();
  }

  const std::filesystem::path cwdRelative =
      std::filesystem::current_path() / requested;
  if (std::filesystem::exists(cwdRelative)) {
    return cwdRelative.string();
  }

  const std::filesystem::path sourceRelative = GetSourceRoot() / requested;
  return sourceRelative.string();
#endif
}

Texture2D *ResourceManager::GetTexture(const std::string &path) {
  if (cache.find(path) == cache.end()) {
    const std::string resolvedPath = ResolvePath(path);
    Texture2D texture = LoadTexture(resolvedPath.c_str());
    if (texture.id == 0) {
      std::cerr << "Failed to load texture: " << resolvedPath << std::endl;
      return nullptr;
    }

    SetTextureFilter(texture, TEXTURE_FILTER_POINT);
    cache[path] = texture;
    std::cout << "Loaded texture: " << resolvedPath << std::endl;
  }

  return &cache[path];
}

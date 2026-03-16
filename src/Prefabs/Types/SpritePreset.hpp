#pragma once

#include "../../Components/Render/SpriteRenderer.hpp"
#include <string>
#include <vector>

struct SpritePreset {
  std::vector<SpriteClip> clips;
  std::string defaultClipName;
  Vector2 scale = {1.0f, 1.0f};
  Vector2 anchorRatio = {0.5f, 0.5f};
  Vector2 offset = {0.0f, 0.0f};
  Color tint = WHITE;
};

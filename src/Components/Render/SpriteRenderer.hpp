#pragma once

#include "../../Core/Component.hpp"
#include "../../Core/Timer.hpp"
#include "raylib.h"
#include <string>
#include <unordered_map>

struct SpriteClip {
  std::string name;
  std::string texturePath;

  int startX;
  int startY;
  int frameWidth;
  int frameHeight;

  int frameCount;
  float frameSpeed;
  bool loop;
};

class SpriteRenderer : public Component {

  std::unordered_map<std::string, SpriteClip> clipsByName;
  std::string activeClipName;

  int currentFrameIndex = 0;
  Timer playbackTimer = Timer(0.0f, true);
  bool isPlaying = false;
  bool debugMode = false;

public:
  Vector2 scale = {1.0f, 1.0f};
  float rotation = 0.0f;
  Color tint = WHITE;
  bool flipX = false;

  Vector2 anchorRatio = {0.5f, 0.5f};
  Vector2 offset = {0, 0};

  SpriteRenderer() = default;
  SpriteRenderer(const SpriteClip &initialClip,
                 Vector2 initialScale = {1.0f, 1.0f},
                 Vector2 initialAnchorRatio = {0.5f, 0.5f}) {
    AddClip(initialClip);
    scale = initialScale;
    anchorRatio = initialAnchorRatio;
  }

  void AddClip(const SpriteClip &clip);

  void Play(const std::string &clipName);

  void Update(float dt) override;

  void Draw() override;
};

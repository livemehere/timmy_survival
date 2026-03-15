#pragma once

#include "../../Core/Component.hpp"
#include "../../Core/Timer.hpp"
#include "raylib.h"
#include <string>
#include <unordered_map>

struct AnimationClip {
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

  std::unordered_map<std::string, AnimationClip> animations;
  std::string currentClip;

  int currentFrame = 0;
  Timer frameTimer = Timer(0.0f, true);
  // this is not timer status, it's animation status
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

  void AddAnimation(const std::string &name, const std::string &texturePath,
                    int startX, int startY, int frameWidth, int frameHeight,
                    int frameCount, float frameSpeed, bool loop);

  void Play(const std::string &name);

  void Update(float dt) override;

  void Draw() override;
};

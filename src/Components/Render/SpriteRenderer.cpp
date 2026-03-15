#include "SpriteRenderer.hpp"
#include "../../Core/GameObject.hpp"
#include "../../Managers/ResourceManager.hpp"

void SpriteRenderer::AddAnimation(const std::string &name,
                                  const std::string &texturePath, int startX,
                                  int startY, int frameWidth, int frameHeight,
                                  int frameCount, float frameSpeed, bool loop) {
  animations[name] = {name,        texturePath, startX,     startY, frameWidth,
                      frameHeight, frameCount,  frameSpeed, loop};

  if (currentClip.empty()) {
    Play(name);
  }
}

void SpriteRenderer::Play(const std::string &name) {
  if (currentClip == name && isPlaying)
    return;

  if (animations.find(name) != animations.end()) {
    auto &clip = animations[name];
    currentClip = name;
    currentFrame = 0;
    frameTimer.Reset(clip.frameSpeed);
    isPlaying = true;
  }
}

void SpriteRenderer::Update(float dt) {
  if (!isPlaying || currentClip.empty())
    return;

  auto &clip = animations[currentClip];
  frameTimer.Update(dt);

  if (frameTimer.DidCompleteThisFrame()) {
    currentFrame++;

    if (currentFrame >= clip.frameCount) {
      if (clip.loop) {
        currentFrame = 0;
      } else {
        currentFrame = clip.frameCount - 1;
        isPlaying = false;
      }
    }
  }
}

void SpriteRenderer::Draw() {
  if (currentClip.empty())
    return;

  auto &clip = animations[currentClip];
  Texture2D *texture = ResourceManager::Get().GetTexture(clip.texturePath);
  if (!texture)
    return;

  float x = clip.startX + (currentFrame * clip.frameWidth);
  float y = clip.startY;

  Rectangle sourceRect = {
      x, y, flipX ? (float)-clip.frameWidth : (float)clip.frameWidth,
      (float)clip.frameHeight};

  float drawWidth = clip.frameWidth * scale.x;
  float drawHeight = clip.frameHeight * scale.y;

  Rectangle destRect = {gameObject->position.x + offset.x,
                        gameObject->position.y + offset.y, drawWidth,
                        drawHeight};

  Vector2 anchor = {drawWidth * anchorRatio.x, drawHeight * anchorRatio.y};

  DrawTexturePro(*texture, sourceRect, destRect, anchor, rotation, tint);

  if (debugMode) {
    Rectangle debugRect = {destRect.x - anchor.x, destRect.y - anchor.y,
                           destRect.width, destRect.height};
    DrawRectangleLinesEx(debugRect, 0.5f, RED);
  }
}

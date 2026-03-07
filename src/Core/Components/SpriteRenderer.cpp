#include "SpriteRenderer.hpp"
#include "../../Managers/ResourceManager.hpp"
#include "../GameObject.hpp"

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
    currentClip = name;
    currentFrame = 0;
    frameTimer = 0.0f;
    isPlaying = true;
  }
}

void SpriteRenderer::Update(float dt) {
  if (!isPlaying || currentClip.empty())
    return;

  frameTimer += dt;
  auto &clip = animations[currentClip];

  if (frameTimer >= clip.frameSpeed) {
    frameTimer -= clip.frameSpeed;
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
  Texture2D *texture =
      ResourceManager::GetInstance().GetTexture(clip.texturePath);
  if (!texture)
    return;

  float x = clip.startX + (currentFrame * clip.frameWidth);
  float y = clip.startY;

  Rectangle sourceRect = {
      x, y, flipX ? (float)-clip.frameWidth : (float)clip.frameWidth,
      (float)clip.frameHeight};

  float drawWidth = clip.frameWidth * scale;
  float drawHeight = clip.frameHeight * scale;

  Rectangle destRect = {gameObject->position.x + offset.x,
                        gameObject->position.y + offset.y, drawWidth,
                        drawHeight};

  Vector2 anchor = {drawWidth * anchorRatio.x, drawHeight * anchorRatio.y};

  DrawTexturePro(*texture, sourceRect, destRect, anchor, rotation, tint);

  Rectangle debugRect = {destRect.x - anchor.x, destRect.y - anchor.y,
                         destRect.width, destRect.height};
  DrawRectangleLinesEx(debugRect, 0.5f, RED);
}

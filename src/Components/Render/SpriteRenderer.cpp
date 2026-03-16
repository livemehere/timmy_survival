#include "SpriteRenderer.hpp"
#include "../../Core/GameObject.hpp"
#include "../../Managers/ResourceManager.hpp"

void SpriteRenderer::AddClip(const SpriteClip &clip) {
  clipsByName[clip.name] = clip;

  if (activeClipName.empty()) {
    Play(clip.name);
  }
}

void SpriteRenderer::Play(const std::string &clipName) {
  if (activeClipName == clipName && isPlaying)
    return;

  if (clipsByName.find(clipName) != clipsByName.end()) {
    auto &clip = clipsByName[clipName];
    activeClipName = clipName;
    currentFrameIndex = 0;
    playbackTimer.Reset(clip.frameSpeed);
    isPlaying = true;
  }
}

void SpriteRenderer::Update(float dt) {
  if (!isPlaying || activeClipName.empty())
    return;

  auto &clip = clipsByName[activeClipName];
  playbackTimer.Update(dt);

  if (playbackTimer.DidCompleteThisFrame()) {
    currentFrameIndex++;

    if (currentFrameIndex >= clip.frameCount) {
      if (clip.loop) {
        currentFrameIndex = 0;
      } else {
        currentFrameIndex = clip.frameCount - 1;
        isPlaying = false;
      }
    }
  }
}

void SpriteRenderer::Draw() {
  if (activeClipName.empty())
    return;

  auto &clip = clipsByName[activeClipName];
  Texture2D *texture = ResourceManager::Get().GetTexture(clip.texturePath);
  if (!texture)
    return;

  float x = clip.startX + (currentFrameIndex * clip.frameWidth);
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

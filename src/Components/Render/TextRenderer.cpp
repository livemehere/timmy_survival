#include "TextRenderer.hpp"

#include "../../Core/GameObject.hpp"
#include "../../Managers/CameraManager.hpp"
#include "../Lifetime.hpp"
#include "raymath.h"

// TODO: change to use SDF based text rendering
void TextRenderer::DrawUI() {
  Font font = GetFontDefault();
  float fontSizeF = static_cast<float>(fontSize);
  Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSizeF, 1.0f);

  Vector2 screenPos = GetWorldToScreen2D(gameObject->position,
                                         CameraManager::Get().GetCamera());
  Vector2 drawPos = screenPos;

  if (centered) {
    drawPos.x -= textSize.x * 0.5f;
  }

  if (fadeOut) {
    auto progress = gameObject->GetComponent<Lifetime>();
    if (progress) {
      alpha = 1.0f - progress->timer.GetProgress();
    }
  }

  Color mainColor = Fade(color, alpha);
  Color strokeColor = Fade(outlineColor, alpha);
  Vector2 offset = {static_cast<float>(outlineThickness),
                    static_cast<float>(outlineThickness)};

  if (hasOutline && outlineThickness > 0) {
    DrawTextEx(font, text.c_str(), Vector2Subtract(drawPos, offset), fontSizeF,
               1.0f, strokeColor);
    DrawTextEx(font, text.c_str(), {drawPos.x, drawPos.y - offset.y}, fontSizeF,
               1.0f, strokeColor);
    DrawTextEx(font, text.c_str(), {drawPos.x + offset.x, drawPos.y - offset.y},
               fontSizeF, 1.0f, strokeColor);
    DrawTextEx(font, text.c_str(), {drawPos.x - offset.x, drawPos.y}, fontSizeF,
               1.0f, strokeColor);
    DrawTextEx(font, text.c_str(), {drawPos.x + offset.x, drawPos.y}, fontSizeF,
               1.0f, strokeColor);
    DrawTextEx(font, text.c_str(), {drawPos.x - offset.x, drawPos.y + offset.y},
               fontSizeF, 1.0f, strokeColor);
    DrawTextEx(font, text.c_str(), {drawPos.x, drawPos.y + offset.y}, fontSizeF,
               1.0f, strokeColor);
    DrawTextEx(font, text.c_str(), Vector2Add(drawPos, offset), fontSizeF, 1.0f,
               strokeColor);
  }

  DrawTextEx(font, text.c_str(), drawPos, fontSizeF, 1.0f, mainColor);
}

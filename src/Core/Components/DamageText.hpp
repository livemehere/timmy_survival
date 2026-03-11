#pragma once

#include "../GameObject.hpp"
#include "../Timer.hpp"
#include "../World.hpp"
#include "raylib.h"
#include "raymath.h"

class DamageText : public Component {

public:
  std::string text;
  Vector2 initialForce = {0.0f, -30.0f};
  Vector2 velocity = {0.0f, -100.0f};
  Timer timer = Timer(1.0f);
  Color color;
  int fontSize;
  float alpha = 1.0f;

  DamageText(const std::string &text, Color color, int fontSize)
      : text(text), color(color), fontSize(fontSize) {}

  void Update(float dt) override {
    if (timer.Update(dt)) {
      gameObject->Destroy();
    }

    gameObject->position.x += velocity.x * dt;
    gameObject->position.y += velocity.y * dt;

    velocity.x = Lerp(velocity.x, 0, 3.0f * dt);
    velocity.y = Lerp(velocity.y, 0, 3.0f * dt);

    float progress = timer.currentTime / timer.targetTime;
    alpha = 1.0f - progress;
  }

  void DrawUI() override {
    int textW = MeasureText(text.c_str(), fontSize);

    Vector2 screenPos = GetWorldToScreen2D(gameObject->position,
                                           gameObject->world->cm->GetCamera());
    float x = screenPos.x - textW / 2.0f;
    float y = screenPos.y;

    Color outlineColor = Fade(BLACK, alpha);
    Color mainColor = Fade(color, alpha);
    int outlineThickness = 1;

    // Draw outline (8 directions)
    DrawText(text.c_str(), x - outlineThickness, y - outlineThickness, fontSize,
             outlineColor);
    DrawText(text.c_str(), x + outlineThickness, y - outlineThickness, fontSize,
             outlineColor);
    DrawText(text.c_str(), x - outlineThickness, y + outlineThickness, fontSize,
             outlineColor);
    DrawText(text.c_str(), x + outlineThickness, y + outlineThickness, fontSize,
             outlineColor);
    DrawText(text.c_str(), x - outlineThickness, y, fontSize, outlineColor);
    DrawText(text.c_str(), x + outlineThickness, y, fontSize, outlineColor);
    DrawText(text.c_str(), x, y - outlineThickness, fontSize, outlineColor);
    DrawText(text.c_str(), x, y + outlineThickness, fontSize, outlineColor);

    // Draw main text
    DrawText(text.c_str(), x, y, fontSize, mainColor);
  }
};

#pragma once

#include "../../Core/Component.hpp"
#include "raylib.h"
#include <string>

class TextRenderer : public Component {
public:
  std::string text;
  Color color;
  int fontSize;
  float alpha = 1.0f;
  bool centered = true;
  bool hasOutline = true;
  Color outlineColor = BLACK;
  int outlineThickness = 1;
  bool fadeOut = false;

  TextRenderer(const std::string &text, Color color, int fontSize,
               float alpha = 1.0f, bool centered = true, bool hasOutline = true,
               Color outlineColor = BLACK, int outlineThickness = 1,
               bool fadeOut = false)
      : text(text), color(color), fontSize(fontSize), alpha(alpha),
        centered(centered), hasOutline(hasOutline), outlineColor(outlineColor),
        outlineThickness(outlineThickness), fadeOut(fadeOut) {};

  ~TextRenderer() = default;

  void DrawUI() override;
};

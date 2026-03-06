#include "raylib.h"

int main() {
  InitWindow(1200, 800, "Timmy Survival");

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Hello World", 600, 400, 40, BLACK);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}

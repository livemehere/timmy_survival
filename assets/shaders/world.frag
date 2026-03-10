#version 330
in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;

uniform sampler2D texture0;
uniform float aspectRatio;

// Shockwave parameters
const float MAX_SHOCKWAVE_TIME = 1.0; // Wave duration (seconds)
const int NUM_SHOCKWAVES = 20; // Number of simultaneous wave slots

const float WAVE_MAX_RADIUS = 0.5; // Maximum radius the wave spreads (UV ratio)
const float WAVE_RING_WIDTH = 0.05; // Ring width (larger = wider)
const float WAVE_STRENGTH = 0.1; // Distortion strength (larger = more refraction)
const float WAVE_FADE_IN = 0.05; // Fade-in period right after start (0-1 normalized time)
const float WAVE_FADE_OUT_START = 0.5; // Fade-out start point

const float CHROMA_AMOUNT = 0.01; // Chromatic aberration strength — set to 0 to disable
// ============================================================

uniform vec2 centres[NUM_SHOCKWAVES];
uniform float times[NUM_SHOCKWAVES];

float getOffsetStrength(float t, vec2 dir) {
  vec2 aspectDir = vec2(dir.x * aspectRatio, dir.y);
  float dist = length(aspectDir);

  float ringDist = abs(dist - t * WAVE_MAX_RADIUS);
  float ring = 1.0 - smoothstep(0.0, WAVE_RING_WIDTH, ringDist);

  float sign = (dist < t * WAVE_MAX_RADIUS) ? -1.0 : 1.0;

  float fade = smoothstep(0.0, WAVE_FADE_IN, t)
      * (1.0 - smoothstep(WAVE_FADE_OUT_START, MAX_SHOCKWAVE_TIME, t));

  return sign * ring * fade * WAVE_STRENGTH;
}

void main() {
  vec2 pos = fragTexCoord;

  // Accumulate UV displacements per channel separately — must add direction*size directly for correct multi-wave summation
  vec2 rDisp = vec2(0.0);
  vec2 gDisp = vec2(0.0);
  vec2 bDisp = vec2(0.0);

  for (int i = 0; i < NUM_SHOCKWAVES; i++) {
    float t = times[i];
    if (t >= MAX_SHOCKWAVE_TIME || t <= 0.0) continue;

    vec2 centre = centres[i];
    vec2 dir = centre - pos;
    if (length(dir) > 0.0) {
      dir = normalize(dir);
    }

    float tOffset = 0.01 * sin(t * 3.14);
    float rD = getOffsetStrength(t + tOffset, centre - pos);
    float gD = getOffsetStrength(t, centre - pos);
    float bD = getOffsetStrength(t - tOffset, centre - pos);

    rDisp += dir * rD;
    gDisp += dir * gD;
    bDisp += dir * bD;
  }

  // If no wave influence at all, output original pixel 100% unchanged
  if (length(gDisp) < 0.0001) {
    finalColor = texture(texture0, pos) * fragColor;
    return;
  }

  // Sample wave areas with per-channel distortion (chromatic aberration effect)
  // Must sample alpha from displaced UV to prevent black edges on sprites
  vec4 rSample = texture(texture0, pos + rDisp);
  vec4 gSample = texture(texture0, pos + gDisp);
  vec4 bSample = texture(texture0, pos + bDisp);

  finalColor = vec4(rSample.r, gSample.g, bSample.b, gSample.a) * fragColor;
}

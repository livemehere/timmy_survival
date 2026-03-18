#ifdef GL_ES
precision mediump float;
#endif

varying vec2 fragTexCoord;
varying vec4 fragColor;

uniform sampler2D texture0;
uniform float aspectRatio;

const float MAX_SHOCKWAVE_TIME = 1.0;
const int NUM_SHOCKWAVES = 20;

const float WAVE_MAX_RADIUS = 0.5;
const float WAVE_RING_WIDTH = 0.08;
const float WAVE_STRENGTH = 0.1;
const float WAVE_FADE_IN = 0.05;
const float WAVE_FADE_OUT_START = 0.5;

uniform vec2 centres[NUM_SHOCKWAVES];
uniform float times[NUM_SHOCKWAVES];

float getOffsetStrength(float t, vec2 dir) {
  vec2 aspectDir = vec2(dir.x * aspectRatio, dir.y);
  float dist = length(aspectDir);

  float ringDist = abs(dist - t * WAVE_MAX_RADIUS);
  float ring = 1.0 - smoothstep(0.0, WAVE_RING_WIDTH, ringDist);

  float signValue = dist < t * WAVE_MAX_RADIUS ? -1.0 : 1.0;
  float fade = smoothstep(0.0, WAVE_FADE_IN, t) *
               (1.0 - smoothstep(WAVE_FADE_OUT_START, MAX_SHOCKWAVE_TIME, t));

  return signValue * ring * fade * WAVE_STRENGTH;
}

void main() {
  vec2 pos = fragTexCoord;
  vec2 rDisp = vec2(0.0);
  vec2 gDisp = vec2(0.0);
  vec2 bDisp = vec2(0.0);

  for (int i = 0; i < NUM_SHOCKWAVES; i++) {
    float t = times[i];
    if (t >= MAX_SHOCKWAVE_TIME || t <= 0.0) {
      continue;
    }

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

  if (length(gDisp) < 0.0001) {
    gl_FragColor = texture2D(texture0, pos) * fragColor;
    return;
  }

  vec4 rSample = texture2D(texture0, pos + rDisp);
  vec4 gSample = texture2D(texture0, pos + gDisp);
  vec4 bSample = texture2D(texture0, pos + bDisp);

  gl_FragColor = vec4(rSample.r, gSample.g, bSample.b, gSample.a) * fragColor;
}

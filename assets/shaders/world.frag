#version 330

// raylib variables
in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;

uniform sampler2D texture0;
uniform float aspectRatio;

const float MAX_SHOCKWAVE_TIME = 1.0;
const int NUM_SHOCKWAVES = 20;
uniform vec2 centres[NUM_SHOCKWAVES];
uniform float times[NUM_SHOCKWAVES];

const float maxRadius = 0.5;

// distortion of shockwave at time t and direction dir
float getOffsetStrength(float t, vec2 dir) {
  vec2 aspectDir = vec2(dir.x * aspectRatio, dir.y);
  float d = length(aspectDir) - t * maxRadius;

  d *= 1.0 - smoothstep(0.0, 0.05, abs(d)); // mask
  d *= smoothstep(0.0, 0.05, t); // soft start
  d *= 1.0 - smoothstep(0.5, 1.0, t); // soft end
  return d;
}

void main() {
  vec2 pos = fragTexCoord;
  vec2 totalDir = vec2(0.0);
  vec3 totalOffsets = vec3(0.0);

  for (int i = 0; i < NUM_SHOCKWAVES; i++) {
    float t = times[i];
    if (t >= MAX_SHOCKWAVE_TIME) continue;

    vec2 centre = centres[i];
    vec2 dir = centre - pos;

    float tOffset = 0.01 * sin(t * 3.14);
    float rD = getOffsetStrength(t + tOffset, dir);
    float gD = getOffsetStrength(t, dir);
    float bD = getOffsetStrength(t - tOffset, dir);

    if (length(dir) > 0.0) {
      dir = normalize(dir);
    }

    float influence = ceil(abs(gD));

    totalDir += dir * influence;
    totalOffsets += vec3(rD, gD, bD) * influence;
  }

  float r = texture(texture0, pos + totalDir * totalOffsets.r).r;
  float g = texture(texture0, pos + totalDir * totalOffsets.g).g;
  float b = texture(texture0, pos + totalDir * totalOffsets.b).b;

  float shading = totalOffsets.g * 8.0; // Shading effects

  finalColor = vec4(r, g, b, 1.0) * fragColor;
  finalColor.rgb += shading; // apply shading
}

#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform float time;
uniform sampler2D texture0;

uniform vec2 center;
uniform float shockTime;
uniform float aspectRatio;
float speed = 1.5;
float thickness = 0.08;

void main() {
  vec2 uv = fragTexCoord; //current

  // distnace from wave center
  vec2 dir = uv - center;
  dir.x *= aspectRatio;
  float dist = length(dir);

  float radius = shockTime * speed;
  float diff = abs(dist - radius);

  if (diff < thickness && shockTime > 0.0 && shockTime < 1.0) {
    float distortionAmount = (thickness - diff) * 1.5;
    vec2 distorionDir = normalize(uv - center);

    uv -= distorionDir * distortionAmount * sin(diff * 30.0);
  }

  finalColor = texture(texture0, uv) * fragColor;
}

#version 330
in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;
uniform sampler2D texture0;
uniform float time; 

void main()
{
    vec2 uv = fragTexCoord;
    uv.x += sin(uv.y * 20.0 + time * 5.0) * 0.01;
    finalColor = texture(texture0, uv) * fragColor;
}

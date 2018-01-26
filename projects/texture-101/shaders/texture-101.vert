#version 330

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec2 uvPos;

uniform mat4 mvp;

out vec2 vs_tex_coord;

void main()
{
  vs_tex_coord = uvPos;
  gl_Position = mvp * vec4(vPosition, 1.0);
}

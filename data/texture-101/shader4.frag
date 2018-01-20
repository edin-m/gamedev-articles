#version 330

uniform sampler2D tex;

in vec2 vs_tex_coord;

out vec4 fColor;

void main()
{
  fColor = texture(tex, vs_tex_coord);
}

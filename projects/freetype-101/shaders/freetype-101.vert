#version 330

uniform mat4 mvp;

in vec3 vertex;
in vec2 tex_coord;
in vec4 color;

out vec2 out_tex_coord;
out vec4 out_color;

void main()
{
  out_tex_coord = tex_coord;
  out_color = color;
  gl_Position = mvp * vec4(vertex,1.0);
}

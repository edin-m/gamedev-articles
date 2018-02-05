#version 330

uniform sampler2D tex;

in vec2 out_tex_coord;
in vec4 out_color;

out vec4 fColor;

void main()
{
    float a = texture(tex, out_tex_coord.xy).r;
    fColor = vec4(out_color.rgb, out_color.a*a);
}

#version 330 core

in vec3 a_Pos;
in vec4 a_Color;

out vec4 outColor;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
    gl_Position = u_Projection * u_View * vec4(a_Pos.xyz, 1.0);

    outColor = a_Color;
}

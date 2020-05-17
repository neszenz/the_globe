#version 330 core

in vec3 a_pos;
in vec3 a_color;

uniform mat4 u_matrix;

out vec4 color;

void main() {
    color = vec4(a_color, 1.0);
    gl_Position = u_matrix * vec4(a_pos, 1.0);
}


#version 330 core

in vec4 i_color;
in vec3 i_pos;

out vec4 fragment;

void main() {
    float f = length(i_pos) - 1.0;

    fragment = length(i_pos) * abs(pow(f, 0.5)) * vec4(1.0, 1.0, 1.0, 1.0);
}


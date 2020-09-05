#version 330 core

layout (location =0 )in vec3 position;
layout(location = 1)in vec4 a_Color;
uniform mat4 u_mvp;

out vec4 v_Color;

void main() {
	v_Color = a_Color;
	gl_Position = u_mvp * vec4(position,1);
};

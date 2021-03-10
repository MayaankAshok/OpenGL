#version 330 core

layout (location =0 )in vec3 position;
layout(location = 1)in vec3 normal;

uniform vec3 u_Color;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 v_normal;
out vec3 fragPos;
out vec3 v_Color;

void main() {
	v_Color = u_Color;

	fragPos = vec3(vec4(position,1));//u_model
	v_normal = normal;

	gl_Position = u_projection * u_view * u_model * vec4(position,1);
};

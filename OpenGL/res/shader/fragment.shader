#version 330 core

layout(location = 0)out vec4 o_Color;

in vec3 v_normal;
in vec3 fragPos;
in vec3 v_Color;

void main() {

	vec3 lightPos = vec3(0, 0, 1);

	vec3 norm = normalize(v_normal);
	//vec3 lightDir = normalize(lightPos-fragPos);
	float diff = max(dot(norm, lightPos), 0.0);

	vec3 result = (diff+0.3) * v_Color * vec3(1, 0.8, 0.8);

	o_Color = vec4(result,1);
};

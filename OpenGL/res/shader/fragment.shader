#version 330 core

layout(location = 0)out vec4 o_Color;

flat in vec3 v_normal;
in vec3 fragPos;
in vec3 v_Color;
in vec2 v_texCoord;

uniform sampler2D ourTexture;

void main() {

	vec3 lightPos = vec3(0, 0, 1);

	vec3 norm = normalize(v_normal);
	vec3 lightDir = normalize(lightPos);
	float diff = max(dot(norm, lightDir), 0.0);

	vec3 result = (diff+0.3) * v_Color;

	//o_Color = vec4(result,1);
	o_Color = texture(ourTexture, v_texCoord);
	//o_Color = vec4(v_Color,1);

};

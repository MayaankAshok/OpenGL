  #version 330 core

layout(location = 0)in vec3 position;
layout(location = 1)in vec2 texCoord;
layout(location = 2)in vec3 i_translation;
layout(location = 3)in vec2 i_rotation;
layout(location = 4)in vec2 i_matIndex;


uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
//uniform mat4 u_normal;

uniform ivec2 texID[6] = { {1,0},{1,0},{1,0},{1,0},{0,0},{2,0} };

uniform sampler2D Tex2;

out vec2 v_texCoord;
out vec3 v_Color;

const float PI = 3.14159;

mat3 fRotY(float _angle) {
	return mat3(
		cos(_angle), 0, -sin(_angle),
		0, 1, 0,
		sin(_angle), 0, cos(_angle)

	);
}

mat3 fRotX(float _angle) {
	return mat3(
		1, 0, 0,
		0, cos(_angle), -sin(_angle),
		0, sin(_angle), cos(_angle)
	);
}

void main() {

	//ivec2 curTexID = (texID[faceIndex]);
	//vec4 curTexID = texture2D(Tex2, vec2(0.01+ float(faceIndex)/6,float(5-i_matIndex)/5 ), 0.0);
	v_texCoord = (vec2(int(i_matIndex.x),15-int(i_matIndex.y))+texCoord)/16;
	vec3 tempPos = position;
	if (i_rotation.x != 0 ) {
		tempPos = fRotY(float(i_rotation.x)*3.14159/2) * position;
	}
	if (i_rotation.y != 0) {
		tempPos = fRotX(float(i_rotation.y)* (PI / 2)) * position;

	}
	
	vec4 tempPos2 = u_model * vec4(tempPos,1);
	tempPos2 += vec4(i_translation,1);

	//v_texCoord = texCoord;
	v_Color = vec3(i_matIndex/4,1);
	//vec4 tempPos = (u_model * vec4(position, 1)) + vec4(i_translation, 1);
	gl_Position = u_projection * u_view * tempPos2;

};


#version 330 core

layout (location =0)in vec3 position;
layout(location = 1)in vec3 normal;
layout(location = 2)in vec2 texCoord;
layout(location = 3)in int faceIndex;
layout(location = 4)in vec3 i_translation;
layout(location = 5)in float i_matIndex;

//layout(location = 5)in vec3 offset2;
//uniform vec3 u_Color;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_normal;

uniform ivec2 texID[6] = { {1,0},{1,0},{1,0},{1,0},{0,0},{2,0} };

uniform sampler2D Tex2;

out vec2 v_texCoord;
out vec3 v_Color;

void main() {

	//ivec2 curTexID = (texID[faceIndex]);
	vec4 curTexID = texture2D(Tex2, vec2(0.01+ float(faceIndex)/6,float(5-i_matIndex)/5 ), 0.0);
	v_texCoord = (vec2(int(16*curTexID.x),15-int(16*curTexID.y))+texCoord)/16;

	//v_texCoord = texCoord;
	v_Color = vec3(curTexID);
	vec4 tempPos = (u_model * vec4(position, 1)) + vec4(i_translation, 1);
	gl_Position = u_projection * u_view * tempPos;

};

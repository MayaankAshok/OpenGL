#version 330 core

layout (location =0)in vec3 position;
layout(location = 1)in vec3 normal;
layout(location = 2)in vec2 texCoord;
layout(location = 3)in int faceIndex;
layout(location = 4)in vec3 offset1;
layout(location = 5)in vec3 offset2;


//uniform vec3 u_Color;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_normal;
uniform ivec2 texID[6] = { {1,0},{1,0},{1,0},{1,0},{0,0},{2,0} };

out vec2 v_texCoord;

void main() {

	ivec2 curTexID = (texID[faceIndex]);
	v_texCoord = (vec2((curTexID.x),(15-curTexID.y))+texCoord)/16;

	//v_Color = vec3(1, 0, 1);
	vec4 tempPos = (u_model * vec4(position, 1)) + vec4(offset1, 1)+ vec4(offset2,1);
	gl_Position = u_projection * u_view * tempPos;

};

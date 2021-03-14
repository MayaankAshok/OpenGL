#version 330 core

layout (location =0)in vec3 position;
layout(location = 1)in vec3 normal;
layout(location = 2)in vec2 texCoord;
layout(location = 3)in int faceIndex;
//layout(location = 4)in vec3 offset;


uniform vec3 u_Color;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_normal;
uniform ivec2 texID[6] = { {1,0},{1,0},{1,0},{1,0},{0,0},{2,0} };

out vec3 v_normal;
out vec3 fragPos;
out vec3 v_Color;
out vec2 v_texCoord;

void main() {
	v_Color = u_Color; 

	fragPos = vec3(u_model*vec4(position,1));
	v_normal = vec3(u_normal *vec4(normal, 1));

	ivec2 curTexID = (texID[faceIndex]);

	v_texCoord = (vec2((curTexID.x),(15-curTexID.y))+texCoord)/16;

	v_Color = vec3(float(curTexID.x)*0.5,0, 0.3);
	//gl_FrontColor = vec4(1, 1, 1, 1);
	//vec4 tempPos = (u_model * vec4(position, 1)) + vec4(offset, 1);
	//gl_Position = u_projection * u_view * tempPos;
	gl_Position = u_projection * u_view * u_model*vec4(position,1);

};

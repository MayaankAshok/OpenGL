#version 330 core

layout(location = 0)out vec4 o_Color;

in vec2 v_texCoord;
in vec3 v_Color;

uniform sampler2D Tex1;
uniform sampler2D Tex2;

void main() {

	
	o_Color = texture(Tex1, v_texCoord);
	//o_Color = vec4(v_Color,1);

};

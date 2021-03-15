#version 330 core

layout(location = 0)out vec4 o_Color;

in vec2 v_texCoord;

uniform sampler2D ourTexture;

void main() {

	
	o_Color = texture(ourTexture, v_texCoord);
	//o_Color = vec4(v_Color,1);

};

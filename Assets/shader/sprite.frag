#version 330

in vec2 fragTexCoord;
out vec4 outColor;
uniform sampler2D uTexture;
uniform vec4 colour;

void main()
{
	outColor = colour * texture(uTexture, fragTexCoord);
}
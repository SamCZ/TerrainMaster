#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;

void main() {
	float c = (Normal.x + Normal.y + Normal.z) / 3.0;
	c = clamp(c, 0.1, 0.8);
    FragColor = vec4(c, c, c, 1.0);
}
#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 TexCoords1[];

out vec2 TexCoords;
out vec3 Normal;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

vec3 calcNormal(vec4 dp1, vec4 dp2, vec4 dp3) {
	vec3 v0 = gl_in[0].gl_Position.xyz + dp1.xyz;
	vec3 v1 = gl_in[1].gl_Position.xyz + dp2.xyz;
	vec3 v2 = gl_in[2].gl_Position.xyz + dp3.xyz;

	// edges of the face/triangle
    vec3 U = v1 - v0;
    vec3 V = v2 - v0;
    
    float nx = (U.y * V.z) - (U.z * V.y);
    float ny = (U.z * V.x) - (U.x * V.z);
    float nz = (U.x * V.y) - (U.y * V.x);

    return normalize(vec3(nx, ny, nz));
}

void main() {
	vec3 normal = calcNormal(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);
	for (int i = 0; i < gl_in.length(); ++i) {
		vec4 vertexPos = gl_in[i].gl_Position;
		gl_Position = ProjectionMatrix * ViewMatrix * vertexPos;
		TexCoords = TexCoords1[i];
		Normal = normal;

		EmitVertex();
	}
	EndPrimitive();
}
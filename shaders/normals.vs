#version 400

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vUV;

uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 MVP;
uniform bool HasUV;

out vec2 uv;
out vec3 color;

void main()
{
	if (HasUV) {
		uv = vUV;
	} else {
		uv = vec2(0,0);
	}
	vec3 n = vNormal;
	color = vec3((n.x + 1.0f) / 2.0f, (n.y + 1.0f) / 2.0f, (n.z + 1.0f) / 2.0f);
	gl_Position = MVP * vec4(vPos, 1.0f);
}

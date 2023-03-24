#version 400

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormals;
layout (location = 2) in vec2 vUV;

uniform mat3 NormalMatrix;
uniform mat4 ModelViewMatrix;
uniform mat4 MVP;
uniform bool HasUV;
uniform vec3 lightPos;
uniform vec3 eyePos;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;

out vec2 uv;
out vec3 posEye;
out vec3 normEye;

void main()
{
	if (HasUV) {
		uv = vUV;
	} else {
		uv = vec2(0,0);
	}
	normEye = normalize(NormalMatrix * vNormals);
	posEye = vec3(ModelViewMatrix * vec4(vPos, 1.0));

	gl_Position = MVP * vec4(vPos, 1.0);
}


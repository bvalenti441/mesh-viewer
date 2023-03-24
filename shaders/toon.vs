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

out vec3 color;

void main()
{
	vec3 normEye = normalize(NormalMatrix * vNormals);
	vec3 posEye = vec3(ModelViewMatrix * vec4(vPos, 1.0));
	
	vec3 lightDir = normalize(lightPos - vec3(ModelViewMatrix * vec4(posEye, 1.0)));
	vec3 diffuse = vec3(0, 0, 0);
	if (dot(lightDir, normEye) < 0) {
		diffuse = vec3(1.0f - material.diffuse.x, 1.0f - material.diffuse.y, 1.0f - material.diffuse.z) * max(dot(normEye, lightDir), 0.0);
	}
	else {
		diffuse = material.diffuse * max(dot(normEye, lightDir), 0.0);
	}
	vec3 viewDir = normalize(eyePos - posEye);
	vec3 reflectDir = reflect(-lightDir, normEye);
	vec3 specular = material.specular * pow(max(dot(viewDir, reflectDir), 0.0), 32);
	
	color = material.ambient + diffuse + specular;
	
	gl_Position = MVP * vec4(vPos, 1.0);
}

#version 400

uniform sampler2D diffuseTexture;
uniform bool HasUV;

in vec2 uv;
in vec3 color;
out vec4 FragColor;

void main()
{
	if(HasUV) {
		vec3 c = color * texture(diffuseTexture, uv*10.0f).xyz; 
		FragColor = vec4(c, 1.0f);
	} 
	else {
		FragColor = vec4(color.x, color.y, color.z, 1.0f);
	}
}

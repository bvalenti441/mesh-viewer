#version 400

in vec3 localColor;
out vec4 FragColor;

void main()
{
   FragColor = vec4(localColor.x, localColor.y, localColor.z, 1.0);
}
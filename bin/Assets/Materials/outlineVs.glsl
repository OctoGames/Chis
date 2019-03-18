#version 330 core

in vec2 uv0;    // text coords
in vec4 vertex; // vertices de pos
in vec3 normal; 

uniform mat4 modelViewProjMat; // constante de programa
uniform mat4 normalMat;

out vec3 NormalOut; 
out vec2 vUv0; // out del vertex shader

void main() 
{

	NormalOut = normalize(vec3( mat3(normalMat) * normal));

	vUv0 = uv0;

	vec4 position = vertex - vec4(NormalOut,1) * 0.1;
	//outline_width

	gl_Position = modelViewProjMat * position; // obligatorio
}
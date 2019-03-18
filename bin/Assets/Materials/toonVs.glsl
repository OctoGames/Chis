#version 330 core

uniform mat4 modelViewProjMat; // constante de programa

in vec4 vertex; // vertices de pos
in vec2 uv0;    // text coords

out vec2 vUv0; // out del vertex shader

//----------------------------------------------------------------------------
 
in vec3 normal; 

out vec3 NormalOut; 

uniform mat4 normalMat;

//----------------------------------------------------------------------------

void main() 
{
	//------------------------------------------------------------------------
	NormalOut = mat3(normalMat) * normal;
	//------------------------------------------------------------------------
	
	vUv0 = uv0;
	gl_Position = modelViewProjMat * vertex; // obligatorio
}
#version 330 core

uniform sampler2D textura;

in vec2 vUv0; // in del vertex shader

out vec4 fFragColor; // out del fragment shader

//----------------------------------------------------------------------------
	
in vec3 NormalOut;
	
uniform vec3 lightDiffuse;
uniform vec4 lightDirection;
uniform vec3 color;
uniform float shadow_level;
uniform float shadow_color;

//----------------------------------------------------------------------------

void main() 
{
	//vec3 col = vec3(1,0.65,0); //vec3(texture(textura, vUv0));
	
	vec3 auxNormalOut;
	vec4 auxLightDirection;
	
	// diffuse
	auxNormalOut = normalize(NormalOut);
	auxLightDirection = normalize(lightDirection);
	float diff = max(dot(auxNormalOut, vec3(auxLightDirection)), 0.0);


	if(diff > shadow_level) diff = 1;
	
	else diff = shadow_color;
	



	vec3 diffuse = lightDiffuse * diff * color;
	
	//----------------------------------------------------------------------------
	
	fFragColor = vec4(diffuse, 1);
}
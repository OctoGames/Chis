#version 330 core

in vec2 vUv0;

uniform sampler2D RTT0;

out vec4 fragColor;

const vec3 WsRGB = vec3(0.2125, 0.7154, 0.0721);

void main()
{
	vec3 sceneColor = vec3(texture(RTT0, vUv0));
	float lum = dot(sceneColor, WsRGB);
	fragColor = vec4(lum, lum, lum, 1.0);
}
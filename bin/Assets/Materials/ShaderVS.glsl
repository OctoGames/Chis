#version 330 core

in vec4 vertex;
in vec2 uv0;

uniform mat4 modelViewProjMat;
uniform float timerSin;

out vec2 vUv0;
out vec2 vUv1;

void main()
{
	vUv0 = uv0;
	vUv1 = uv0;

	vUv1.s -= 0.5;
	vUv1.s = vUv1.s * timerSin;
	vUv1.s += 0.5;

	vUv1.t -= 0.5;
	vUv1.t = vUv1.t * timerSin;
	vUv1.t += 0.5;

	gl_Position = modelViewProjMat * vertex;
}
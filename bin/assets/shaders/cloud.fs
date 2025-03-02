#version 330 core

uniform vec4 uColor;
varying vec4 vColor;

void main()
{
	gl_FragColor = vColor + uColor * 0.01;
}

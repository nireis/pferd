#version 120

uniform mat4 mvp;

attribute vec2 in_position;
attribute float in_color;

varying float color;


void main(void) 
{
	color = in_color;
	gl_Position = mvp * vec4(in_position,0.0,1.0);
}
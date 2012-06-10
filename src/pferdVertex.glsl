#version 120

uniform mat4 mvp;

attribute vec2 in_position;
attribute float in_color;

varying float color;


void main(void) 
{
	gl_PointSize = 4.0;

	color = in_color;
	gl_Position = mvp * vec4(in_position, 1.0, 1.0);
}
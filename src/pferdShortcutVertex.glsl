#version 120

uniform mat4 mvp;

attribute vec2 in_position;
attribute float in_color;

varying vec4 color;

vec4 transferFunction(in float value)
{	
	float r;
	float g;
	float b;
	r = sin(value * 3.14/2.0);
	g = sin(value * 3.14);
	b = cos(value* 3.14/2.0);
	return vec4(r,g,b,1.0);
}


void main(void) 
{
	gl_PointSize = 4.0;

	color = transferFunction(in_color);
	gl_Position = mvp * vec4(in_position, 1.0, 1.0);
}
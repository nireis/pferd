#version 120

uniform mat4 mvp;

attribute vec2 in_position;
attribute float in_color;
attribute vec2 in_uv_coord;

varying float color;
varying vec2 uv_coord;

void main(void) 
{
	color = in_color;
	uv_coord = in_uv_coord;
	gl_Position = mvp * vec4(in_position, 1.0, 1.0);
}
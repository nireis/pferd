#version 120

uniform mat4 mvp_matrix;
uniform mat4 tex_matrix;
uniform vec3 camera_pos;

attribute vec3 in_position;
attribute vec3 in_color;


varying vec3 color;
varying vec4 out_position;

void main(void) 
{
	color = in_color;
	out_position = vec4(in_position,1.0);
	
	gl_Position = mvp_matrix * vec4(in_position.xyz, 1.0);
}
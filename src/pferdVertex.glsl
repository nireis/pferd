#version 120

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;

attribute vec2 in_position;
attribute float in_color;

varying float color;


void main(void) 
{
	color = in_color;
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(in_position.x*0.6649,in_position.y,0.0, 1.0);
}
#version 120

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;

in vec2 in_position;
in float in_color;

out float color;


void main(void) 
{
	color = in_color;
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(in_position,0.0, 1.0);
}
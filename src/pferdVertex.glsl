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
	float pi = 3.141592;
	float pos_radian = (pi/180.0)*in_position.y;
	float y_mercator = log((1.0 + sin(pos_radian))/cos(pos_radian));
	float x_mercator = (pi/180.0)*in_position.x;
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(x_mercator,y_mercator,0.0, 1.0);
}
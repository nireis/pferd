#version 120

uniform mat4 mvp;

attribute vec2 in_position;
attribute float in_color;

varying float color;


void main(void) 
{
	gl_PointSize = 4.0;

	color = in_color;
	float pi = 3.141592;
	float pos_radian = (pi/180.0)*in_position.y;
	float y_mercator = log((1.0 + sin(pos_radian))/cos(pos_radian));
	float x_mercator = (pi/180.0)*in_position.x;
	gl_Position = mvp * vec4(x_mercator,y_mercator,1.0, 1.0);
}
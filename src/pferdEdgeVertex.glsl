#version 120

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;

attribute vec2 in_position;
attribute float in_color;
attribute vec3 in_normal;

varying float color;


void main(void) 
{
	color = in_color;
	//in_normal = normalize(in_normal); 
	vec3 in_normal_normalized = normalize(vec3(in_normal));

	float pi = 3.141592;
	float pos_radian = (pi/180.0)* in_position.y;
	float y_mercator = log((1.0 + sin(pos_radian))/cos(pos_radian));
	float x_mercator = (pi/180.0)*in_position.x;
	
	float normal_radian =(pi/180.0)* in_normal_normalized.y;
	float y_normal_mercator = log((1.0 + sin(normal_radian))/cos(normal_radian));
	float x_normal_mercator = (pi/180.0)*in_normal_normalized.x;
	
	vec2 offsetPos = vec2(x_mercator,y_mercator) + (vec2(x_normal_mercator,y_normal_mercator)*0.00002);
	
	gl_Position = projection_matrix * view_matrix * model_matrix * vec4(offsetPos,0.0, 1.0);
}


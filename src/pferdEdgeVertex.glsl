#version 120

uniform mat4 mvp;

attribute vec2 in_position;
attribute float in_color;
attribute vec3 in_normal;

varying vec4 color;

vec4 transferFunction(in float value)
{	
	float r;
	float g;
	float b;
	r = 0.0;
	g = sin(value * 3.14);
	b = cos(value* 3.14);
	if(value > 0.5)
	{
		b = 0.0;
		r = sin((value-0.5) * (3.14/2.0));
	}
	return vec4(r,g,b,1.0);
}

void main(void) 
{
	color = transferFunction(in_color);
	//in_normal = normalize(in_normal); 
	vec3 in_normal_normalized = normalize(vec3(in_normal));

	float pi = 3.141592;
	
	float normal_radian =(pi/180.0)* in_normal_normalized.y;
	float y_normal_mercator = log((1.0 + sin(normal_radian))/cos(normal_radian));
	float x_normal_mercator = (pi/180.0)*in_normal_normalized.x;
	//vec2 offsetPos = vec2(x_mercator,y_mercator) + (vec2(x_normal_mercator,y_normal_mercator)*0.00002);
	
	vec2 offsetPos = in_position + (vec2(x_normal_mercator,y_normal_mercator)*0.00002);
	
	gl_Position = mvp * vec4(offsetPos,(in_color), 1.0);
}


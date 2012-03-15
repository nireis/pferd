#version 120


in float color;

out vec4 outFragColor;

void main(void)
{	
	float c = color * 0.001f;
	outFragColor = vec4(c,c,c,1.0);
	//outFragColor = vec4(0.5,0.8,1.0,1.0);
}
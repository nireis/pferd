#version 120


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
	gl_FragColor = color;
}
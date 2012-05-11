#version 120


varying float color;

void main(void)
{	
	gl_FragColor = vec4(color,color,color,1.0);
}

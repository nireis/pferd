#version 120


varying float color;

void main(void)
{	
	//float c = color * 0.001f;
	//outFragColor = vec4(c,c,c,1.0);
	//outFragColor = vec4(0.5,0.8,1.0,1.0);
	
	gl_FragColor[0] = color;
	gl_FragColor[1] = 0.0;
	gl_FragColor[2] = 0.0;
	gl_FragColor[3] = 1.0;
}
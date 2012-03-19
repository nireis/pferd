#version 120

varying float color;

//out vec4 outFragColor;

void main(void)
{	
	float c = color * 0.001f;
  gl_FragColor[0] = c;
  gl_FragColor[1] = c;
  gl_FragColor[2] = c;
  gl_FragColor[3] = 1.0;
	//outFragColor = vec4(0.5,0.8,1.0,1.0);
	//outFragColor = vec4(c,c,c,1.0);
}

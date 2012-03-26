#version 120

uniform sampler2D mytexture;

varying float color;
varying vec2 uv_coord;

void main(void)
{	
	gl_FragColor = texture2D(mytexture, uv_coord);
}
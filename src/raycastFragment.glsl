//Fragmentshader

#version 120

uniform mat4 tex_matrix;
uniform vec3 camera_pos;
uniform vec3 tex3D_dim;
uniform sampler2D Tex3D;

varying vec3 color;
varying vec4 out_position;


vec4 transferFunction(in vec4 color)
{	
	/*
	float r;
	float g;
	float b;
	r = sin(color.x * 3.14/2.0);
	g = sin(color.x * 3.14);
	b = cos(color.x * 3.14/2.0);
	return vec4(r,g,b,1.0);
	*/

	
	vec3 v0 = vec3(1.0,0.0,0.0);
	vec3 v1 = vec3(0.0,0.0,1.0);
	
	float opacity = color.w;
	
	return vec4(mix(v0,v1,color.z), opacity);
}

vec4 access3DTex(in vec3 loc, in float sliceCount)
{
	vec4 slice0;
	vec4 slice1;
	
	float x = loc.x;
	float y = loc.y/(sliceCount+1) + (floor(loc.z*(sliceCount-1))/sliceCount);
	slice0 = texture2D(Tex3D,vec2(x,y));
	
	y = loc.y/(sliceCount+1) + (floor((loc.z*(sliceCount-1))+1.0)/sliceCount);
	slice1 = texture2D(Tex3D,vec2(x,y));
	
	//Interpolate between slice0 and slice1
	float weight = (loc.z*(sliceCount-1)) - floor(loc.z*(sliceCount-1));
	
	return (slice0*(1.0 - weight) + slice1*weight);
}


void main(void)
{	
	
	//Calculate coordinates in texture space
	vec3 objPos = (tex_matrix * out_position).xyz;
	vec3 camPos = (tex_matrix * vec4(camera_pos,1.0)).xyz;
	
	//Calculate ray-direction in object space
	vec3 dir = normalize(objPos - camPos);
	
	
	//Accumulated color in xyz, opacity in w
	vec4 out_color = vec4(0.0);
	//Used to travel through the volume
	float step = 0.0;
	float opacity = 0.0;
	
	//Loop for traveling through the volume
	while(opacity < 0.99)
	{	
		//Get values from the 3d texture at the current location
		vec4 texColor = access3DTex((color+(dir*step)), tex3D_dim.z);
		
		//Accumulate color
		out_color += (1.0 - opacity) * transferFunction(texColor)* transferFunction(texColor).w;
		//Accumulate opacity
		opacity += (1.0 - opacity) * transferFunction(texColor).w;
		
		step += 0.011;
		
		//TODO: Speedup the test for the break condition
		//Check if still inside bounding box
		if((color+(dir*step)).x > 1.0
			|| (color+(dir*step)).y > 1.0
			|| (color+(dir*step)).z > 1.0		
			|| (color+(dir*step)).x < 0.0
			|| (color+(dir*step)).y < 0.0
			|| (color+(dir*step)).z < 0.0)
		{
			break;
		}
	}
	
	//Output accumulated color
	gl_FragColor = out_color;
}
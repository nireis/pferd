#version 120


varying float color;


vec3 HSV2RGB(vec3 hsv) {

    vec3 color = vec3(0.0, 0.0, 0.0);
    float f,p,q,t, h,s,v;
    float r = 0.0, g = 0.0, b = 0.0, i;
    
    if(hsv[1] == 0.0)
    {
        if(hsv[2] != 0.0)
        {
            color = vec3(hsv[2]);        // black and white case
        }
    }
    else
    {        // this next step is flawed if the texels are 8 bit!!
        h = hsv.x * 360.0;
        s = hsv.y;
        v = hsv.z;
        if(h == 360.0)
        {   
            h = 0.0;
        }

        h /= 60.0;
        i = floor(h);
        f = h-i;
        p = v * (1.0 - s);
        q = v * (1.0 -(s * f));
        t = v * (1.0 -(s * (1.0 -f)));
        if( i == 0)        {             r = v;            g  = t; b = p; }
        else if( i== 1 )   {             r = q;            g  = v; b = p; }
        else if( i== 2 )   {             r = p;            g  = v; b = t; }
        else if( i== 3 )   {             r = p;            g  = q; b = v; }
        else if( i== 4 )   {             r = t;            g  = p; b = v; }
        else if( i== 5 )   {             r = v;            g  = p; b = q; }
        color.r = r;        color.g = g;         color.b = b;
    }
    return color;

}


void main(void)
{	
	//float c = color * 0.001f;
	//outFragColor = vec4(c,c,c,1.0);
	//outFragColor = vec4(0.5,0.8,1.0,1.0);
	
	//gl_FragColor[0] = color;
	//gl_FragColor[1] = 0.0;
	//gl_FragColor[2] = 0.0;
	//gl_FragColor[3] = 1.0;
	
	gl_FragColor = vec4(HSV2RGB(vec3((color,1.0,1.0)),1.0);
	
}

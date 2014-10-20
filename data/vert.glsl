/*
	Character data
	Packed uint32 color(16) flags(8) ascii(8)
*/
layout(location=0) in uint chardata;
/*
	Grid data
	vec4(x,y,w,h)
	x,y in pixels
	w,h in characters
*/
layout(location=1) in vec4 grid;//x,y,w,h

// Window size in pixels
layout(std140) uniform resolutionUBO
{
	vec2 resolution;
};

flat out uint charoffset;
flat out vec2 charsizendc;

//------------------------------------------------------------------------------

vec2 PixToNDC(vec2 v)
{
	return vec2( (2.0f*v.x)/resolution.x-1.0f, -(2.0f*v.y)/resolution.y+1.0f);
}

void main()
{
	charoffset=(chardata&0xFF000000)>>24;

	vec2 gridpos=PixToNDC(grid.xy);

	int margin=int(grid.z);

	charsizendc=vec2(8.f * 2.f/resolution.x,8.f * 2.f/resolution.y);
	vec2 charpos;
	charpos.x=(gl_VertexID%margin) * 8.f * 2.f/resolution.x;
	charpos.y=-(gl_VertexID/margin) * 8.f * 2.f/resolution.y;
	vec2 position=gridpos+charpos;
	gl_Position=vec4(position, 0.0, 1.0);

}
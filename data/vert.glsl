/*
	Character data
		GLushort colors;
		GLubyte flags;
		GLubyte c;
		GLshort x,y;
*/
layout(location=0) in ivec2 chardata;

uniform vec2 resolution;

flat out uint charoffset;
flat out vec2 charsizendc;

//----------------------------------------------------------------------------

// Convert pixel position to NDC according to resolution.
vec2 PixToNDC(vec2 v)
{
	return vec2( (2.0f*v.x)/resolution.x-1.0f, -(2.0f*v.y)/resolution.y+1.0f);
}

// Get character position.
vec2 GetPos()
{
	vec2 u=vec2(chardata.y&0xFFFF,chardata.y>>16&0xFFFF);
	return PixToNDC(u);
}

// Get character value.
uint GetChar()
{
	return chardata.x>>24&0xFF;
}

void main()
{
	charsizendc=vec2(8.f * 2.f/resolution.x,8.f * 2.f/resolution.y);
	charoffset=GetChar();
	vec2 position=GetPos();
	gl_Position=vec4(position, 0.0, 1.0);
}

constexpr char const* SHADERS_STR[3]={
// Vertex
// -----------------------------------------------------------------------------
R"(
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
out vec4 charColor;

// Convert pixel position to NDC according to resolution.
vec2 PixToNDC(in vec2 v)
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

vec4 GetColor()
{
	uint fc=chardata.x>>8&0xFF;
	float blue=(fc&3)/3.0f;
	float green=(fc>>2&3)/3.0f;
	float red=(fc>>4&3)/3.0f;
	return vec4(red,green,blue,1);
}

void main()
{
	charsizendc=vec2(8.f * 2.f/resolution.x,8.f * 2.f/resolution.y);
	charoffset=GetChar();
	vec2 position=GetPos();
	charColor=GetColor();
	gl_Position=vec4(position, 0.0, 1.0);
})",
// Geometry
// -----------------------------------------------------------------------------
R"(
layout(points) in;
layout(triangle_strip,max_vertices=4) out;

flat in uint charoffset[];
flat in vec2 charsizendc[];
in vec4 charColor[];

out vec2 uv;
out vec4 fragColor;

void main()
{
	const float tw=256.0f;

	vec2 uvc=vec2((charoffset[0]-32)/tw,0);
	const float uvx=1.f/tw;
	const float uvy=1.f;

	float sx=charsizendc[0].x;
	float sy=charsizendc[0].y;

	vec4 v=gl_in[0].gl_Position;

	fragColor=charColor[0];

	gl_Position=v;
	uv=uvc+vec2(0,0);
	EmitVertex();

	gl_Position+=vec4(0,-sy,0,0);
	uv=uvc+vec2(0,uvy);
	EmitVertex();

	gl_Position+=vec4(sx,sy,0,0);
	uv=uvc+vec2(uvx,0);
	EmitVertex();

	gl_Position+=vec4(0,-sy,0,0);
	uv=uvc+vec2(uvx,uvy);
	EmitVertex();
})",
// Fragment
// -----------------------------------------------------------------------------
R"(
uniform sampler2D tex;

in vec2 uv;

in vec4 fragColor;
out vec4 outColor;

void main()
{
	float r=texture(tex,uv).r;
	outColor=r*fragColor;
})"};

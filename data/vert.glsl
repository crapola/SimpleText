layout(location=0) in uint chardata;
layout(location=1) in uvec2 grid;

layout(std140) uniform uniblock
{
	vec2 res;
};

const int margin=10;

flat out vec2 charsizendc;

vec2 PixToNDC(vec2 v)
{
	return vec2( (2.0f*v.x)/res.x-1.0f, -(2.0f*v.y)/res.y+1.0f);
}

void main()
{
	uint foo=chardata;

	vec2 gridpos=PixToNDC(unpackSnorm2x16(grid.x)*65535.f);

	charsizendc=vec2(8.f * 2.f/res.x,8.f * 2.f/res.y);
	vec2 charpos;
	charpos.x=(gl_VertexID%margin) * 9.f * 2.f/res.x;
	charpos.y=-(gl_VertexID/margin) * 9.f * 2.f/res.y;
	vec2 position=gridpos+charpos;
	gl_Position=vec4(position, 0.0, 1.0);

}
layout(location=0) in uint chardata;
layout(location=1) in ivec2 grid;

layout(std140) uniform uniblock
{
	vec2 res;
};

const int margin=10;

flat out vec2 charsizendc;

vec2 ScreenPosToNDC(int x,int y)
{
	return vec2( (2.0f*x)/res.x-1.0f, -(2.0f*y)/res.y+1.0f);
}

void main()
{
	uint foo=chardata>>2;

	int gridx=grid.x&0xFFFF;
	int gridy=int((grid.x&0xFFFF0000)>>16);

	vec2 gridpos=ScreenPosToNDC(gridx,gridy);
	charsizendc=vec2(8.f * 2.f/res.x,8.f * 2.f/res.y);
	vec2 charpos;
	charpos.x=(gl_VertexID%margin) * 9.f * 2.f/res.x;
	charpos.y=-(gl_VertexID/margin) * 9.f * 2.f/res.y;
	vec2 position=gridpos+charpos;
	gl_Position=vec4(position, 0.0, 1.0);

}
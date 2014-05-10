layout(location=0) in uint chardata;

layout(std140) uniform uniblock
{
	vec2 res;
};

const int margin=10;

flat out vec2 charsizendc;

void main()
{
	uint foo=chardata>>2;
	charsizendc=vec2(8.f * 2.f/res.x,8.f * 2.f/res.y);
	float x=mod(gl_VertexID,margin) * 9.f * 2.f/res.x;
	float y=-(gl_VertexID/margin) * 9.f * 2.f/res.y;
	vec2 position=vec2(x,y);
	gl_Position = vec4(position, 0.0, 1.0);

}
layout(points) in;
layout(triangle_strip,max_vertices=4) out;

layout(std140) uniform uniblock
{
	vec2 res;
};

void main()
{
	float sx=8.f * 2.f/res.x;
	float sy=8.f * 2.f/res.y;

	vec4 v=gl_in[0].gl_Position;
	gl_Position=v;
	v+=vec4(sx,sy,0,0);
	EmitVertex();

	gl_Position+=vec4(0,sy,0,0);
	EmitVertex();

	gl_Position+=vec4(sx,-sy,0,0);
	EmitVertex();

	gl_Position+=vec4(0,sy,0,0);
	EmitVertex();

}
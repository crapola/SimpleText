layout(points) in;
layout(triangle_strip,max_vertices=4) out;

flat in vec2 charsizendc[];

void main()
{
	float sx=charsizendc[0].x;
	float sy=charsizendc[0].y;

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
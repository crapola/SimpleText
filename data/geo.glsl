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
}

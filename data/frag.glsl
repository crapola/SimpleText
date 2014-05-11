layout(location=0) uniform sampler2D tex;

in vec2 uv;

out vec4 outColor;

void main()
{
	float tx=texture(tex,uv).r;
	outColor=vec4(tx,tx,0,1);
}
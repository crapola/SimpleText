uniform sampler2D tex;

in vec2 uv;

out vec4 outColor;

void main()
{
	float r=texture(tex,uv).r;
	outColor=vec4(r,r,0,r);
	//outColor=vec4(1,1,1,1);
}
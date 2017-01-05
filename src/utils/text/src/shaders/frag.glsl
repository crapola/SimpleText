uniform sampler2D tex;

in vec2 uv;

in vec4 fragColor;
out vec4 outColor;

void main()
{
	float r=texture(tex,uv).r;
	outColor=r*fragColor;
}

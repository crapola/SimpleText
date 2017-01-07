uniform sampler2D tex;

in vec2 uv;

in Color
{
	vec4 front;
	vec4 back;
} inColor;

out vec4 outColor;

void main()
{
	float r=texture(tex,uv).r;
	outColor=r*inColor.front;
}

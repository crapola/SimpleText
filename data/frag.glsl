layout(location=0) uniform sampler2D tex;

in vec2 uv;

out vec4 outColor;

void main()
{
	outColor=vec4(texture(tex,uv).r,0,0,1);
}
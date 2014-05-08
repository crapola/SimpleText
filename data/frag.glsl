out vec4 outColor;

layout(std140) uniform uniblock
{
	vec2 res;
};

void main()
{
	vec2 fp=vec2(gl_FragCoord.x/res.x-0.5,gl_FragCoord.y/res.y-0.5);
	float p=1-distance(fp,vec2(0,0));
	outColor = vec4(p,p,p,1);
}
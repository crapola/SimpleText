out vec4 outColor;
flat in vec2 poz;
void main()
{
	vec2 fp=vec2(gl_FragCoord.x/800-0.5,gl_FragCoord.y/600-0.5);
	vec2 bla=poz*fp;
	float p=2*distance(fp,bla);
	outColor = vec4(p,p,p,1);
}
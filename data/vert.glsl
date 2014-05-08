layout(location=0) in vec2 position;

void main()
{
	gl_Position = vec4(position, 0.0, 1.0)+vec4(0.35,0.3,0,0);
}
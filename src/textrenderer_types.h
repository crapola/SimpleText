struct Character
{
	// Front and back colors
	GLushort colors;
	// Flags
	GLubyte flags;
	// Character code
	GLubyte c;		//ivec2.x 24
	// Position in pixels
	GLshort x,y;	//ivec2.y
};
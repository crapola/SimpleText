#ifndef TEXTRENDERERM1_H
#define TEXTRENDERERM1_H
#include <algorithm>
#include <string>
#include <vector>
#include "utils/gl/buffer.h"
#include "utils/loadstring.h"
#include "utils/gl/shader.h"
#include "utils/gl/program.h"
#include "utils/gl/texture.h"
#include "utils/gl/logerrors.h"

#define TEST(x) gl::LogErrors(x);

class TextRendererM1
{
public:
	typedef size_t TextHandle;

	TextRendererM1(const gl::Buffer& resBuf);
	~TextRendererM1();

	TextHandle Create(int x,int y,int w,int h);
	void Delete(TextHandle);
	// Draw everything
	void Draw();
	void Print(int g,int x,int y,const std::string& s);
private:
	struct Grid
	{
		GLfloat x,y; // Pixels
		GLfloat w,h; // Number of characters
	};
	struct Character
	{
		GLushort colors;
		GLubyte grid:4;
		GLubyte flags:4;
		GLubyte c;
	};
	std::vector<Grid> _grids;
	std::vector<Character> _chars;
	gl::Buffer _charBuf;
	gl::Program _program;
	gl::Texture _texture;
	GLint _gridAttrib;
};

#endif // TEXTRENDERERM1_H


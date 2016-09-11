#pragma once
#include <algorithm>
#include <map>
#include <string>
#include "utils/backcache.hpp"
#include "utils/loadstring.h"
#include "utils/gfx/gl/buffer.h"
#include "utils/gfx/gl/shader.h"
#include "utils/gfx/gl/program.h"
#include "utils/gfx/gl/texture.h"
#include "utils/gfx/gl/logerrors.h"

class TextRenderer
{
public:
	TextRenderer();
	~TextRenderer();
	// Add a number of characters to the end
	void Add(size_t count);
	// Delete characters in range [from,to[
	void Delete(size_t from,size_t to);
	// Draw everything
	void Draw();

	void Write(size_t offset,const std::string& s);
	void Resolution(int width,int height);
private:
	void UploadWholeBuffer();

	struct Character
	{
		GLushort colors;
		GLubyte flags;
		GLubyte c;		//x 24
		GLshort x,y;	//y
	};

	BackCache<Character> _chars;
	gl::Buffer _charBuf;
	gl::Program _program;
	gl::Texture _texture;
};

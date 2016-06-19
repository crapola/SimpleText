#pragma once
#include <algorithm>
#include <map>
#include <string>
#include "utils/backcache.hpp"
#include "utils/indexmap.h"
#include "utils/loadstring.h"
#include "utils/gfx/gl/buffer.h"
#include "utils/gfx/gl/shader.h"
#include "utils/gfx/gl/program.h"
#include "utils/gfx/gl/texture.h"
#include "utils/gfx/gl/logerrors.h"

class TextRendererM1
{
public:
	typedef size_t TextHandle;

	TextRendererM1();
	~TextRendererM1();

	TextHandle Create(int x,int y,int w,int h);
	void Delete(TextHandle);
	// Draw everything
	void Draw();
	void Print(size_t offset,const std::string& s);
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

	// Handles
	IndexMap _handles;
};

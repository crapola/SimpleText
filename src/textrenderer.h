#pragma once
#include "misc/backcache.hpp"
#include "gl/buffer.h"
#include "gl/shader.h"
#include "gl/program.h"
#include "gl/texture.h"
#include "gl/vao.h"
#include <functional>
#include <string>

class TextRenderer
{
	#include "textrenderer_types.h"
public:
	TextRenderer();
	~TextRenderer();
	// Add a number of characters to the end.
	void Add(size_t count);
	// Delete characters in range [from,to[.
	void Delete(size_t from,size_t to);
	// Draw everything.
	void Draw();
	// Apply function to range.
	void ForEach(size_t from,size_t to,std::function<Character(Character)> f);
	// Arrange in a box
	void Paragraph(size_t offset,size_t lenght,int x_px,int y_px,int width);
	void Write(size_t offset,const std::string& s);
	// Set pixel resolution.
	void Resolution(int width,int height);
private:
	void UploadWholeBuffer();

	BackCache<Character> _chars;
	gl::Buffer _charBuf;
	gl::Program _program;
	gl::Texture _texture;
	gl::VAO _vao;
};

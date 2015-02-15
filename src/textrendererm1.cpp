#include <iostream>
#include "textrendererm1.h"
#include "fonttexture.h"

#define TEST(x) gl::LogErrors(x);

using namespace std;

// Helper to glBufferSubData part of a vector.
template<typename C>
void VSubData(GLenum p_target,const C& p_container,size_t p_from,size_t p_count)
{
	constexpr size_t valueSize=sizeof(typename C::value_type);
	glBufferSubData(p_target,
					valueSize*p_from,
					valueSize*p_count,
					p_container.data()+p_from
				   );
}

TextRendererM1::TextRendererM1(const gl::Buffer& p_resBuf):
	_chars(),
	_charBuf(),_program(),_texture()
{
	// Fill
	_chars.resize(200);
	int i=0;
	for_each(_chars.begin(),_chars.end(),[&i](Character& c)
	{
		c.colors=33;
		c.flags=33;
		c.c=(i+32)%256;

		c.x=g_fontTexture.charSize*(i%20);
		c.y=g_fontTexture.charSize*(i/20);
		i++;
	});
	// Program
	gl::Shader vs(GL_VERTEX_SHADER),gs(GL_GEOMETRY_SHADER),
	fs(GL_FRAGMENT_SHADER);
	vs.Compile(LoadString("data/vert.glsl"));
	gs.Compile(LoadString("data/geo.glsl"));
	fs.Compile(LoadString("data/frag.glsl"));
	_program.Attach(vs);
	_program.Attach(gs);
	_program.Attach(fs);
	_program.Link();
	_program.Bind();

	// Texture
	constexpr int cs=g_fontTexture.charSize;
	constexpr int tw=g_fontTexture.textureWidth;
	const GLubyte* indata=static_cast<const GLubyte*>(g_fontTexture.rawData);
	GLubyte dest[tw*cs]= {0};
	{
		// Convert 1bpp to 1Bpp (GL_RED)
		for (int y=0; y<cs; ++y)
			for (int x=0; x<g_fontTexture.symCount; ++x)
			{
				GLubyte c=indata[x+y*g_fontTexture.symCount];
				for (int b=0; b<cs; ++b)
				{
					if (((c)&(1<<b))!=0)
					{
						constexpr int cs2=cs-1;
						dest[x*cs+(cs2-y)*g_fontTexture.textureWidth+cs2-b]=255;
					}
				}
			}
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,GL_COMPRESSED_RED,g_fontTexture.textureWidth,8,0,GL_RED,GL_UNSIGNED_BYTE,dest);
	GLint samplerLoc=glGetUniformLocation(_program,"tex");
	glUniform1i(samplerLoc,0);
	TEST("Texture")

	// Chars
	_charBuf.Bind(GL_ARRAY_BUFFER);
	glBufferData(GL_ARRAY_BUFFER,sizeof(Character)*_chars.size(),_chars.data(),GL_DYNAMIC_DRAW);
	GLint charAttrib = glGetAttribLocation(_program,"chardata");
	glVertexAttribIPointer(charAttrib,2,GL_UNSIGNED_INT,0,0);
	glEnableVertexAttribArray(charAttrib);
	TEST("Chars")

	// Window size
	GLuint uniblock=glGetUniformBlockIndex(_program,"resolutionUBO");
	//p_resBuf.Bind(GL_UNIFORM_BUFFER);
	glUniformBlockBinding(_program,uniblock,0);
	glBindBufferBase(GL_UNIFORM_BUFFER,uniblock,p_resBuf);
}

TextRendererM1::~TextRendererM1()
{

}

TextRendererM1::TextHandle TextRendererM1::Create(int p_x,int p_y,int p_w,int p_h)
{
	size_t gn=1;
	for (int i=0; i<p_w*p_h; ++i)
	{
		_chars.push_back({0,gn,0,'0'+i});
	};
	// update
	_charBuf.Bind(GL_ARRAY_BUFFER);
	glBufferData(GL_ARRAY_BUFFER,sizeof(Character)*_chars.size(),_chars.data(),GL_DYNAMIC_DRAW);
	return gn;
}

void TextRendererM1::Delete(TextRendererM1::TextHandle p_t)
{

}

void TextRendererM1::Draw()
{
	glDrawArrays(GL_POINTS,0,_chars.size());
}

void TextRendererM1::Print(int p_o, const string& p_s)
{
	// Copy string
	auto it=_chars.begin()+p_o;
	for_each(p_s.begin(),p_s.end(),[&it](const char c)
	{
		*it= {111,111,static_cast<GLubyte>(c),it->x,it->y};
		++it;
	});

	// Send those chars
	_charBuf.Bind(GL_ARRAY_BUFFER);
	VSubData(GL_ARRAY_BUFFER,_chars,p_o,p_s.size());
}

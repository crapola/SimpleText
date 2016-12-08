#include "textrenderer.h"
#include "gl/logerrors.h"
#include "misc/loadstring.h"
#include "fonttexture.h"
#include <algorithm> // for_each
#include <iostream>

#define TEST(x) gl::LogErrors(x);
//#define TEST(x)

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

TextRenderer::TextRenderer():
	_chars(),_charBuf(),_program(),_texture(),_vao()
{
	_vao.Bind();

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
	constexpr int cs=FontTexture::charSize;
	constexpr int tw=2048;
	const GLubyte* indata=static_cast<const GLubyte*>(FontTexture::rawData);
	GLubyte dest[tw*cs]= {0};
	{
		// Convert 1bpp to 1Bpp (GL_RED)
		for(int y=0; y<cs; ++y)
			for(int x=0; x<FontTexture::symCount; ++x)
			{
				GLubyte c=indata[x+y*FontTexture::symCount];
				for(int b=0; b<cs; ++b)
				{
					if(((c)&(1<<b))!=0)
					{
						constexpr int cs2=cs-1;
						dest[x*cs+(cs2-y)*tw+cs2-b]=255;
					}
				}
			}
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,GL_COMPRESSED_RED,tw,8,0,GL_RED,
				 GL_UNSIGNED_BYTE,dest);
	GLint samplerLoc=glGetUniformLocation(_program,"tex");
	glUniform1i(samplerLoc,0);
	TEST("Texture")

	// Chars
	_charBuf.Bind(GL_ARRAY_BUFFER);
	glBufferData(GL_ARRAY_BUFFER,sizeof(Character)*_chars.size(),_chars.data(),
				 GL_DYNAMIC_DRAW);
	GLint charAttrib = glGetAttribLocation(_program,"chardata");
	glVertexAttribIPointer(charAttrib,2,GL_UNSIGNED_INT,0,0);
	glEnableVertexAttribArray(charAttrib);
	TEST("Chars")

	// Window size
	Resolution(800,600);
	TEST("Size")

	_vao.Unbind();

	cout<<"sizeof(Character)="<<sizeof(Character)<<'\n';
}

TextRenderer::~TextRenderer()
{
}

void TextRenderer::Add(size_t p_count)
{
	/* use this later
		vector<Character> extra(p_count,{0,
				0,
				GLubyte('0'),
				GLshort(0),
				GLshort(0)});
		_chars.insert(_chars.end(),extra.begin(),extra.end());
	*/
	size_t l=_chars.size();
	for(size_t i=0; i<p_count; ++i)
	{
		_chars.push_back(
		{
			0,
			0,
			GLubyte('0'+i),
			GLshort(l+i*16%128),
			GLshort(l+i)
		}
		);
	};
	UploadWholeBuffer();
}

void TextRenderer::Delete(size_t p_from,size_t p_to)
{
	_chars.erase(_chars.begin()+p_from,_chars.begin()+p_to);
	UploadWholeBuffer();
}

void TextRenderer::Draw()
{
	if(_chars.Pending())
	{
		cout<<"u";
		_charBuf.Bind(GL_ARRAY_BUFFER);
		// Need to add 1 to get count because BackCache's range is a closed
		// interval.
		VSubData(GL_ARRAY_BUFFER,_chars,_chars.From(),
				 1+_chars.To()-_chars.From());
		cout<<"("<<_chars.From()<<","<<_chars.To()<<")\n";
		_chars.Reset();
	}
	cout<<"d";

	_vao.Bind();
	_program.Bind();
	glDrawArrays(GL_POINTS,0,_chars.size());
}

void TextRenderer::ForEach(size_t p_from,size_t p_len,
						   std::function<Character(Character)> f)
{
	//std::for_each(_chars.begin(),_chars.end(),f);
	for(size_t i=p_from; i<p_from+p_len; ++i)
	{
		//_chars[i]=f(_chars.at(i));
		auto c=_chars.at(i);
		_chars[i]=f(c);
	}
}

void TextRenderer::Paragraph(size_t p_o,size_t p_l,int p_x,int p_y,int p_w)
{
	if(p_w<=0) p_w=p_l;

	for(size_t i=0; i<p_l; ++i)
	{
		Character c
		{
			_chars.at(p_o+i).colors,
			_chars.at(p_o+i).flags,
			_chars.at(p_o+i).c,
			GLshort(p_x+FontTexture::charSize*(i%p_w)),
			GLshort(p_y+FontTexture::charSize*(i/p_w))
		};
		_chars[p_o+i]=c;
	};
}

void TextRenderer::SetColor(size_t p_o,size_t p_l,unsigned short p_color)
{
	for(size_t i=p_o;i<p_o+p_l;++i)
	{
		_chars[i].colors=p_color;
	}
}

void TextRenderer::Write(size_t p_o, const string& p_s)
{
	// Return if offset out of bounds
	if(p_o>=_chars.size())
	{
		return;
	}
	// Copy string
	auto it=_chars.begin()+p_o;
	size_t numChars=min(p_s.size(),_chars.size()-p_o);
	cout<<"numChars="<<numChars;
	for(size_t i=0; i<numChars; ++i)
	{
		_chars[i+p_o].c=static_cast<GLubyte>(p_s[i]);
	}
}

void TextRenderer::Resolution(int p_w,int p_h)
{
	GLint resolution=glGetUniformLocation(_program,"resolution");
	glUniform2f(resolution,static_cast<float>(p_w),
				static_cast<float>(p_h));
}

void TextRenderer::UploadWholeBuffer()
{
	_charBuf.Bind(GL_ARRAY_BUFFER);
	glBufferData(GL_ARRAY_BUFFER,sizeof(Character)*_chars.size(),
				 _chars.data(),
				 GL_STATIC_DRAW);
}

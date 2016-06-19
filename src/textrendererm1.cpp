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

TextRendererM1::TextRendererM1():
	_chars(),_charBuf(),_program(),_texture(),_handles()
{
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
		for (int y=0; y<cs; ++y)
			for (int x=0; x<FontTexture::symCount; ++x)
			{
				GLubyte c=indata[x+y*FontTexture::symCount];
				for (int b=0; b<cs; ++b)
				{
					if (((c)&(1<<b))!=0)
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
	glTexImage2D(GL_TEXTURE_2D,0,GL_COMPRESSED_RED,tw,8,0,GL_RED,GL_UNSIGNED_BYTE,dest);
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
	Resolution(800,600);
	TEST("Size")
}

TextRendererM1::~TextRendererM1()
{

}

TextRendererM1::TextHandle TextRendererM1::Create(int p_x,int p_y,int p_w,int p_h)
{
	size_t offset=_chars.size();
	for (int i=0; i<p_w*p_h; ++i)
	{
		_chars.push_back(
		{
			0,
			0,
			GLubyte('0'+i),
			GLshort(p_x+FontTexture::charSize*(i%p_w)),
			GLshort(p_y+FontTexture::charSize*(i/p_w))
		}
		);
	};
	UploadWholeBuffer();
	TextHandle key=_handles.Add(offset);
	return key;
}

void TextRendererM1::Delete(TextRendererM1::TextHandle p_t)
{
	_handles.Print();
	size_t offset=_handles[p_t];
	size_t next=_handles.Next(p_t);
	int shift=next>offset?next-offset:0;
	//std::cout<<"off "<<offset<<" shift= "<<shift;
	_handles.Remove(p_t,shift);
	_handles.Print();

	auto charEnd=(shift==0)?_chars.end():_chars.begin()+offset+shift;
	_chars.erase(_chars.begin()+offset,charEnd);
	UploadWholeBuffer();
}

void TextRendererM1::Draw()
{
	if (_chars.Pending())
	{
		cout<<"u";
		_charBuf.Bind(GL_ARRAY_BUFFER);
		VSubData(GL_ARRAY_BUFFER,_chars,_chars.From(),_chars.To());
		_chars.Reset();
	}
	cout<<"d";
	glDrawArrays(GL_POINTS,0,_chars.size());
}

void TextRendererM1::Print(size_t p_o, const string& p_s)
{
	// Return if offset out of bounds
	if (p_o>=_chars.size())
	{
		return;
	}
	// Copy string
	auto it=_chars.begin()+p_o;
	size_t numChars=min(p_s.size(),_chars.size()-p_o);
	// Write
	for(size_t i=0;i<numChars;++i)
	{
		const Character& c=_chars[i+p_o];
		_chars[i+p_o]={111,111,static_cast<GLubyte>(p_s[i]),c.x,c.y};
	}
}

void TextRendererM1::Resolution(int p_w,int p_h)
{
	GLint resolution=glGetUniformLocation(_program,"resolution");
	glUniform2f(resolution,static_cast<float>(p_w),static_cast<float>(p_h));
}

void TextRendererM1::UploadWholeBuffer()
{
	_charBuf.Bind(GL_ARRAY_BUFFER);
	glBufferData(GL_ARRAY_BUFFER,sizeof(Character)*_chars.size(),_chars.data(),
			GL_STATIC_DRAW);
}

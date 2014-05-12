#include <iostream>
#include "textrendererm1.h"

using namespace std;
extern const unsigned char g_fontTextureRaw[];

const size_t NUMGRIDS=5;
const size_t NUMCHARS=100;

TextRendererM1::TextRendererM1(const gl::Buffer& p_resBuf):
_grids(NUMGRIDS,{0,0,30,5}),
_chars(NUMCHARS, {0,0,'a'}),
_gridBuf(),_charBuf(),_program(),_texture(),_gridAttrib()
{
	// Fill
	float i=0;
	for_each(_grids.begin(),_grids.end(),[&i](Grid& c)
	{
		c.x=i*10;
		c.y=i*20;
		c.h+=i;
		i++;
	});
	for_each(_chars.begin(),_chars.end(),[](Character& c)
	{
		cout<<c.c;
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
	const size_t TEXTURE_WIDTH=2048;
	const GLubyte* indata=static_cast<const GLubyte*>(g_fontTextureRaw);
	GLubyte dest[TEXTURE_WIDTH*8]= {0};
	{
		// Convert 1bpp to 1Bpp (GL_RED)
		for (int y=0; y<8; ++y)
			for (int x=0; x<224; ++x)
			{
				GLubyte c=indata[x+y*224];
				for (int b=0; b<8; ++b)
				{
					if (((c)&(1<<b))!=0)
					{
						dest[x*8+(7-y)*TEXTURE_WIDTH+7-b]=255;
					}
				}
			}
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,GL_COMPRESSED_RED,TEXTURE_WIDTH,8,0,GL_RED,GL_UNSIGNED_BYTE,dest);
	GLint samplerLoc=glGetUniformLocation(_program,"tex");
	glUniform1i(samplerLoc,0);
	cout<<samplerLoc;
	TEST("Texture")

	// Grid
	_gridBuf.Bind(GL_ARRAY_BUFFER);
	glBufferData(GL_ARRAY_BUFFER,sizeof(Grid)*_grids.size(),_grids.data(),GL_STATIC_DRAW);
	_gridAttrib=glGetAttribLocation(_program,"grid");
	glVertexAttribIPointer(_gridAttrib,2,GL_UNSIGNED_INT,0,0);
	//glEnableVertexAttribArray(gridAttrib);
	TEST("Grid")

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

void TextRendererM1::Draw()
{
	size_t offset=0;
	for (size_t i=0; i<NUMGRIDS; ++i)
	{
		size_t numpoints=_grids[i].w*_grids[i].h;
		glVertexAttrib4f(_gridAttrib,_grids[i].x,_grids[i].y,_grids[i].w,_grids[i].h);
		glDrawArrays(GL_POINTS,offset,numpoints);
		offset+=numpoints;
	}
}
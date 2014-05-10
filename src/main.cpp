#include <algorithm>
#include <vector>

#include <SDL2/SDL.h>

#include "utils/context.h"
#include "utils/window.h"
#include "utils/loadstring.h"
#include "utils/gl/buffer.h"
#include "utils/gl/shader.h"
#include "utils/gl/program.h"
#include "utils/gl/logerrors.h"

#include "processevents.h"
#include <iostream>
#include <stdexcept>

#define TEST(x) gl::LogErrors(x);

using namespace std;

struct Grid
{
	int16_t x,y;
	int16_t w,h;
};

struct Character
{
	uint8_t flags;
	uint8_t c;
};

int main(int,char**) try
{
	Window window("SimpleText",800,600,SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL);
	Context glcontext(window);
	glClearColor(0,0.5,0.75,1.0);

	/* Text */
	const size_t NUMGRIDS=5;
	const size_t NUMCHARS=32;
	vector<Grid> grids(NUMGRIDS,{20,200,100,220});
	short i=0;
	for_each(grids.begin(),grids.end(),[&i](Grid& c)
	{
		c.x=i*40;
		c.y=i*50;
		i++;
		cout<<c.x<<c.y;
	});
	vector<Character> chars(NUMCHARS, {0,'a'});
	chars[12]={1,'z'};
	for_each(chars.begin(),chars.end(),[](Character& c)
	{
		cout<<c.c;
	});

	// Program
	gl::Shader vs(GL_VERTEX_SHADER),gs(GL_GEOMETRY_SHADER),
	fs(GL_FRAGMENT_SHADER);
	vs.Compile(LoadString("data/vert.glsl"));
	gs.Compile(LoadString("data/geo.glsl"));
	fs.Compile(LoadString("data/frag.glsl"));
	gl::Program prog;
	prog.Attach(vs);
	prog.Attach(gs);
	prog.Attach(fs);
	prog.Link();
	prog.Bind();

	// Grid
	gl::Buffer gridBuff;
	gridBuff.Bind(GL_ARRAY_BUFFER);
	glBufferData(GL_ARRAY_BUFFER,sizeof(Grid)*grids.size(),grids.data(),GL_STATIC_DRAW);
	GLint gridAttrib = glGetAttribLocation(prog,"grid");
	glVertexAttribIPointer(gridAttrib,2,GL_INT,0,0);
	glEnableVertexAttribArray(gridAttrib);
	glVertexAttribDivisor(gridAttrib,1);
	TEST("Grid")

	// Chars
	gl::Buffer buff;
	buff.Bind(GL_ARRAY_BUFFER);
	glBufferData(GL_ARRAY_BUFFER,sizeof(Character)*chars.size(),chars.data(),GL_DYNAMIC_DRAW);
	GLint charAttrib = glGetAttribLocation(prog,"chardata");
	glVertexAttribIPointer(charAttrib,1,GL_UNSIGNED_SHORT,0,0);
	glEnableVertexAttribArray(charAttrib);
	TEST("Chars")

	/* Window size */
	pair<float,float> reso= {800,600};
	GLuint uniblock=glGetUniformBlockIndex(prog,"uniblock");
	gl::Buffer unibuf;
	unibuf.Bind(GL_UNIFORM_BUFFER);
	glUniformBlockBinding(prog,uniblock,0);
	glBufferData(GL_UNIFORM_BUFFER,sizeof(float)*2,&reso,GL_STREAM_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER,uniblock,unibuf);

	TEST("init")

	struct Meh
	{
		pair<float,float>& cap;

		Meh(pair<float,float>& r):cap(r)
		{
		}
		void operator()(SDL_KeyboardEvent&)
		{
			cout<<"keyboard event ";
		}
		void operator()(SDL_WindowEvent& we)
		{
			switch (we.event)
			{
				default:
					break;
				case SDL_WINDOWEVENT_RESIZED:
				{
					cap.first=we.data1;
					cap.second=we.data2;
					cout<<"Resized\n";
					// +bind
					glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(float)*2,&cap);
					glViewport(0,0,we.data1,we.data2);
					break;
				}
			}
			cout<<"window event ";
		}
	} meh(reso);

	while (ProcessEvents(meh))
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		//glDrawArrays(GL_POINTS,0,NUMCHARS);
		glDrawArraysInstanced(GL_POINTS,0,NUMCHARS,5);
		SDL_Delay(16);
		SDL_GL_SwapWindow(window);
	}

	TEST("end")
	return 0;
}
catch (const runtime_error& e)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,u8"Error",u8"Sum ting wong",0);
	std::cout<<"Runtime error: "<<e.what();
	return -1;
}

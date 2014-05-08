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

float vertices[] =
{
	-0.5f, -0.5f,
	-0.5f, 0.15f,
	0.5f, -0.5f,
	0.35f, 0.5f,
	0.95,-0.5,1,.5
};

int main(int,char**) try
{
	Window window("SimpleText",800,600,SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL);
	Context glcontext(window);

	glClearColor(0,0.5,0.75,1.0);

	gl::Buffer buff;
	buff.Bind(GL_ARRAY_BUFFER);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	gl::Shader vs(GL_VERTEX_SHADER),fs(GL_FRAGMENT_SHADER);
	vs.Compile(LoadString("data/vert.glsl"));
	fs.Compile(LoadString("data/frag.glsl"));

	gl::Program prog;
	prog.Attach(vs);
	prog.Attach(fs);
	prog.Link();
	prog.Bind();

	GLint posAttrib = glGetAttribLocation(prog, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);

	std::pair<float,float> reso= {800,600};

	GLuint uniblock=glGetUniformBlockIndex(prog,"uniblock");
	gl::Buffer unibuf;
	unibuf.Bind(GL_UNIFORM_BUFFER);
	glUniformBlockBinding(prog,uniblock,0);

	glBufferData(GL_UNIFORM_BUFFER,sizeof(float)*2,&reso,GL_STREAM_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER,uniblock,unibuf);

	TEST("init")

	struct Meh
	{
		std::pair<float,float>& cap;

		Meh(std::pair<float,float>& r):cap(r)
		{
		}
		void operator()(SDL_KeyboardEvent& e)
		{
			std::cout<<"keyboard event ";
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
					std::cout<<"Resized\n";
					// +bind
					glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(float)*2,&cap);
					glViewport(0,0,we.data1,we.data2);
					break;
				}
			}
			std::cout<<"window event ";
		}
	} meh(reso);

	while (ProcessEvents(meh))
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
		SDL_Delay(16);
		SDL_GL_SwapWindow(window);
	}

	TEST("end")
	return 0;
}
catch (const std::runtime_error& e)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,u8"Error",u8"Sum ting wong",0);
	std::cout<<"Runtime error: "<<e.what();
	return -1;
}

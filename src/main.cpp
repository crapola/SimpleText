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

	TEST("before loop")

	struct Meh: public EventProcessor<Meh>
	{
		void operator()(SDL_WindowEvent& we)
		{
			switch (we.event)
			{
				default:
					break;
				case SDL_WINDOWEVENT_RESIZED:
				{
					std::cout<<"Resized.\n";
					break;
				}
			}
			std::cout<<"e ";
		}
	} meh;
	std::cout<<sizeof(Meh);
	while (meh)
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
		SDL_Delay(16);
		SDL_GL_SwapWindow(window);
	}

	TEST("end of prog")
	return 0;
}
catch (const std::runtime_error& e)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,u8"Error",u8"Sum ting wong",0);
	std::cout<<"Runtime error: "<<e.what();
	return -1;
}

#include <SDL2/SDL.h>

#include "processevents.h"
#include <iostream>
#include <stdexcept>

#include "utils/context.h"
#include "utils/window.h"
#include "utils/loadstring.h"
#include "utils/gl/buffer.h"
#include "utils/gl/shader.h"
#include "utils/gl/program.h"
#include "utils/gl/logerrors.h"

#include <fstream>

#define TEST(x) gl::LogErrors(x);

float vertices[] =
{
	0.0f,  0.5f,0.5f, -0.5f,-0.5f, -0.5f
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
	std::string vcode(LoadString("data/vert.glsl"));
	std::string fcode(LoadString("data/frag.glsl"));
	vs.Compile(vcode);
	fs.Compile(fcode);

	gl::Program prog;
	prog.Attach(vs);
	prog.Attach(fs);
	prog.Link();
	prog.Bind();

	GLint posAttrib = glGetAttribLocation(prog, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);

	TEST("before loop")
	while (ProcessEvents())
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
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

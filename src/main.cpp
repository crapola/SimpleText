#include <SDL2/SDL.h>
#include "context.h"
#include "window.h"
#include "processevents.h"
#include <iostream>
#include <stdexcept>

//
#include "gl/buffer.h"
#include "gl/shader.h"
#include "gl/program.h"
#include "gl/logerrors.h"

#include <cassert>
#define TEST(x) gl::LogErrors(#x);

float vertices[] =
{
	0.0f,  0.5f,0.5f, -0.5f,-0.5f, -0.5f
};

const GLchar* vertexSource =
	"layout(location=0) in vec2 position;"
	"void main() {"
	"   gl_Position = vec4(position, 0.0, 1.0);"
	"}";
const GLchar* fragmentSource =
	"out vec4 outColor;"
	"void main() {"
	"   outColor = vec4(1,1,0, 1.0);"
	"}";


int main(int,char**) try
{
	Window window("SimpleText",800,600,SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL);
	Context glcontext(window);

	glClearColor(0,0.5,0.75,1.0);

	gl::Buffer buff;
	buff.Bind(GL_ARRAY_BUFFER);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	gl::Shader vs(GL_VERTEX_SHADER),fs(GL_FRAGMENT_SHADER);
	vs.Compile(vertexSource);
	fs.Compile(fragmentSource);

	gl::Program prog;
	prog.Attach(vs);
	prog.Attach(fs);
	prog.Link();
	prog.Bind();

	GLint posAttrib = glGetAttribLocation(prog, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);

	TEST(before loop)
	while (ProcessEvents())
	{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		SDL_Delay(16);
		SDL_GL_SwapWindow(window);
	}

	TEST(end of prog)
	return 0;
}
catch (const std::runtime_error& e)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,u8"Title",u8"sum ting wong",0);
	std::cout<<"Runtime error: "<<e.what();
	return -1;
}

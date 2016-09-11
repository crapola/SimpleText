#include <iostream>
#include <stdexcept>
#include <sstream>

#include <SDL2/SDL.h>

#include "utils/gfx/context.h"
#include "utils/gfx/window.h"
#include "processevents.h"

#include "textrenderer.h"

using namespace std;
#define TEST(x) gl::LogErrors(x);

int main(int,char**) try
{
	Window window("SimpleText",800,600,SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL);
	Context glcontext(window);
	glClearColor(0.0f,0.0f,1.0f,1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Wires
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glEnable(GL_COLOR_LOGIC_OP);
	//glLogicOp(GL_EQUIV);

	TextRenderer textrend;

	textrend.Add(100);
	textrend.Delete(50,50);

	textrend.Write(0,">! HELLO WORLD !<");
	textrend.Format(0,30,200,200,0);
	TEST("init")

	struct Meh
	{
		TextRenderer& t;
		Meh(TextRenderer& t):t(t)
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
				cout<<"Resized\n";
				glViewport(0,0,we.data1,we.data2);
				t.Resolution(we.data1/3,we.data2/2);
				break;
			}
			}
		}
	} meh(textrend);

	long time=0;
	while (ProcessEvents(meh))
	{
		// Counter
		stringstream foo;
		foo<<">> "<<time++<<" <<";
		string s(foo.str());

		if (time%5==0) textrend.Write(50,s);

		// Draw
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		textrend.Draw();
		SDL_GL_SwapWindow(window);
		SDL_Delay(16);
	}

	TEST("end")
	return 0;
}
catch (const runtime_error& e)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,u8"Error",u8"Sum ting wong",0);
	std::cerr<<"Runtime error: "<<e.what();
	return -1;
}


#include <iostream>
#include <stdexcept>
#include <sstream>

#include <SDL2/SDL.h>

#include "utils/gfx/context.h"
#include "utils/gfx/window.h"
#include "processevents.h"

#include "textrendererm1.h"

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

	TextRendererM1 textrend;

	auto test1=textrend.Create(0,0,20,20);
	auto test2=textrend.Create(150,0,16,16);
	auto test3=textrend.Create(0,150,16,16);
	auto test4=textrend.Create(150,150,16,16);

	textrend.Delete(test2);

	textrend.Print(656," ! HELLO WORLD !!! ");
	TEST("init")

	struct Meh
	{
		TextRendererM1& t;
		Meh(TextRendererM1& t):t(t)
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
		auto o=textrend.Offset(test4);
		cout<<o<<" ";
		if (time%11==0) textrend.Print(o,s);

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


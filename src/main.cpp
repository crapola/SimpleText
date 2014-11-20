#include <iostream>
#include <stdexcept>
#include <sstream>

#include <SDL2/SDL.h>

#include "utils/context.h"
#include "utils/window.h"
#include "processevents.h"

#include "textrendererm1.h"

using namespace std;

int main(int,char**) try
{
	Window window("SimpleText",800,600,SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL);
	Context glcontext(window);
	glClearColor(0.0,0.0,1.0,1.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Wires
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	// Resolution UBO
	pair<float,float> reso= {400,300};
	gl::Buffer resbuf;
	resbuf.Bind(GL_UNIFORM_BUFFER);
	glBufferData(GL_UNIFORM_BUFFER,sizeof(float)*2,&reso,GL_STATIC_DRAW);

	TextRendererM1 textrend(resbuf);

	auto g=textrend.Create(5,100,50,10);


	textrend.Print(0,0,0,"Hello world ... Hello world ... Hello world ... Hello world ... Hello world ... ");

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
			//cout<<"window event ";
		}
	} meh(reso);

	long time=0;
	while (ProcessEvents(meh))
	{
		stringstream foo;
		foo<<">> "<<time++<<" <<";
		string s(foo.str());
		textrend.Print(1,1,1,s);
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
	std::cout<<"Runtime error: "<<e.what();
	return -1;
}


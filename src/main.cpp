#include <iostream>
#include <stdexcept>
#include <sstream>

#include <SDL2/SDL.h>

#include "gfx/context.h"
#include "gfx/window.h"
#include "processevents.h"
#include "gl/debug.h"

#include "textrenderer.h"

using namespace std;

int main(int,char**) try
{
	Window window("SimpleText",800,600,SDL_WINDOW_RESIZABLE|SDL_WINDOW_OPENGL);
	Context glcontext(window);

	glClearColor(0.0f,0.0f,1.0f,1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	gl::InitDebugProc();
	// Wires
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glEnable(GL_COLOR_LOGIC_OP);
	//glLogicOp(GL_EQUIV);

	//glUseProgram(123);

	TextRenderer textrend;

	textrend.Add(100);
	textrend.Delete(50,50);

	textrend.Write(0,">! HELLO WORLD !<");
	textrend.Paragraph(0,30,200,200,10);

	textrend.SetColor(0,100,Color2B(3,3,0),0);

	textrend.ForEach(0,16,[](auto c)->TextRenderer::Character
	{
		static int i=0;
		TextRenderer::Character a=c;
		a.y+=30;
		a.colors=(Color2B(3,3,0)+i)<<8;
		i--;
		return a;
	});

	auto lol=textrend.Add(16);
	textrend.Write(lol,"Some more text!!");
	textrend.SetColor(lol,16,Color2B(1,3,2),255);

	struct Meh
	{
		TextRenderer& t;
		Meh(TextRenderer& t):t(t)
		{
		}
		void operator()(SDL_KeyboardEvent& ke)
		{
			cout<<"keyboard event ";
			if(ke.type==SDL_KEYUP)
			{
				switch(ke.keysym.sym)
				{
					default:
						break;
					case SDLK_a:
					{
						cout<<"Adding more ";
						t.Add(100);
						break;
					}
					case SDLK_d:
					{
						cout<<"Deleting ";
						t.Delete(0,23);
						break;
					}
				}
			}
		}
		void operator()(SDL_WindowEvent& we)
		{
			switch(we.event)
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
	while(ProcessEvents(meh))
	{
		// Counter
		stringstream foo;
		foo<<">> "<<time++;
		string s(foo.str());

		if(time%3==0) textrend.Write(0,s);

		// Draw
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		textrend.Draw();
		SDL_GL_SwapWindow(window);
		SDL_Delay(16);
	}

	return 0;
}
catch(const runtime_error& e)
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,u8"Error",u8"Sum ting wong",0);
	std::cerr<<"Runtime error: "<<e.what();
	return -1;
}

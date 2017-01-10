#include <iostream>
#include <stdexcept>
#include <sstream>

#include <SDL2/SDL.h>

#include "gfx/context.h"
#include "gfx/window.h"
#include "processevents.h"
#include "gl/debug.h"

#include "text/smalltext.h"

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

	SmallText textrend;

	textrend.Add(100);
	textrend.Delete(50,50);

	textrend.Write(0,">! HELLO WORLD !<");
	textrend.Paragraph(0,100,31,55,30);

	textrend.SetColor(0,50,Color2B(3,3,0,1),Color2B(0,0,0,1));

	textrend.ForEach(0,16,[](auto c)->SmallText::Character
	{
		static int i=0;
		SmallText::Character a=c;
		a.y+=100+i;
		a.SetColors(Color2B(3,i,3,3),Color2B(1,1,1,1));
		i--;
		return a;
	});

	auto lol=textrend.Add("Some more text!!");
	textrend.Paragraph(lol,16,300,300,8);
	textrend.SetColor(lol,16,Color2B(0,3,0,3),Color2B(3,0,0,3));
	textrend.Delete(lol+15,lol+15);

	struct Meh
	{
		SmallText& t;
		Meh(SmallText& t):t(t)
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

		if(time%3==0) textrend.Write(50,s);
		textrend.SetColor(0,50,time|0x03,Color2B(0,0,0,time%4));

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

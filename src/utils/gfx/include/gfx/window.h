#pragma once
#include <SDL2/SDL.h>

/** \brief SDL window and initialization.
 *  SDL_Init is called in the constructor.
 */
class Window
{
public:
	Window(const char* title,int width,int height,Uint32 flags=0);
	~Window();
	Window(const Window&)=delete;
	Window& operator=(const Window&)=delete;

	operator SDL_Window*() const;

	SDL_Point Size() const;
	SDL_Surface* Surface() const;
private:
	SDL_Window* _window;
};

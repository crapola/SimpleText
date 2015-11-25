#pragma once
#include <SDL2/SDL.h>

// GLEW context for an SDL window.

class Context
{
public:
	Context(SDL_Window*);
	~Context();
	Context(const Context&)=delete;
	Context& operator=(const Context&)=delete;
private:
	SDL_GLContext _context; // void*
};
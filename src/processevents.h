#ifndef PROCESSEVENTS_H_INCLUDED
#define PROCESSEVENTS_H_INCLUDED

template<class Td>
struct EventProcessor
{
	operator bool()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			// Check for messages
			switch (event.type)
			{
				default:
					break;
				// Exit if the window is closed
				case SDL_QUIT:
					return false;
					break;
				// Check for keypresses
				case SDL_KEYDOWN:
				{
					// Exit if ESCAPE is pressed
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						return false;
					}
					break;
				}
				case SDL_WINDOWEVENT:
					(*static_cast<Td*>(this))(event.window);
					break;
			}
		}
		return true;
	};
};

#endif // PROCESSEVENTS_H_INCLUDED


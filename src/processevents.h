#ifndef PROCESSEVENTS_H_INCLUDED
#define PROCESSEVENTS_H_INCLUDED

// Return false to quit
bool ProcessEvents()
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
					return false;
				break;
			}
			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
					default:
						break;
					case SDL_WINDOWEVENT_RESIZED:
					{
						break;
					}
				}
				break;
		}
	}
	return true;
}

#endif // PROCESSEVENTS_H_INCLUDED

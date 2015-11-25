#pragma once

template<typename T>
bool ProcessEvents(T& t)
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
				t(event.key);		// SDL_KeyboardEvent
				break;
			}
			case SDL_WINDOWEVENT:
				t(event.window);	// SDL_WindowEvent
				break;
		}
	}
	return true;
}
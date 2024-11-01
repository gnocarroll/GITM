#include "src/Events.hpp"

#include <SDL2/SDL_events.h>

#include "src/MyIntDef.hpp"
#include "src/Util.hpp"

bool quit;

bool shouldQuit() {
	return quit;
}

template <ui32 eventType>
static inline void handleEvent(SDL_Event* event) {
	perrorCustom("handleEvent", "event type invalid or not implemented");
}

template <>
static inline void handleEvent<SDL_WINDOWEVENT>(SDL_Event* event) {
	SDL_WindowEvent windowEvent = *reinterpret_cast<SDL_WindowEvent*>(event);

	switch (windowEvent.event) {
		case SDL_WINDOWEVENT_CLOSE:
			quit = true;
			break;
	}
}

#define EVENT_CASE(x, event) case x: handleEvent<x>(&event); break

void pollEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			EVENT_CASE(SDL_WINDOWEVENT, event);
		}
	}
}
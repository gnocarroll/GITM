#include "src/Events.hpp"

#include <bitset>
#include <cstring>

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_scancode.h>

#include "src/CircleBuff.hpp"
#include "src/MyIntDef.hpp"
#include "src/Util.hpp"

// Maximum number of characters that will be stored at once from text input
// events (will discard new ones if bounded buffer wraps all the way around)

#define MAX_TEXT_INPUT (1024)

CircleBuff<char, MAX_TEXT_INPUT> textInput;

char popTextInput() {
	return textInput.Pop();
}

typedef ui8 ScancodeArr[SDL_NUM_SCANCODES];

// In order: current state
// # times pressed this frame
// # times released this frame

static ScancodeArr pressedNow;
static ScancodeArr pressedFrame;
static ScancodeArr releasedFrame;

bool keyPressed(int key) {
	return scanPressed(SDL_GetScancodeFromKey(key));
}
bool scanPressed(int scancode) {
	return pressedNow[scancode];
}

ui8 keyThisFrame(int key) {
	return scanThisFrame(SDL_GetScancodeFromKey(key));
}
ui8 scanThisFrame(int scancode) {
	return pressedFrame[scancode];
}

ui8 keyReleasedFrame(int key) {
	return scanReleasedFrame(SDL_GetScancodeFromKey(key));
}

ui8 scanReleasedFrame(int scancode) {
	return releasedFrame[scancode];
}

void startTextInput() {
	SDL_StartTextInput();
}

void stopTextInput() {
	SDL_StopTextInput();
}

bool quit;

bool shouldQuit() {
	return quit;
}

void setShouldQuit(bool _quit) {
	quit = _quit;
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

static inline void handleKeyEvent(SDL_Event* event) {
	SDL_KeyboardEvent keyboardEvent =
		*reinterpret_cast<SDL_KeyboardEvent*>(event);

	// Do not want to consider repeats from holding key down, can handle that
	// myself

	if (keyboardEvent.repeat)
		return;

	SDL_Scancode scancode = keyboardEvent.keysym.scancode;
	ui8 state = keyboardEvent.state;

	pressedFrame[scancode]  += (state == SDL_PRESSED);
	releasedFrame[scancode] += (state == SDL_RELEASED);
	pressedNow[scancode]     = (state == SDL_PRESSED);
}

template <>
static inline void handleEvent<SDL_KEYDOWN>(SDL_Event* event) {
	handleKeyEvent(event);
}

template <>
static inline void handleEvent<SDL_KEYUP>(SDL_Event* event) {
	handleKeyEvent(event);
}

template <>
static inline void handleEvent<SDL_TEXTINPUT>(SDL_Event* event) {
	SDL_TextInputEvent inputEvent = *reinterpret_cast<SDL_TextInputEvent*>(event);

	for (size_t i = 0;
		(i < SDL_TEXTINPUTEVENT_TEXT_SIZE) && (inputEvent.text[i]);
		i++) {

		textInput.Push(inputEvent.text[i]);
	}
}

#define EVENT_CASE(x, event) case x: handleEvent<x>(&event); break

void pollEvents() {
	for (auto& arr : { pressedFrame, releasedFrame })
		std::memset((void*)&arr, 0, sizeof(arr));

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		EVENT_CASE(SDL_WINDOWEVENT, event);
		EVENT_CASE(SDL_KEYDOWN, event);
		EVENT_CASE(SDL_KEYUP, event);
		EVENT_CASE(SDL_TEXTINPUT, event);
		default:
			break;
		}
	}
}
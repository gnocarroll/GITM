#pragma once

#include "src/MyIntDef.hpp"

bool shouldQuit();
void setShouldQuit(bool _quit);

void pollEvents();

// Query keyboard input

bool keyPressed(int key);
bool scanPressed(int scancode);

ui8 keyThisFrame(int key);
ui8 scanThisFrame(int scancode);

ui8 keyReleasedFrame(int key);
ui8 scanReleasedFrame(int scancode);

// Wrappers around SDL text input functionality, which provides you with the
// actual text the user is inputting (e.g. if they are pressing shift it gives
// capital letters. When enabled event polling code will receive instances of
// SDL_TextInputEvent.

void startTextInput();
void stopTextInput();

char popTextInput();
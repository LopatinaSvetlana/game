#pragma once
#include<iostream>
#include<SDL.h>
#include <SDL_ttf.h>
#include<SDL_image.h>
#include "SDL_mixer.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 720
#define ELEM_SIZE 80

extern int condition;

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Event event;

extern Mix_Chunk** sounds;
extern Mix_Music* music;

extern int currentLevel;
extern int musicOn;
extern int gridScale;
extern int selectedIndex;
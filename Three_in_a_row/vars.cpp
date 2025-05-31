#pragma once
#include"vars.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Event event;
int condition = 0;
Mix_Chunk** sounds;
Mix_Music* music;
int currentLevel = 0;
int musicOn = 1;
int gridScale = 70;
int selectedIndex = -1;
#pragma once
#include<iostream>
#include<SDL.h>
#include <SDL_ttf.h>
#include<SDL_image.h>
#include "SDL_mixer.h"

bool init(SDL_Window*& window, SDL_Renderer*& renderer);
SDL_Texture* loadTexture(const char* name, SDL_Renderer* renderer);
bool isButtonClicked(const SDL_Rect& rect, const SDL_Event& event, Mix_Chunk* touchSound = nullptr);
void FillBackground(SDL_Renderer* rend, int colorR, int colorG, int colorB, int colorA);
void DrawText(SDL_Renderer* render, TTF_Font* font, const char* text, SDL_Color color, int xPos, int yPos, int Scale);


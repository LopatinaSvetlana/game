#pragma once
#include<iostream>
#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<malloc.h>
#include <string>

struct elem {
	int i, j;
	SDL_Rect transPos;
	bool isSelected = false;
	int element = 0;
};

void GenerateInitialField(elem* field, int scale, int amount_figure, int gridScale);
int RemoveMatches(elem* field, int scale);
void GenerateNewElements(elem* field, int scale, int amount_figure);
bool HasMatch(elem* field, int scale);
void DrawDynamicUI(SDL_Renderer* renderer, TTF_Font* font, int remainingMoves, int Score);

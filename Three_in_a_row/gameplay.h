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
/*void ButtonBackDraw(SDL_Texture* button_back, SDL_Rect button_back_card, int targetCondition = 0, Mix_Chunk* click = nullptr);
void RunLevel(SDL_Renderer* renderer, int amount_figure, int scale, SDL_Texture* gridImg, SDL_Texture** elems,
	SDL_Texture* selectedImg, SDL_Event event, TTF_Font* font, SDL_Texture* button_back, int maxMoves, int maxScore);*/

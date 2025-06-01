#pragma once
#include"base.h"
#include<iostream>
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include"SDL_mixer.h"
#include"vars.h"
using namespace std;

bool init(SDL_Window*& window, SDL_Renderer*& renderer) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "ошибка: " << SDL_GetError() << endl;
		return false;
	}
	window = SDL_CreateWindow(u8"три в ряд", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window) {
		cout << "ошибка: " << SDL_GetError() << endl;
		SDL_Quit();
		return false;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) {
		cout << "ошибка: " << SDL_GetError() << endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return false;
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	return true;
}

SDL_Texture* loadTexture(const char* name, SDL_Renderer* renderer) {
	SDL_Surface* surface = IMG_Load(name);
	SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);

	if (!surface) {
		cout << "Ошибка загрузки изображения: " << IMG_GetError() << endl;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	if (!texture) {
		cout << "Ошибка создания текстуры: " << SDL_GetError() << endl;
	}
	return texture;
}

bool isButtonClicked(const SDL_Rect& rect, const SDL_Event& event, Mix_Chunk* touchSound) {
	int x = event.button.x;
	int y = event.button.y;
	bool check = x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h;
	if (check)
		Mix_PlayChannel(0, touchSound, 0);
	return check;
}

void FillBackground(SDL_Renderer* rend, int colorR, int colorG, int colorB, int colorA) {
	SDL_SetRenderDrawColor(rend, colorR, colorG, colorB, colorA);
	SDL_RenderClear(rend);
}

void DrawText(SDL_Renderer* render, TTF_Font* font, const char* text, SDL_Color color, int xPos, int yPos, int Scale) {
	SDL_Surface* surface = TTF_RenderText_Solid(font, text, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(render, surface);
	SDL_Rect textTransform = { xPos,yPos,(int)(Scale * strlen(text) * 0.3),Scale };
	SDL_RenderCopy(render, texture, NULL, &textTransform);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}
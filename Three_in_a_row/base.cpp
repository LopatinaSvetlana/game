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

/*Mix_Chunk* loadSound(const char* name) {
	Mix_Chunk* sound = Mix_LoadWAV(name);
	if (!sound) {
		cout << "Ошибка загрузки звука: " << Mix_GetError() << endl;
	}
	return sound;
}

Mix_Music* loadMusic(const char* name) {
	Mix_Music* music = Mix_LoadMUS(name);
	if (!music) {
		cout << "Ошибка загрузки музыки: " << Mix_GetError() << endl;
	}
	return music;
}*/

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

/*void ButtonBackDraw(SDL_Texture* button_back, SDL_Rect button_back_card, int targetCondition = 0, Mix_Chunk* click = nullptr) {
	SDL_RenderCopy(renderer, button_back, NULL, &button_back_card);
	if (event.type == SDL_MOUSEBUTTONDOWN && isButtonClicked(button_back_card, event, click))
		condition = targetCondition;
}*/
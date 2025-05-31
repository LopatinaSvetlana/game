#pragma once
#include<iostream>
#include<SDL.h>
#include<SDL_ttf.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include"base.h"
#include"vars.h"
#include"gameplay.h"
#include<malloc.h>
#include <string>
using namespace std;

void GenerateInitialField(elem* field, int scale, int amount_figure, int gridScale) {
    for (int i = 0; i < scale; i++) {
        for (int j = 0; j < scale; j++) {
            int value;
            do {
                value = rand() % amount_figure;
                if (j >= 2 && field[i * scale + (j - 1)].element == value && field[i * scale + (j - 2)].element == value)
                    continue;
                if (i >= 2 && field[(i - 1) * scale + j].element == value && field[(i - 2) * scale + j].element == value)
                    continue;
                break;
            } while (true);

            field[i * scale + j] = {
                i,j,
                {
                    i * gridScale + SCREEN_WIDTH / 2 - scale * gridScale / 2,
                    j * gridScale + SCREEN_HEIGHT / 2 - scale * gridScale / 2,
                    ELEM_SIZE,ELEM_SIZE
                },
                false,
                value
            };
        }
    }
}

int RemoveMatches(elem* field, int scale) {
    int removedCount = 0;

    //Горизонтальные совпадения
    for (int i = 0; i < scale; i++) {
        int j = 0;
        while (j < scale) {
            int start = j;
            int currentElem = field[i * scale + j].element;
            while (j < scale && field[i * scale + j].element == currentElem && currentElem != -1)
                j++;
            if (j - start >= 3) {
                for (int k = start; k < j; k++) {
                    if (field[i * scale + k].element != -1) {
                        field[i * scale + k].element = -1;
                        removedCount++;
                    }
                }
            }
            if (j == start)j++;
        }
    }
    //Вертикальные совпадения
    for (int j = 0; j < scale; j++) {
        int i = 0;
        while (i < scale) {
            int start = i;
            int currentElem = field[i * scale + j].element;
            while (i < scale && field[i * scale + j].element == currentElem && currentElem != -1)
                i++;
            if (i - start >= 3) {
                for (int k = start; k < i; k++) {
                    if (field[k * scale + j].element != -1) {
                        field[k * scale + j].element = -1;
                        removedCount++;
                    }
                }
            }
            if (i == start)i++;
        }
    }
    return removedCount;
}

void GenerateNewElements(elem* field, int scale, int amount_figure) {
    for (int i = 0; i < scale; i++) {
        for (int j = 0; j < scale; j++) {
            elem* cell = &field[i * scale + j];
            if (cell->element != -1) continue;

            int value;
            do {
                value = rand() % amount_figure;

                // Проверка по горизонтали
                if (j >= 2 &&
                    field[i * scale + (j - 1)].element == value &&
                    field[i * scale + (j - 2)].element == value)
                    continue;

                // Проверка по вертикали
                if (i >= 2 &&
                    field[(i - 1) * scale + j].element == value &&
                    field[(i - 2) * scale + j].element == value)
                    continue;

                // Дополнительная проверка на возможность составить три вместе с соседними (вниз и вправо)
                if (j >= 1 && j + 1 < scale &&
                    field[i * scale + (j - 1)].element == value &&
                    field[i * scale + (j + 1)].element == value)
                    continue;

                if (i >= 1 && i + 1 < scale &&
                    field[(i - 1) * scale + j].element == value &&
                    field[(i + 1) * scale + j].element == value)
                    continue;

                break; // значение подходит
            } while (true);

            cell->element = value;
        }
    }
}

bool HasMatch(elem* field, int scale) {
    for (int i = 0; i < scale; i++) {
        for (int j = 0; j < scale; j++) {
            int current = field[i * scale + j].element;
            if (current == -1) continue;

            if (j <= scale - 3 &&
                current == field[i * scale + (j + 1)].element &&
                current == field[i * scale + (j + 2)].element)
                return true;

            if (i <= scale - 3 &&
                current == field[(i + 1) * scale + j].element &&
                current == field[(i + 2) * scale + j].element)
                return true;
        }
    }
    return false;
}

void DrawDynamicUI(SDL_Renderer* renderer, TTF_Font* font, int remainingMoves, int Score) {
    DrawText(renderer, font, "REMAINING MOVES: ", { 255,255,255,0 }, 20, SCREEN_HEIGHT / 2 - 150, 50);
    DrawText(renderer, font, to_string(remainingMoves).c_str(), { 255,255,255,0 },
        20, SCREEN_HEIGHT / 2 - 100, 50);

    DrawText(renderer, font, "SCORE: ", { 255, 238, 205,0 }, 20, SCREEN_HEIGHT / 2 - 20, 50);
    DrawText(renderer, font, to_string(Score).c_str(), { 255, 238, 205,0 },
        20, SCREEN_HEIGHT / 2, 100);

    if (condition == 3)
        DrawText(renderer, font, "GOAL: 100 ", { 255, 238, 205,0 }, 20, SCREEN_HEIGHT / 2 - 60, 50);
    if (condition == 4)
        DrawText(renderer, font, "GOAL: 200 ", { 255, 238, 205,0 }, 20, SCREEN_HEIGHT / 2 - 60, 50);
    if (condition == 5)
        DrawText(renderer, font, "GOAL: 500 ", { 255, 238, 205,0 }, 20, SCREEN_HEIGHT / 2 - 60, 50);
}

/*void ButtonBackDraw(SDL_Texture* button_back, SDL_Rect button_back_card, int targetCondition = 0, Mix_Chunk* click = nullptr) {
    SDL_RenderCopy(renderer, button_back, NULL, &button_back_card);
    if (event.type == SDL_MOUSEBUTTONDOWN && isButtonClicked(button_back_card, event, click))
        condition = targetCondition;
}

void RunLevel(SDL_Renderer* renderer, int amount_figure, int scale, SDL_Texture* gridImg, SDL_Texture** elems,
    SDL_Texture* selectedImg, SDL_Event event, TTF_Font* font, SDL_Texture* button_back, int maxMoves, int maxScore) {
    currentLevel = condition;
    elem* field = (elem*)malloc(scale * scale * sizeof(elem));
    SDL_Rect selectedElem = { 0,0,0,0 };
    bool isSelectedExist = false;
    SDL_Rect button_back_cord = { 20, 20, 100, 100 };
    GenerateInitialField(field, scale, amount_figure, gridScale);
    int remainingMoves = maxMoves;
    int localScore = 0;

    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                condition = -1;
                running = false;
                break;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN && isButtonClicked(button_back_cord, event, sounds[1])) {
                condition = 1;
                running = false;
                break;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                for (int i = 0; i < scale; i++) {
                    for (int j = 0; j < scale; j++) {
                        elem* curElem = &field[i * scale + j];
                        if (isButtonClicked(curElem->transPos, event)) {
                            if (!isSelectedExist) {
                                Mix_PlayChannel(1, sounds[2], 0);
                                selectedElem = curElem->transPos;
                                selectedIndex = i * scale + j;
                                isSelectedExist = true;
                            }
                            else {

                                int selI = selectedIndex / scale;
                                int selJ = selectedIndex % scale;

                                if ((abs(selI - i) == 1 && selJ == j) || (abs(selJ - j) == 1 && selI == i)) {
                                    int secondIndex = i * scale + j;
                                    swap(field[selectedIndex].element, field[secondIndex].element);




                                    SDL_RenderPresent(renderer);
                                    if (HasMatch(field, scale)) { //если ход выдал три в ряд
                                        Mix_PlayChannel(1, sounds[4], 0);
                                        SDL_RenderClear(renderer);
                                        FillBackground(renderer, 191, 149, 190, 0);
                                        ButtonBackDraw(button_back, button_back_cord, 1);
                                        DrawDynamicUI(renderer, font, remainingMoves, localScore);

                                        for (int ii = 0; ii < scale; ii++) {
                                            for (int jj = 0; jj < scale; jj++) {
                                                elem* e = &field[ii * scale + jj];
                                                SDL_RenderCopy(renderer, gridImg, NULL, &e->transPos);
                                                if (e->element != -1)
                                                    SDL_RenderCopy(renderer, elems[e->element], NULL, &e->transPos);
                                            }
                                        }
                                        SDL_RenderPresent(renderer);
                                        SDL_Delay(500);

                                        int cntRemoved = RemoveMatches(field, scale);
                                        if (cntRemoved <= 3) {
                                            localScore += 10;
                                            Mix_PlayChannel(0, sounds[3], 0);
                                        }
                                        if (cntRemoved == 4) {
                                            localScore += 20;
                                            Mix_PlayChannel(0, sounds[3], 0);
                                        }
                                        if (cntRemoved > 4) {
                                            localScore += 30;
                                            Mix_PlayChannel(0, sounds[0], 0);
                                        }

                                        remainingMoves--;
                                        // Рендерим поле с пустыми клетками
                                        SDL_RenderClear(renderer);
                                        FillBackground(renderer, 191, 149, 190, 0);
                                        ButtonBackDraw(button_back, button_back_cord, 1);
                                        //обнови счёт!
                                        DrawDynamicUI(renderer, font, remainingMoves, localScore);

                                        if (isSelectedExist)
                                            SDL_RenderCopy(renderer, selectedImg, NULL, &selectedElem);

                                        for (int ii = 0; ii < scale; ii++) {
                                            for (int jj = 0; jj < scale; jj++) {
                                                elem* e = &field[ii * scale + jj];
                                                SDL_RenderCopy(renderer, gridImg, NULL, &e->transPos);
                                                if (e->element != -1)
                                                    SDL_RenderCopy(renderer, elems[e->element], NULL, &e->transPos);
                                            }
                                        }
                                        SDL_RenderPresent(renderer);

                                        SDL_Delay(500);  // Пауза перед появлением новых

                                        GenerateNewElements(field, scale, amount_figure);
                                        // Цепная реакция — убираем новые совпадения после генерации
                                        while (HasMatch(field, scale)) {
                                            for (int ii = 0; ii < scale; ii++) {
                                                for (int jj = 0; jj < scale; jj++) {
                                                    elem* e = &field[ii * scale + jj];
                                                    SDL_RenderCopy(renderer, gridImg, NULL, &e->transPos);
                                                    if (e->element != -1)
                                                        SDL_RenderCopy(renderer, elems[e->element], NULL, &e->transPos);
                                                }
                                            }

                                            SDL_Delay(500);
                                            cntRemoved = RemoveMatches(field, scale);

                                            if (cntRemoved <= 3) {
                                                localScore += 10;
                                                Mix_PlayChannel(0, sounds[3], 0);
                                            }
                                            if (cntRemoved == 4) {
                                                localScore += 20;
                                                Mix_PlayChannel(0, sounds[3], 0);
                                            }
                                            if (cntRemoved > 4) {
                                                localScore += 30;
                                                Mix_PlayChannel(0, sounds[0], 0);
                                            }

                                            // Рендерим после удаления
                                            SDL_RenderClear(renderer);
                                            FillBackground(renderer, 191, 149, 190, 0);
                                            ButtonBackDraw(button_back, button_back_cord, 1);
                                            DrawDynamicUI(renderer, font, remainingMoves, localScore);

                                            for (int ii = 0; ii < scale; ii++) {
                                                for (int jj = 0; jj < scale; jj++) {
                                                    elem* e = &field[ii * scale + jj];
                                                    SDL_RenderCopy(renderer, gridImg, NULL, &e->transPos);
                                                    if (e->element != -1)
                                                        SDL_RenderCopy(renderer, elems[e->element], NULL, &e->transPos);
                                                }
                                            }

                                            SDL_RenderPresent(renderer);
                                            SDL_Delay(500); // Пауза перед генерацией

                                            GenerateNewElements(field, scale, amount_figure);
                                        }




                                    }
                                    else {
                                        remainingMoves--;
                                        SDL_RenderClear(renderer);
                                        FillBackground(renderer, 191, 149, 190, 0);
                                        ButtonBackDraw(button_back, button_back_cord, 1);
                                        DrawDynamicUI(renderer, font, remainingMoves, localScore);

                                        for (int ii = 0; ii < scale; ii++) {
                                            for (int jj = 0; jj < scale; jj++) {
                                                elem* e = &field[ii * scale + jj];
                                                SDL_RenderCopy(renderer, gridImg, NULL, &e->transPos);
                                                if (e->element != -1)
                                                    SDL_RenderCopy(renderer, elems[e->element], NULL, &e->transPos);
                                            }
                                        }
                                        SDL_RenderPresent(renderer);
                                        Mix_PlayChannel(1, sounds[4], 0);
                                        SDL_Delay(500);
                                        Mix_PlayChannel(1, sounds[4], 0);
                                        swap(field[selectedIndex].element, field[secondIndex].element);
                                    }
                                }

                                isSelectedExist = false;
                                selectedIndex = -1;
                            }
                        }
                    }
                }
            }

            if (localScore >= maxScore) {
                condition = 6;
                running = false;
                break;
            }
            if (remainingMoves <= 0) {
                condition = 7;
                running = false;
                break;
            }
        }

        FillBackground(renderer, 191, 149, 190, 0);
        ButtonBackDraw(button_back, button_back_cord, 1);
        DrawDynamicUI(renderer, font, remainingMoves, localScore);
        if (isSelectedExist)
            SDL_RenderCopy(renderer, selectedImg, NULL, &selectedElem);

        for (int i = 0; i < scale; i++) {
            for (int j = 0; j < scale; j++) {
                elem* curElem = &field[i * scale + j];
                SDL_RenderCopy(renderer, gridImg, NULL, &curElem->transPos);
                if (curElem->element >= 0)
                    SDL_RenderCopy(renderer, elems[curElem->element], NULL, &curElem->transPos);
            }
        }

        SDL_RenderPresent(renderer);
    }

    free(field);
}*/
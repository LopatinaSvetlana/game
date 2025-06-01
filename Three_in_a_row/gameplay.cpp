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

                break;
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

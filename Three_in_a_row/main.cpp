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

void ButtonBackDraw(SDL_Texture* button_back, SDL_Rect button_back_card, int targetCondition = 0, Mix_Chunk* click = nullptr) {
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
                                    if (HasMatch(field, scale)) { //если ход выдал три в р€д
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
                                        // –ендерим поле с пустыми клетками
                                        SDL_RenderClear(renderer);
                                        FillBackground(renderer, 191, 149, 190, 0);
                                        ButtonBackDraw(button_back, button_back_cord, 1);
                                        //обнови счЄт!
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

                                        SDL_Delay(500);

                                        GenerateNewElements(field, scale, amount_figure);
                                        // ÷епна€ реакци€ Ч убираем новые совпадени€ после генерации
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

                                            // –ендерим после удалени€
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
                                            SDL_Delay(500); // ѕауза перед генерацией

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
}



int main(int argc, char* argv[]) {

    if (!init(window, renderer))
        return 1;

    Mix_Init(0);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("font.ttf", 100);

    SDL_Texture* logoType = loadTexture("logotype.png", renderer);
    SDL_Texture* button1 = loadTexture("button1.png", renderer);
    SDL_Texture* button_back = loadTexture("returnn.png", renderer);
    SDL_Texture* settings_logo = loadTexture("settingslogo.png", renderer);
    SDL_Texture* level1 = loadTexture("level1.bmp", renderer);
    SDL_Texture* level2 = loadTexture("level2.bmp", renderer);
    SDL_Texture* level3 = loadTexture("level3.bmp", renderer);
    SDL_Texture* selectedImg = loadTexture("selected.png", renderer);
    SDL_Texture* gridImg = loadTexture("back.png", renderer);

    const char* sounds_to_load[] = {
        /*0*/ "big_streak.wav",
        /*1*/ "button_touch.wav",
        /*2*/ "cell_select.wav",
        /*3*/ "defalut_streak.wav",
        /*4*/ "game_failed_turn.wav",
        /*5*/ "game_lose.wav",
        /*6*/ "game_win.wav",
        /*7*/ "level_start.wav"
    };
    int soundsCount = sizeof(sounds_to_load) / sizeof(sounds_to_load[0]);
    sounds = (Mix_Chunk**)malloc(soundsCount * sizeof(Mix_Chunk*));

    for (int i = 0; i < soundsCount; i++) {
        Mix_Chunk* sound = Mix_LoadWAV(sounds_to_load[i]);
        if (!sound) {
            cout << "ќшибка загрузки звука: " << Mix_GetError() << endl;
        }
        sounds[i] = sound;
    }

    SDL_Texture* elems[5] = {
        loadTexture("krug.png", renderer),
        loadTexture("kvadrat.png", renderer),
        loadTexture("romb.png", renderer),
        loadTexture("treug.png", renderer),
        loadTexture("star.png", renderer)
    };

    SDL_Rect button1_cord = { SCREEN_WIDTH / 2 - 125, SCREEN_HEIGHT - 350, 250, 90 };
    SDL_Rect button2_cord = { SCREEN_WIDTH / 2 - 125, SCREEN_HEIGHT - 350 + 100, 250, 90 };
    SDL_Rect button3_cord = { SCREEN_WIDTH / 2 - 125, SCREEN_HEIGHT - 400, 250, 90 };

    SDL_Rect logoTransform = { SCREEN_WIDTH / 2 - 550, SCREEN_HEIGHT - 700, 1200, 350 };
    SDL_Rect settings_card = { SCREEN_WIDTH - 120, SCREEN_HEIGHT - 120, 90, 90 };
    SDL_Rect level1_card = { SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 550, 400, 150 };
    SDL_Rect level2_card = { SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 380, 400, 150 };
    SDL_Rect level3_card = { SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 210, 400, 150 };
    SDL_Rect button_back_cord = { 20, 20, 100, 100 };

    bool singleSound = false;
    bool quit = false;
    while (quit == false && condition != -1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                quit = true;

            Mix_Music* music = Mix_LoadMUS("musiccc.wav");
            if (!music) {
                cout << "ќшибка загрузки музыки: " << Mix_GetError() << endl;
            }
            if (musicOn == 1) {
                if (!Mix_PlayingMusic()) {
                    Mix_PlayMusic(music, -1);
                }
            }
            switch (condition) {
            case 0:
                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {

                    if (isButtonClicked(button1_cord, event, sounds[1]))
                        condition = 1;
                    if (isButtonClicked(settings_card, event, sounds[1]))
                        condition = 2;
                }

                FillBackground(renderer, 250, 165, 206, 0);
                SDL_RenderCopy(renderer, logoType, NULL, &logoTransform);
                SDL_RenderCopy(renderer, button1, NULL, &button1_cord);
                DrawText(renderer, font, "PLAY", { 255,255,255,0 }, SCREEN_WIDTH / 2 - 62, SCREEN_HEIGHT - 360, 100);
                SDL_RenderCopy(renderer, settings_logo, NULL, &settings_card);
                break;

            case 1:

                FillBackground(renderer, 250, 165, 206, 0);
                ButtonBackDraw(button_back, button_back_cord, 0, sounds[1]);
                SDL_RenderCopy(renderer, level1, NULL, &level1_card);
                SDL_RenderCopy(renderer, level2, NULL, &level2_card);
                SDL_RenderCopy(renderer, level3, NULL, &level3_card);

                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    if (isButtonClicked(level1_card, event, sounds[7])) condition = 3;
                    if (isButtonClicked(level2_card, event, sounds[7])) condition = 4;
                    if (isButtonClicked(level3_card, event, sounds[7])) condition = 5;
                }
                break;

            case 2:
                FillBackground(renderer, 250, 165, 206, 0);
                ButtonBackDraw(button_back, button_back_cord, 0, sounds[1]);
                SDL_RenderCopy(renderer, button1, NULL, &button3_cord);
                DrawText(renderer, font, "MUSIC: ", { 255,255,255,0 }, SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT - 480, 80);

                if (musicOn == 1)
                    DrawText(renderer, font, "OFF", { 255,255,255,0 }, SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT - 400, 80);
                else
                    DrawText(renderer, font, "ON", { 255,255,255,0 }, SCREEN_WIDTH / 2 - 40, SCREEN_HEIGHT - 400, 80);

                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                    if (isButtonClicked(button3_cord, event, sounds[1])) {
                        if (musicOn == 1) {
                            if (Mix_PlayingMusic()) {
                                Mix_HaltMusic();
                            }
                            musicOn = 0;
                        }
                        else {
                            if (!Mix_PlayingMusic()) {
                                Mix_PlayMusic(music, -1);
                            }
                            musicOn = 1;
                        }
                        singleSound = false;
                    }
                }
                break;

            case 3:
                RunLevel(renderer, 3, 8, gridImg, elems, selectedImg, event, font, button_back, 20, 100);
                break;
            case 4:
                RunLevel(renderer, 4, 8, gridImg, elems, selectedImg, event, font, button_back, 30, 200);
                break;
            case 5:
                RunLevel(renderer, 5, 10, gridImg, elems, selectedImg, event, font, button_back, 40, 500);
                break;
            case 6:
                FillBackground(renderer, 245, 185, 216, 0);
                DrawText(renderer, font, "YOU WIN", { 255,255,255,0 }, SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 200, 200);
                if (!singleSound) {
                    Mix_PlayChannel(0, sounds[6], 0);
                    singleSound = true;
                }

                SDL_RenderCopy(renderer, button1, NULL, &button1_cord);
                SDL_RenderCopy(renderer, button1, NULL, &button2_cord);
                DrawText(renderer, font, "BACK TO LEVELS", { 255,255,255,0 }, SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT - 330, 40);
                DrawText(renderer, font, "REPLAY", { 255,255,255,0 }, SCREEN_WIDTH / 2 - 70, SCREEN_HEIGHT - 330 + 80, 80);

                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {

                    if (isButtonClicked(button1_cord, event, sounds[1])) {
                        condition = 1;
                        singleSound = false;
                    }
                    if (isButtonClicked(button2_cord, event, sounds[1])) {
                        condition = currentLevel;
                        singleSound = false;
                    }

                }
                break;
            case 7:
                FillBackground(renderer, 0, 0, 0, 0);
                DrawText(renderer, font, "GAMEOVER", { 255,0,0,0 }, SCREEN_WIDTH / 2 - 230, SCREEN_HEIGHT / 2 - 200, 200);
                if (!singleSound) {
                    Mix_PlayChannel(0, sounds[5], 0);
                    singleSound = true;
                }

                SDL_RenderCopy(renderer, button1, NULL, &button1_cord);
                SDL_RenderCopy(renderer, button1, NULL, &button2_cord);
                DrawText(renderer, font, "BACK TO LEVELS", { 255,255,255,0 }, SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT - 330, 40);
                DrawText(renderer, font, "REPLAY", { 255,255,255,0 }, SCREEN_WIDTH / 2 - 70, SCREEN_HEIGHT - 330 + 80, 80);

                if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {

                    if (isButtonClicked(button1_cord, event, sounds[1])) {
                        condition = 1;
                        singleSound = false;
                    }
                    if (isButtonClicked(button2_cord, event, sounds[1])) {
                        condition = currentLevel;
                        singleSound = false;
                    }

                }
                break;
            }

            SDL_RenderPresent(renderer);
        }
    }

    SDL_DestroyTexture(logoType);
    SDL_DestroyTexture(button1);
    SDL_DestroyTexture(button_back);
    SDL_DestroyTexture(settings_logo);
    SDL_DestroyTexture(level1);
    SDL_DestroyTexture(level2);
    SDL_DestroyTexture(level3);

    free(sounds);
    Mix_FreeMusic(music);

    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

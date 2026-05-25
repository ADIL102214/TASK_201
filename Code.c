#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>

#define FALSE 0
#define TRUE 1
#define SNAKE_LENGTH 20

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int gameIsRunning = FALSE;
SDL_Point snake[SNAKE_LENGTH];
int xVelocity = 15;
int yVelocity = 0;

int initialize_window(){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return FALSE;
    }
    window = SDL_CreateWindow(
        "Snakko Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1000,
        800,
        0
    );
    if (!window){
        fprintf(stderr, "Error initializing SDL window: %s\n", SDL_GetError());
        return FALSE;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer){
        fprintf(stderr, "Error creating SDL renderer: %s\n", SDL_GetError());
        return FALSE;
    }
    return TRUE;
}

void setup_snake(){
    for(int i = 0; i < SNAKE_LENGTH; i++){
        snake[i].x = 400 - (i*15);
        snake[i].y = 400;
    }
}

void process_input(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                gameIsRunning = FALSE;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) gameIsRunning = FALSE;
                if (event.key.keysym.sym == SDLK_RIGHT){
                    xVelocity = 15;
                    yVelocity = 0;
                }
                if (event.key.keysym.sym == SDLK_LEFT){
                    xVelocity = -15;
                    yVelocity = 0;
                }
                if (event.key.keysym.sym == SDLK_UP){
                    xVelocity = 0;
                    yVelocity = -15;
                }
                if (event.key.keysym.sym == SDLK_DOWN){
                    xVelocity = 0;
                    yVelocity = 15;
                }
                break;
        }
    }
}

void update() {
    for (int i = SNAKE_LENGTH - 1; i > 0; i--){
        snake[i].x = snake[i - 1].x;
        snake[i].y = snake[i - 1].y;
    }
    snake[0].x += xVelocity;
    snake[0].y += yVelocity;
}

void draw(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    for (int i = 0; i < SNAKE_LENGTH; i++){
        SDL_Rect body = {snake[i].x, snake[i].y, 10, 10};
        if (i == 0){
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }else{
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        SDL_RenderDrawRect(renderer, &body);
    }
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]){
    gameIsRunning = initialize_window();
    setup_snake();
    while (gameIsRunning){
        process_input();
        update();
        draw();
        SDL_Delay(70);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
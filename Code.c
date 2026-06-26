#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include<time.h>
#include <SDL2/SDL_ttf.h>

#define FALSE 0
#define TRUE 1
#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1000

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int gameIsRunning = FALSE;
SDL_Point snake[100];
SDL_Point food;
int snakeLength = 10;
int xVelocity = 15;
int yVelocity = 0;
int blockNumber = 0;
int score = 0;

int initialize_window(){
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return FALSE;
    }
    if(TTF_Init() == -1){
        fprintf(stderr, "Error initializing SDL_ttf: %s\n", TTF_GetError());
        return FALSE;
    }
    window = SDL_CreateWindow(
        "Snakko Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
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
    for(int i = 0; i < snakeLength; i++){
        snake[i].x = 390 - (i*15);
        snake[i].y = 390;
    }
}

void generate_food(){
    food.x = (rand() % (WINDOW_WIDTH/15)*15);
    food.y = (rand() % (WINDOW_HEIGHT/15)*15);
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
    if(snake[0].x <= 0 || snake[0].x >= WINDOW_WIDTH || snake[0].y <= 0 || snake[0].y >= WINDOW_HEIGHT){
        if(blockNumber < snakeLength){
            snake[blockNumber].x = -50;
            snake[blockNumber].y = -50;
            blockNumber++;
            SDL_Delay(5);
        }else{
            gameIsRunning = FALSE;
        }
        return;
    }
    for(int i = 1; i < snakeLength; i++){
        if(snake[0].x == snake[i].x && snake[0].y == snake[i].y){
            if(blockNumber < snakeLength){
                snake[blockNumber].x = -50;
                snake[blockNumber].y = -50;
                blockNumber++;
                SDL_Delay(5);
            }else{
                gameIsRunning = FALSE;
            }
            return;
        }
    }
    for (int i = snakeLength - 1; i > 0; i--){
        snake[i].x = snake[i - 1].x;
        snake[i].y = snake[i - 1].y;
    }
    snake[0].x += xVelocity;
    snake[0].y += yVelocity;
    SDL_Rect head = {snake[0].x, snake[0].y, 10, 10};
    SDL_Rect foodRect = {food.x, food.y, 10, 10};
    if(SDL_HasIntersection(&head, &foodRect)){
        generate_food(); 
        score++;
        snake[snakeLength].x = snake[snakeLength - 1].x;
        snake[snakeLength].y = snake[snakeLength - 1].y;
        snakeLength++;
    }
}

void render_text(){
    TTF_Font *font = TTF_OpenFont("PixelBookOut-Regular.ttf", 40);
    if(font != NULL){
        char string[15];
        sprintf(string, "SCORE : %d", score);
        SDL_Color red = {255, 0, 0, 255};
        SDL_Surface *surface = TTF_RenderText_Solid(font, string, red);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect text ={400, 5, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &text);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        TTF_CloseFont(font);
    }
}

void draw(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    for (int i = 0; i < snakeLength; i++){
        SDL_Rect body = {snake[i].x, snake[i].y, 10, 10};
        if (i == 0){
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }else{
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        SDL_RenderDrawRect(renderer, &body);
    }
    SDL_Rect foodRect = {food.x, food.y, 10, 10};
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &foodRect);
    render_text();
    SDL_RenderPresent(renderer);    
}

int main(int argc, char *argv[]){
    srand(time(NULL));
    gameIsRunning = initialize_window();
    setup_snake();
    generate_food();
    while (gameIsRunning){
        process_input();
        update();
        draw();
        SDL_Delay(70);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();
    return 0;
}
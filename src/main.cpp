#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_timer.h>
#include <SDL3_image/SDL_image.h>
#include <cstddef>
#include <cstdio>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

SDL_Event event;
bool keep = true;
int Init(){
    //设置日志输出级别
    SDL_SetLogPriorities(SDL_LOG_PRIORITY_VERBOSE);
    
    //初始化 SDL 库
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return -1;
    }
    
    //创建窗口和渲染器
    window = SDL_CreateWindow(
        "Renaissance",
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        SDL_Log("Could not create a window: %s", SDL_GetError());
        return -1;
    }
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("Create renderer failed: %s", SDL_GetError());
        return -1;
    }
    return 0;
}
int main(int argc, char* argv[]) {
    if(Init() != 0){
        return -1;
    }
    SDL_Surface* img=IMG_Load("./assets/test.png");

    SDL_Texture* tex=SDL_CreateTextureFromSurface(renderer, img);

    while(keep){
        SDL_PollEvent(&event);
        if(event.type == SDL_EVENT_QUIT){
            keep = false;
            break;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderTexture(renderer, tex, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_Log("Event: %d", event.type);
        SDL_Delay(50);
    }
    return 0;
}

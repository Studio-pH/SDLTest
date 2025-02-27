#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_math.h>
#include <box2d/b2_world.h>
#include <box2d/box2d.h>
#include <cstddef>
#include <cstdio>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

struct Camera{
	Camera(int _x, int _y, int _w, int _h, int _zoom) { 
        x = _x; y = _y; w = _w; h = _h; zoom = _zoom;
    }
	int x, y, w, h, zoom;
}camera(0,0, WINDOW_WIDTH, WINDOW_HEIGHT,30);

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

SDL_Event event;
bool keep = true;

// box2d 
b2Vec2 gravity(0.0f, -10.0f);
b2World world(gravity);
float timeStep = 1.0f / 60.0f;
int32 velocityIterations = 6;
int32 positionIterations = 2;
struct Object{
    //构造函数
    Object( b2World* world,
            float _x = 0.0f, float _y = 0.0f, 
            float _w = 1.0f, float _h = 1.0f,
            float _density = 1.0f ,float _friction = 0.3f){

        bodyDef.type = b2_dynamicBody;//动态身体
        bodyDef.position.Set(_x, _y);
        body = world->CreateBody(&bodyDef);
        Shape.SetAsBox(_w,_h);

        fixtureDef.shape = &Shape;
        fixtureDef.density = _density;   //密度
        fixtureDef.friction = _friction; //摩擦
        body->CreateFixture(&fixtureDef);
    }
    b2BodyDef bodyDef;
    b2Body* body;

    b2PolygonShape Shape;
    
    b2FixtureDef fixtureDef;
};

void DrawBox(b2Body* &body){
    const SDL_FRect R={ 
        camera.x+body->GetPosition().x*camera.zoom,
        camera.y+body->GetPosition().y*camera.zoom,
        1.0f,1.0f
    };
    
}
int Box2d(){
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);

    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 10.0f);

    groundBody->CreateFixture(&groundBox, 0.0f);

    return 0;
}

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
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
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

void Destroy(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if(Init() != 0){
        return -1;
    }

    while(keep){
        SDL_PollEvent(&event);
        if(event.type == SDL_EVENT_QUIT){
            keep = false;
            break;
        }
        
        SDL_RenderPresent(renderer);
        SDL_Log("Event: %d", event.type);
        SDL_Delay(50);
    }

    Destroy();
    return 0;
}

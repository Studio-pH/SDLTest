#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <algorithm>
#include <box2d/b2_body.h>
#include <box2d/b2_common.h>
#include <box2d/b2_math.h>
#include <box2d/box2d.h>
#include <cmath>
#include <cstdio>
#include <vector>
#include "LTE.h"
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;

Camera camera(0,0, WINDOW_WIDTH, WINDOW_HEIGHT,64);

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

SDL_Event event;
bool keep = true;

// box2d 
b2Vec2 gravity(0.0f, -10.0f);
b2World world(gravity);
float timeStep = 1.0f / 60.0f;
int32 velocityIterations = 8;
int32 positionIterations = 3;
struct Object{
    //构造函数
    Object( b2World* world,
            b2Vec2 position,
            b2BodyType bodytype,
            int32 _count,
            b2Vec2 _vertices[],
            float _density,float _friction
            ){
        
        bodyDef.type = bodytype;    //b2_staticBody,b2_kinematicBody,b2_dynamicBody
        bodyDef.position.Set(position.x, position.y);
        body = world->CreateBody(&bodyDef);
        
        for (int i=0; i < _count; i++)
            vertices.push_back(_vertices[i]);
        count = _count;
        Shape.Set(_vertices,_count);

        fixtureDef.shape = &Shape;
        fixtureDef.density = _density;   //密度 1.0f
        fixtureDef.friction = _friction; //摩擦 0.3f
        body->CreateFixture(&fixtureDef);
    }

    b2BodyDef bodyDef;
    b2Body* body;
    
    std::vector<b2Vec2> vertices;
    int32 count;
    b2PolygonShape Shape;

    b2FixtureDef fixtureDef;
};

float ToWPosX(float x){
    return camera.x+WINDOW_WIDTH/2+x*camera.zoom;
}

float ToWPosY(float y){
    return WINDOW_HEIGHT-(camera.y+WINDOW_HEIGHT/2+y*camera.zoom);
}

void LTE_DrawShapes(Object* object){
    b2Vec2 Pos = object->body->GetPosition();
    float Angle = object->body->GetAngle();
    //SDL_Log("Angle%.2f\n",Angle);
    for(int i=0;i<object->count;i++){
        float x1,y1,x2,y2;
        float radius = sqrt((object->vertices[i].x*object->vertices[i].x)+(object->vertices[i].y*object->vertices[i].y));
        x1 = ToWPosX(Pos.x+radius*cos(acos(object->vertices[i].x/radius)+Angle)); 
        y1 = ToWPosY(Pos.y+radius*sin(asin(object->vertices[i].y/radius)+Angle));
        x2 = ToWPosX(Pos.x+radius*cos(acos(object->vertices[(i+1)%object->count].x/radius)+Angle)); 
        y2 = ToWPosY(Pos.y+radius*sin(asin(object->vertices[(i+1)%object->count].y/radius)+Angle));
        //SDL_Log("x1:%.2f, y1:%.2f   x2:%.2f, y2:%.2f\n",x1,y1,x2,y2); 
        SDL_RenderLine(renderer, x1, y1, x2, y2);
    }
    //BUG in this
}
/*
void DrawBox(Object* box){

    b2Vec2 pos = box->body->GetPosition();
    float angle = box->body->GetAngle();

    SDL_FRect Rect =  {
        camera.x+camera.w/2+pos.x*camera.zoom,
        camera.h-(camera.y+camera.h/2+pos.y*camera.zoom),
        box->w*camera.zoom,
        box->h*camera.zoom
    };
    
    SDL_RenderRect(renderer, &Rect);
}*/
int Init();
void Destroy();

int main(int argc, char* argv[]) {
    if(Init() != 0){
        return -1;
    }

    //Box2d();
    //Object box1(&world,0.0f,4.0f,1.0f,1.0f,10.0f,0.3f);
    //Object box2(&world,1.0f,4.0f,1.0f,1.0f,10.0f,0.3f);
    b2Vec2 vertices[5]={
        b2Vec2(0.7,0.1),
        b2Vec2(0.5,0.5),b2Vec2(-0.5,0.5),
        b2Vec2(-0.5,-0.7),b2Vec2(0.5,-0.5)
    };
    Object box(&world,b2Vec2(0.0f,1.4f),b2_dynamicBody,5,vertices,1.0f,0.3f);
    
    b2Vec2 vertices2[4] = {
        b2Vec2(2.0f,0.0f),
        b2Vec2(-2.0f,0.0f),
        b2Vec2(-2.0f,-1.0f),
        b2Vec2(1.8f,-1.0f)
    };

    Object ground(&world,b2Vec2(0.0f,0.0f),b2_staticBody,4,vertices2,1.0f,1.0f);
    
    SDL_FRect R;
    while(keep){
        SDL_PollEvent(&event);
        if(event.type == SDL_EVENT_QUIT){
            keep = false;
            break;
        }

        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);

        world.Step(timeStep, velocityIterations, positionIterations);
        
        //DrawBox(&box1); 
        //DrawBox(&box2);
        LTE_DrawShapes(&box);
        LTE_DrawShapes(&ground);

        SDL_RenderPresent(renderer);
        //SDL_Log("Event: %d", event.type);
        SDL_Delay(50);
    }

    Destroy();
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

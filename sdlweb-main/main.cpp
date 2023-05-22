#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_render.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <emscripten.h>
#include <UiManager.h>



//Define global variables
const char* title = "web test";
int width = 737, height = 550;
SDL_Renderer* renderer;
SDL_Window* window;
SDL_Texture* screen;
SDL_Rect screensize;
int frameCount, timerFPS, lastFrame, fps, lastTime;
int setFPS = 120;
SDL_Point mouse;
int mousex, mousey = 0;
const Uint8* keystates;
Uint32 mousestate;
SDL_Event event;
bool running;
SDL_Color bkg = { 14, 149, 148};

int scene = 0;

//Define classes
class AudioClip {
public:
    AudioClip(const char* clipPath) {
        strcpy(this->file_name, clipPath);

        if (SDL_LoadWAV(clipPath, &(this->spec), &(this->buf), &(this->len)) == NULL) {
            printf("Failed to load wav file: %s\n", SDL_GetError());
        }

        //SDL_OpenAudioDevice(NULL, 0, &(introMusic.spec), NULL, 0);
        device_id = SDL_OpenAudioDevice(NULL, 0, &(this->spec), NULL, 0);
        if (device_id == 0) {
            printf("THESSSS NOPE %s\n", SDL_GetError());
        }
    }

    void Play() {
        SDL_PauseAudioDevice(device_id, 0);
        int succes = SDL_QueueAudio(device_id, this->buf, this->len);
    }

    

private:
    char file_name[100];
    int test;
    SDL_AudioSpec spec;
    Uint32 len;
    Uint8* buf;

    SDL_AudioDeviceID device_id;
};


//Gameplay classes


//Desfine methods and functions


void updateKeys() {
  keystates = SDL_GetKeyboardState(NULL);
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT)
      running = false;
  }
  mousestate = SDL_GetMouseState(&mouse.x, &mouse.y);
}

void setDrawColor(SDL_Color c) { SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a); }

void begin_render() {
      SDL_SetRenderTarget(renderer, screen);
      setDrawColor(bkg);
      SDL_RenderClear(renderer);
      frameCount++;
      timerFPS = SDL_GetTicks()-lastFrame;
      if(timerFPS<(1000/setFPS)) {
        SDL_Delay((1000/setFPS)-timerFPS);
          //SDL_Delay(8);
  }
}

void end_render() {
      SDL_SetRenderTarget(renderer, NULL);
      SDL_RenderCopy(renderer, screen, &screensize, &screensize);
      SDL_RenderPresent(renderer);
}

void init() {
     SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
     SDL_Init(SDL_INIT_EVERYTHING);
     SDL_Init(SDL_INIT_AUDIO);

     window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
     renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
     SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
     SDL_RenderSetLogicalSize(renderer, width, height);
     running = 1;
     screensize.x=screensize.y=0;
     screensize.w=width;screensize.h=height;
     screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
     
     TTF_Init();
     font = TTF_OpenFont("res/Garute-Black.otf", font_size);
}

void quit() {
     running = 0;
     SDL_DestroyRenderer(renderer);
     SDL_DestroyWindow(window);
     SDL_Quit();
}

void loop() {
  end_render();
  lastFrame = SDL_GetTicks();
  if (lastFrame >= (lastTime + 1000)) {
    lastTime = lastFrame;
    fps = frameCount;
    frameCount = 0;
  }
  updateKeys();
  begin_render();
}

EM_JS(int, get_canvas_width, (), { return canvas.width; });
EM_JS(int, get_canvas_height, (), { return canvas.height; });

int canvasHeight = get_canvas_height();
int canvasWidth = get_canvas_width();

//Every scene should need this to render basic things
void renderingBasics() {
    loop();         //Loop that ends last frame rendering and keeps a track of framerate
    SDL_SetWindowSize(window, get_canvas_width(), get_canvas_height());
    canvasWidth = get_canvas_width();
    canvasHeight = get_canvas_height();
    mousex = (int)(mouse.x);
    mousey = (int)(mouse.y);
}


UiManager uiManager;


void mainMenue() {
    
    uiManager.addNewGroup();
    
    StaticImage test = StaticImage(renderer, "res/logo.png", 100, 100, 100, 100, 1980, 1080);
    Text fpsText = Text(renderer, "FPS is", 100, 100);
    
    uiManager.addUi(0, test); //id 0
    uiManager.addUi(0, fpsText); //id 1

    
    uiManager.getUi(0, 0, test)->SetPosition(0,0);
    uiManager.getUi(0, 1, fpsText)->SetText("fuck you fuckers");

    while (true) {
        renderingBasics();
        uiManager.renderUi();
    }
}


//Main loop that runs
int main() {
     init();//initilises variables used in the program
     while(running) {      
          renderingBasics();
          mainMenue();
     }
     quit();
     return 0;
}

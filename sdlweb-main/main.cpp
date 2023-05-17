#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <AL/al.h>
#include <AL/alc.h>
#include <emscripten.h>


//Define global variables
const char* title = "web test";
int width = 550, height = 737;
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


//UI Classes
class StaticImage {
public:
    StaticImage(){}
    
    StaticImage(SDL_Renderer* render, const char* imageName, int x, int y, int w, int h, int imagew, int imageh) {
        this->destrect.x = x;
        this->destrect.y = y;
        this->destrect.w = w;
        this->destrect.h = h;

        this->srcrect.x = 0;
        this->srcrect.y = 0;
        this->srcrect.w = imagew;
        this->srcrect.h = imageh;

        this->render = render;
        
        this->image = IMG_LoadTexture(this->render, imageName);
        if (image == NULL) {
            printf("Failed to load image \n");
        }
    }

    void SetPosition(int x, int y) {
        this->destrect.x = x;
        this->destrect.y = y;
    }

    void Draw() {
        SDL_RenderCopy(this->render, image, &srcrect, &destrect);
    }

private:
    SDL_Rect srcrect;
    SDL_Texture* image;
    SDL_Renderer* render;

protected:
    SDL_Rect destrect;
};

class Button : public StaticImage {
public:
    Button(SDL_Renderer* render, const char* imageName, int x, int y, int w, int h, int imagew, int imageh):StaticImage(render, imageName, x, y, w, h, imagew, imageh) {
 
    }

    bool ClickHandler(int mousex, int mousey) {
        if (mousex < destrect.x || mousex > destrect.x + destrect.w) {
            return false;
        }
        else if (mousey < destrect.y || mousey > destrect.y + destrect.h) {
            return false;
        }
        return true;
    }

    void SetActive(bool newActive) {
            this->active = newActive;     
    }

    bool GetActive() {
        return this->active;
    }

    void Draw() {
        printf("This gets called \n");
        if (active == true) {
            StaticImage::Draw();
        }
    }

private:
    bool active;
};

int font_size = 64;
SDL_Color font_color = { 255,255,255,255 };
TTF_Font* font;
class Text {
public:
    Text(std::string displayText, int x, int y) {
        this->displayText = displayText.c_str();
        //text_surface = TTF_RenderText_Solid(font, this->displayText, font_color);
        text_surface = TTF_RenderText_Blended(font, this->displayText, font_color);
        text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        textRect = { x, y, text_surface->w, text_surface->h };
    }

    void SetText(std::string newText) {
        this->displayText = newText.c_str();
        text_surface = TTF_RenderText_Blended(font, this->displayText, font_color);
        text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    }

    void Draw() {
        SDL_FreeSurface(text_surface);
        SDL_RenderCopy(renderer, text_texture, NULL, &textRect);
        //SDL_DestroyTexture(text_texture);
    }
private:
    const char* displayText;
    int font_size;
    SDL_Color font_color = { 255,255,255,255 };
    SDL_Surface* text_surface;
    SDL_Texture* text_texture;
    SDL_Rect textRect;
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

Text fpsText = Text("FPS is", 100, 100);
void mainMenue() {
    
    
    
    while (true) {
        renderingBasics();
        fpsText.SetText(std::to_string(fps));
        fpsText.Draw();
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

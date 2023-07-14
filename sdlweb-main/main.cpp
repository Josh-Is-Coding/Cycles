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
#include <stdio.h>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <UiManager.h>
#include <chrono>
#include <thread>
#include <3DRendering.h>
#include <emscripten/websocket.h>





//Define global variables
const char* title = "web test";
int width = 737, height = 550;
SDL_Renderer* renderer;
SDL_Window* window;
SDL_Texture* screen;
SDL_Rect screensize;
int frameCount, timerFPS, lastFrame,currentFrame, fps;
int lastTime = 0;
int setFPS = 60;
int barWidth, barHeight;
float scaleX, scaleY;
SDL_Point mouse;
float mousex, mousey = 0.0f;
const Uint8* keystates;
Uint32 mousestate;
SDL_Event event;
bool running;
SDL_Color bkg = { 14, 149, 148};
SDL_Rect playerRect = { width / 2 - 20, height / 2 - 40,40,80 };
PlayerData player(playerRect);
CameraData camera;


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

struct SquareData {
    std::vector<Triangle> triangles;
    SDL_Point vertices[5];
    SDL_Vertex vertex[5];
    SDL_Color color;
};

//Gameplay classes
class SquareRendererPooling {
public:
    SquareRendererPooling() {

        int poolSize = 20;
        squarePool.reserve(poolSize);
        for (int i = 0; i < poolSize; i++) {
            squarePool.push_back(new ObjectData());
        }
        avaliablePool = squarePool;

        worldObjectRenderer = WorldObjectRenderer(renderer, width, height);
    }

    void AddSquare(int xPos, int yPos, int zPos, int rotation, int width, int height, int depth, const char* textureFileName) {

        if (avaliablePool.size() <= 0) {
            //Add more squares to the pool
            ObjectData* newSquare = new ObjectData();
            squarePool.push_back(newSquare);
            avaliablePool.push_back(newSquare);
        }
        
        ObjectData* currentSquare = avaliablePool.back();
        avaliablePool.pop_back();
        usingPool.push_back(currentSquare);

        std::vector<Triangle> allTriangles;

        SDL_Vertex point1; point1.position.x = xPos; point1.position.y = yPos-height; 
        SDL_Vertex point2; point2.position.x = xPos; point2.position.y = yPos; 
        SDL_Vertex point3; point3.position.x = xPos+width; point3.position.y = yPos; 
        std::vector<SDL_Vertex> leftTrianglePoints;
        leftTrianglePoints.push_back(point1);
        leftTrianglePoints.push_back(point2);
        leftTrianglePoints.push_back(point3);

        SDL_Color c{ 255, 255, 255, 255 };
        Triangle leftTriangle(leftTrianglePoints, c, true);
        allTriangles.push_back(leftTriangle);

        SDL_Vertex point1a; point1a.position.x = xPos; point1a.position.y = yPos-height; 
        SDL_Vertex point2a; point2a.position.x = xPos+width; point2a.position.y = yPos - height; 
        SDL_Vertex point3a; point3a.position.x = xPos+width; point3a.position.y = yPos; 
        std::vector<SDL_Vertex> rightTrianglePoints;
        rightTrianglePoints.push_back(point1a);
        rightTrianglePoints.push_back(point2a);
        rightTrianglePoints.push_back(point3a);

        Triangle rightTriangle(rightTrianglePoints, c, false);
        allTriangles.push_back(rightTriangle);

        if (textureFileName != nullptr) {
            allTriangles[0].AddTexture(renderer,textureFileName);
            allTriangles[1].AddTexture(renderer, textureFileName);
        }

        currentSquare->SetAll(allTriangles, xPos, zPos, yPos, rotation, width, height,depth);
        //squarePools.push_back(square);

    }

    ObjectData* GetSquare(int squareId) {
        return usingPool[squareId];
    }

    void renderSquares() {    

        //for (ObjectData* currentSquare : usingPool) {
        //    worldObjectRenderer.renderObject(renderer,currentSquare, &player);
        //}
        worldObjectRenderer.renderObject(renderer, usingPool, &player, &camera);
        
        squaresRenderingThisFrame = 0;
        
    }

    ~SquareRendererPooling() {
        for (ObjectData* currentSquare:squarePool) {
            delete currentSquare;
        }
    }

private:

    int squaresRenderingThisFrame = 0;
    //static std::vector<ObjectData> squaresPool;
    WorldObjectRenderer worldObjectRenderer;
    std::vector<ObjectData*> squarePool;
    std::vector<ObjectData*> avaliablePool;
    std::vector<ObjectData*> usingPool;
    
};


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
      lastFrame = SDL_GetTicks();
      if(timerFPS<(16)) {
        //emscripten_sleep((16) - timerFPS);
      //SDL_Delay(timerFPS);
        
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

     int windowWidth, windowHeight;
     SDL_GetRendererOutputSize(renderer, &windowWidth, &windowHeight);

     int logicalWidth, logicalHeight;
     SDL_RenderGetLogicalSize(renderer, &logicalWidth, &logicalHeight);

     scaleX = static_cast<float>(windowWidth) / logicalWidth;
     scaleY = static_cast<float>(windowHeight) / logicalHeight;

     if (scaleX > 1.0f) {
         barWidth = (windowWidth - logicalWidth) / 2.0f;
     }

     if (scaleY > 1.0f) {
         barHeight = (windowHeight - logicalHeight) / 2.0f;
     }
     
     
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
   //lastFrame = SDL_GetTicks();
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
    mousex = static_cast<float>(mouse.x);
    mousey = static_cast<float>(mouse.y);
}


UiManager uiManager;

SquareRendererPooling squareRenderer;


void mainGame() {
    
    
    
    Text fpsText = Text(renderer, "FPS is", 100, 100);
    StaticImage test = StaticImage(renderer, "res/logo.png", width/2  -20 , height/2 - 40, 40, 80, 1980, 1080);

    squareRenderer.AddSquare(300, 0, 500, 0, 50, 200, 50, nullptr);
    squareRenderer.GetSquare(0)->Set2DPos(700, 500);
    squareRenderer.AddSquare(0, 0, 0, 0, 50, 50, 50, nullptr);
    squareRenderer.AddSquare(500, 0, 550, 0, 100, 100,50, "res/logo2.png");
    //squareRenderer.AddSquare(1500, 0, 550, 0, 100, 100);
    
    camera.RePosition(player.xPos, player.zPos, player.yPos);

    uiManager.addNewGroup();
    Text fpsText2 = Text(renderer, "Player Position Is: ", 10, 10);
    uiManager.addUi(1, fpsText2); //id 0
    uiManager.addUi(1, test);
    

    while (true) {
        uiManager.getUi(0, 1, fpsText)->SetText(std::to_string(fps));
        uiManager.getUi(1, 0, fpsText)->SetText(std::to_string(player.xPos) + std::to_string(player.zPos));

        uiManager.renderUi();
        
        player.moveForward(0.1);
        camera.cameraRotation = player.rotation;
        camera.RePosition(player.xPos, player.zPos, player.yPos);
        squareRenderer.renderSquares();
        renderingBasics();
    }
}

void startGame() {
    printf("starting the game \n");
    emscripten_cancel_main_loop();
    SDL_Delay(1000);
    //uiManager.setGroupActive(0, false);
    emscripten_set_main_loop(mainGame, 0, 1);
}

void mainMenue() {
    
    uiManager.addNewGroup();
    
    StaticImage test = StaticImage(renderer, "res/logo.png", 100, 100, 100, 100, 1980, 1080);
    Text fpsText = Text(renderer, "FPS is", 100, 100);
    Button playButton = Button(renderer, "res/UI/button.png", 62, 300, 400, 100, 1241, 326, startGame);
    
    uiManager.addUi(0, test); //id 0
    uiManager.addUi(0, fpsText); //id 1
    uiManager.addUi(0, playButton); //id 2

    
    uiManager.getUi(0, 0, test)->SetPosition(100,0);
    
    

    while (true) {

       

        //while (SDL_PollEvent(&event))
        //{
        //    switch (event.type)
        //    {
        //    case SDL_KEYDOWN:
        //        break;
        //    case SDL_MOUSEBUTTONDOWN:
        //        if (event.button.button == SDL_BUTTON_LEFT) {
        //            printf("left clicked \n");
        //            bool isUiButton = uiManager.buttonClickCheck(mousex, mousey);
        //            
        //            if (isUiButton == false) {
        //                printf("no clcikable ui surface \n");
        //            }
        //            else {
        //                printf("clicked surface \n");
        //            }
        //        }
        //        break;
        //    }
        //}
        printf("THIS LOOPS RUNNS \n");
        uiManager.renderUi();
        uiManager.getUi(0, 1, fpsText)->SetText(std::to_string(fps));
        renderingBasics();
    }
}

void Rotate(int rotationAmount) {
    player.rotate(rotationAmount);

    while (player.rotation % 90 != 0) {
        player.rotate(rotationAmount);
   }
}

int numTriangles = 1;
EM_BOOL key_callback(int eventType, const EmscriptenKeyboardEvent* e, void* userData) {
    if (eventType == EMSCRIPTEN_EVENT_KEYPRESS && (!strcmp(e->key, "a") || e->which == 97)) {
        player.xPos -= 10 ;
        squareRenderer.AddSquare(300, 0, 500, 0, 50, 200, 50, nullptr);
        //player.zPos -= 10;

     }
    if (eventType == EMSCRIPTEN_EVENT_KEYPRESS && (!strcmp(e->key, "d") || e->which == 100)) {
        player.xPos += 10;
    }

    if (eventType == EMSCRIPTEN_EVENT_KEYPRESS && (!strcmp(e->key, "w") || e->which == 119)) {
        //player.zPos += 10;
        player.moveForward(10);
    }
    if (eventType == EMSCRIPTEN_EVENT_KEYPRESS && (!strcmp(e->key, "s") || e->which == 115)) {
        player.moveForward(-10);
    }

    if (eventType == EMSCRIPTEN_EVENT_KEYPRESS && (!strcmp(e->key, "e"))) {
        Rotate(-10);
    }

    if (eventType == EMSCRIPTEN_EVENT_KEYPRESS && (!strcmp(e->key, "q"))) {
        Rotate(10);
    }

    camera.cameraRotation = player.rotation;
    camera.RePosition(player.xPos, player.zPos, player.yPos);
    //camera.xPos = player.xPos + camera.xOffset;
    //camera.yPos = player.yPos + camera.yOffset;
    //camera.zPos = player.zPos + camera.zOffset;
    

     return 0;
}

EM_BOOL mouse_callback(int eventType, const EmscriptenMouseEvent* e, void* userData) {
    if (eventType == EMSCRIPTEN_EVENT_CLICK) {
        bool isUiButton = uiManager.buttonClickCheck(mousex, mousey);
        printf("The x click is: %f \n", mousex);
        printf("the y click is : %f \n", mousey);
        //printf("the mouse pos is %d , %d \n", mousex, mousey);
    }
    return 0;
}




bool fullScreen = false;
EM_BOOL touch_callback(int eventType, const EmscriptenTouchEvent* e, void* userData)
{
    if (fullScreen == false) {
        fullScreen = true;

    }
    for (int i = 0; i < (e->numTouches)+1; ++i)
    {
        float touchX = static_cast<float>(e->touches[i].targetX) - barWidth;
        float touchY = static_cast<float>(e->touches[i].targetY) - barHeight;
        touchX /= scaleX;
        touchY /= scaleY;
        bool isUiButton = uiManager.buttonClickCheck(touchX, touchY);
        printf("The xtouch is: %f \n", touchX);
        printf("the yTouch is : %f \n", touchY);
    }

    return 0;
}

//Main loop that runs
int main() {
    int isMobile = 0;
      isMobile = EM_ASM_INT({
    return/Mobi|Android/i.test(navigator.userAgent);
          });

     init();//initilises variables used in the program

     if (isMobile == 0){
         emscripten_set_click_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, mouse_callback);
         emscripten_set_keypress_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, key_callback);
     }
     else {
         emscripten_set_touchstart_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, touch_callback);
     }
     
     

     while(running) {      
          renderingBasics();
          emscripten_set_main_loop(mainMenue, 0, 1);
          //mainMenue();
     }
     quit();
     return 0;
}

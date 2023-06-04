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
#include <3DRendering.h>





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
PlayerData player;


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

        std::vector<Triangle> allTriangles;

        SDL_Vertex point1; point1.position.x = 100; point1.position.y = 100; point1.tex_coord.x = 0; point1.tex_coord.y = 0; 
        SDL_Vertex point2; point2.position.x = 100; point2.position.y = 200; point2.tex_coord.x = 0; point2.tex_coord.y = 1; 
        SDL_Vertex point3; point3.position.x = 300; point3.position.y = 400; point3.tex_coord.x = 1; point3.tex_coord.y = 1; 
        std::vector<SDL_Vertex> leftTrianglePoints;
        leftTrianglePoints.push_back(point1);
        leftTrianglePoints.push_back(point2);
        leftTrianglePoints.push_back(point3);

        printf("capoosh \n");
        SDL_Color c{ 175, 178, 255, 200};
        Triangle leftTriangle(leftTrianglePoints, c);
        allTriangles.push_back(leftTriangle);

        SDL_Vertex point1a; point1a.position.x = 100; point1a.position.y = 100; point1a.tex_coord.x = 0; point1a.tex_coord.y = 0;
        SDL_Vertex point2a; point2a.position.x = 300; point2a.position.y = 300; point2a.tex_coord.x = 1; point2a.tex_coord.y = 0;
        SDL_Vertex point3a; point3a.position.x = 300; point3a.position.y = 400; point3a.tex_coord.x = 1; point3a.tex_coord.y = 1;
        std::vector<SDL_Vertex> rightTrianglePoints;
        rightTrianglePoints.push_back(point1a);
        rightTrianglePoints.push_back(point2a);
        rightTrianglePoints.push_back(point3a);

        Triangle rightTriangle(rightTrianglePoints, c);
        allTriangles.push_back(rightTriangle);

        ObjectData square(allTriangles, 0, 0, 0);
        squaresPool.push_back(square);
    }

    void AddSquare(int xPos, int yPos, int rotation, int width, int height) {

    }

    /*
    static void addSquare(std::vector<SDL_Vertex> &points, SDL_Color squareColor) {
        //topleft, bottomleft, bottomright, topright
        printf("stop point 1 %d %d\n", squaresRenderingThisFrame , squaresPool.size());
        int squaresPoolSize = squaresPool.size();
        if (squaresRenderingThisFrame > squaresPoolSize-4) {
            printf("adding more space \n");
            ObjectData square;
            squaresPool.push_back(square);
           
        }
        std::vector<Triangle> triangles;

        printf("stop point 2 \n");
        SDL_Vertex leftSidePoints[3];
        for (int i = 0; i < 3; i++) {
            leftSidePoints[i] = points[i];
            printf("the point is %d %d \n", leftSidePoints[i].position.y, points[0].position.y);
            //squaresPool[squaresRenderingThisFrame].triangles.push_back(leftSide);
        }
        //Triangle leftSide(leftSidePoints,squareColor);
        //triangles.push_back(leftSide);

        //printf("stop point 3 %d\n",leftSide.triangle[1].position.y);
        SDL_Vertex rightSidePoints[3];
        for (int i = 0; i < 3; i++) {
            rightSidePoints[i] = points[i];
            //squaresPool[squaresRenderingThisFrame].triangles.push_back(leftSide);
        }
        //Triangle rightSide(rightSidePoints, squareColor);
        //triangles.push_back(rightSide);


        std::vector<Triangle> testTriangles;

        SDL_Vertex point1; point1.position.x = 100; point1.position.y = 100; point1.tex_coord.x = 0; point1.tex_coord.y = 0; point1.color = squareColor;
        SDL_Vertex point2; point2.position.x = 100; point2.position.y = 200; point2.tex_coord.x = 0; point2.tex_coord.y = 1; point2.color = squareColor;
        SDL_Vertex point3; point3.position.x = 300; point3.position.y = 200; point3.tex_coord.x = 1; point3.tex_coord.y = 1; point3.color = squareColor;

        SDL_Vertex allPoints[3] = {point1, point2, point3};
        Triangle demo(allPoints[3], squareColor);
        testTriangles.push_back(demo);

        printf("stop point 4 \n");
        squaresPool[0].SetAll(testTriangles, 0, 0, 0);

        printf("stop point 5 \n");
        squaresRenderingThisFrame++;

    }
    */
    //static void addSquare(SDL_Point topLeft, SDL_Point bottomLeft, SDL_Point topRight, SDL_Point bottomRight, SDL_Color squareColor) {
    //    if (squaresRenderingThisFrame > squaresPool.size()-1) {
    //        SquareData newSquare;
    //        newSquare.vertex[0].position.x = topLeft.x;
    //        newSquare.vertex[0].position.y = topLeft.y;
    //        newSquare.vertex[0].tex_coord.x = 0;
    //        newSquare.vertex[0].tex_coord.y = 0;
    //        newSquare.vertex[1].position.x = bottomLeft.x;
    //        newSquare.vertex[1].position.y = bottomLeft.y;
    //        newSquare.vertex[1].tex_coord.x = 0;
    //        newSquare.vertex[1].tex_coord.y = 1;
    //        newSquare.vertex[2].position.x = topRight.x;
    //        newSquare.vertex[2].position.y = topRight.y;
    //        newSquare.vertex[2].tex_coord.x = 1;
    //        newSquare.vertex[2].tex_coord.y = 0;
    //        newSquare.vertex[3].position.x = bottomRight.x;
    //        newSquare.vertex[3].position.y = bottomRight.y;
    //        newSquare.vertex[3].tex_coord.x = 1;
    //        newSquare.vertex[3].tex_coord.y = 1;
    //        newSquare.vertex[4].position.x = topLeft.x;
    //        newSquare.vertex[4].position.y = topLeft.y;
    //        newSquare.vertex[4].tex_coord.x = 0;
    //        newSquare.vertex[4].tex_coord.y = 0;
    //        newSquare.color = squareColor;

    //        squaresPool.push_back(newSquare);
    //    }
    //    else {
    //        squaresPool[squaresRenderingThisFrame].vertex[0].position.x = topLeft.x;
    //        squaresPool[squaresRenderingThisFrame].vertex[0].position.y = topLeft.y;
    //        squaresPool[squaresRenderingThisFrame].vertex[0].tex_coord.x = 0;
    //        squaresPool[squaresRenderingThisFrame].vertex[0].tex_coord.y = 0;
    //        squaresPool[squaresRenderingThisFrame].vertex[1].position.x = bottomLeft.x;
    //        squaresPool[squaresRenderingThisFrame].vertex[1].position.y = bottomLeft.y;
    //        squaresPool[squaresRenderingThisFrame].vertex[1].tex_coord.x = 0;
    //        squaresPool[squaresRenderingThisFrame].vertex[1].tex_coord.y = 1;
    //        squaresPool[squaresRenderingThisFrame].vertex[2].position.x = topRight.x;
    //        squaresPool[squaresRenderingThisFrame].vertex[2].position.y = topRight.y;
    //        squaresPool[squaresRenderingThisFrame].vertex[2].tex_coord.x = 1;
    //        squaresPool[squaresRenderingThisFrame].vertex[2].tex_coord.y = 0;
    //        squaresPool[squaresRenderingThisFrame].vertex[3].position.x = bottomRight.x;
    //        squaresPool[squaresRenderingThisFrame].vertex[3].position.y = bottomRight.y;
    //        squaresPool[squaresRenderingThisFrame].vertex[3].tex_coord.x = 1;
    //        squaresPool[squaresRenderingThisFrame].vertex[3].tex_coord.y = 1;
    //        squaresPool[squaresRenderingThisFrame].vertex[4].position.x = topLeft.x;
    //        squaresPool[squaresRenderingThisFrame].vertex[4].position.y = topLeft.y;
    //        squaresPool[squaresRenderingThisFrame].vertex[4].tex_coord.x = 0;
    //        squaresPool[squaresRenderingThisFrame].vertex[4].tex_coord.y = 0;
    //        squaresPool[squaresRenderingThisFrame].color = squareColor;
    //    }
    //    squaresRenderingThisFrame++;
    //}

    static void renderSquares() {

        SDL_Vertex vert[3];
        SDL_Color c{ 175, 178, 255, 200 };
        // center
        vert[0].position.x = 100;
        vert[0].position.y = 100;
        vert[0].tex_coord.x = 0;
        vert[0].tex_coord.y = 0;
        vert[0].color = c;

        // left
        vert[1].position.x = 100;
        vert[1].position.y = 200;
        vert[1].tex_coord.x = 0;
        vert[1].tex_coord.y = 1;
        vert[1].color = c;

        // right 
        vert[2].position.x = 300;
        vert[2].position.y = 200;
        vert[2].tex_coord.x = 1;
        vert[2].tex_coord.y = 1;
        vert[2].color = c;

        
        //Triangle testTriangle;
        //testTriangle.vertex = vert;

        //std::vector<SDL_Vertex> triangle;
        //triangle.push_back(vert[0]);
        //triangle.push_back(vert[1]);
        //triangle.push_back(vert[2]);
        //std::vector<Triangle> testTriangles;
        //testTriangles.push_back(Triangle(triangle));
        //ObjectData testTriangle(testTriangles,0,0,0);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        //SDL_Surface* surface = SDL_CreateRGBSurface(0, 640, 480, 32, 0, 0, 0, 0);  // Create an empty surface
        //SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 0, 0));  // Fill the surface with red 
        //SDL_Texture *demoTexture = SDL_CreateTextureFromSurface(renderer,surface);
        //SDL_RenderGeometry(renderer, NULL, vert, 3, NULL, 0);
        renderObject(renderer, (squaresPool[0]));
        //for (int i = 1; i < squaresRenderingThisFrame; i++) {
        //    //SDL_SetRenderDrawColor(renderer, squaresPool[i].color.r, squaresPool[i].color.g, squaresPool[i].color.b, squaresPool[i].color.a);
        //    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        //    SDL_RenderDrawLines(renderer, squaresPool[i].vertices, 5);
        //    //SDL_RenderDrawRect(renderer, &squaresPool[i]);
        //}
        //SDL_Color colors = {};
        
        SquareRendererPooling::squaresRenderingThisFrame = 0;
        
    }

private:

    static int squaresRenderingThisFrame;
    static std::vector<ObjectData> squaresPool;
};
int SquareRendererPooling::squaresRenderingThisFrame = 0;
std::vector<ObjectData> SquareRendererPooling::squaresPool;


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


void mainGame() {
    renderingBasics();
    SquareRendererPooling squareRenderer;
    
    SDL_Rect testRect = { 0,0,100,100 };
    Text fpsText = Text(renderer, "FPS is", 100, 100);
    //SDL_RenderFillRect(renderer, &testRect);
    SDL_Color red = { 255,0,0,1 };

    SDL_Vertex blank;
    std::vector<SDL_Vertex> square(4);
    for (int i = 0; i < 3; i++) {
        square.push_back(blank);
    }
    square[0].position.x = 0; square[0].position.y = 0;
    square[1].position.x = 0; square[1].position.y = 100;
    square[2].position.x = 100; square[2].position.y = 100;
    square[3].position.x = 100; square[3].position.y = 0;
    

    while (true) {
        renderingBasics();
        uiManager.getUi(0, 1, fpsText)->SetText(std::to_string(fps));
        uiManager.renderUi();
        

        

        //squareRenderer.addSquare(square,red);

        SquareRendererPooling::renderSquares();

        //squareRenderer.addSquare({ 0,0 }, { 0,0 }, { 0,0 }, { 0,0 }, red);
        ////squareRenderer.addSquare({ 0,0 }, { 100,0 }, { 100,100 }, { 0,100 }, red);
        //squareRenderer.addSquare({ 0,0 }, { 100, 50 }, { 100,150 }, { 0,100 }, red);
    }
}

void startGame() {
    printf("starting the game \n");
    emscripten_cancel_main_loop();
    SDL_Delay(10);
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

        renderingBasics();
        uiManager.renderUi();
        uiManager.getUi(0, 1, fpsText)->SetText(std::to_string(fps));
    }
}



EM_BOOL mouse_callback(int eventType, const EmscriptenMouseEvent* e, void* userData) {
    if (eventType == EMSCRIPTEN_EVENT_CLICK) {
        bool isUiButton = uiManager.buttonClickCheck(mousex, mousey);
        //printf("the mouse pos is %d , %d \n", mousex, mousey);
    }
    return 0;
}

EM_BOOL touch_callback(int eventType, const EmscriptenTouchEvent* e, void* userData)
{
    for (int i = 0; i < (e->numTouches)+1; ++i)
    {
        int touchX = e->touches[i].targetX;
        int touchY = e->touches[i].targetY;
        bool isUiButton = uiManager.buttonClickCheck(touchX, touchY);
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

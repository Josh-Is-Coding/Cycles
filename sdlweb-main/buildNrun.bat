@echo off

cd "C:\dev\cycles\sdlweb-main\web"
start em++ ../main.cpp -sASYNCIFY -s WASM=1 -s USE_SDL=2 -s ALLOW_MEMORY_GROWTH=1 -I../include -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS="["png"]" -s USE_SDL_TTF=2 -lopenal -o index.js --use-preload-plugins --preload-file res -s TOTAL_MEMORY=268435456
TIMEOUT 20
start emrun C:\dev\cycles\sdlweb-main\web\index.html
exit
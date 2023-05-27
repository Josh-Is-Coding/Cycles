#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_render.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <functional>
class StaticImage {
public:
    StaticImage() {}

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

typedef void (*ButtonEvent)();

class Button : public StaticImage {
public:
    Button(SDL_Renderer* render, const char* imageName, int x, int y, int w, int h, int imagew, int imageh, ButtonEvent buttonEvent) :StaticImage(render, imageName, x, y, w, h, imagew, imageh) {
        this->buttonEvent = buttonEvent;
        this->active = true;
    }

    bool ClickHandler(int mousex, int mousey) {
        if (mousex < destrect.x || mousex > destrect.x + destrect.w) {
            return false;
        }
        else if (mousey < destrect.y || mousey > destrect.y + destrect.h) {
            return false;
        }
        buttonEvent();
        return true;
    }

    void SetActive(bool newActive) {
        this->active = newActive;
    }

    bool GetActive() {
        return this->active;
    }

    void Draw() {
        if (active == true) {
            StaticImage::Draw();
        }
    }

private:
    bool active;
    ButtonEvent buttonEvent;
};

int font_size = 24;
SDL_Color font_color = { 255,255,255,255 };
TTF_Font* font;
class Text {
public:
    Text(SDL_Renderer* render,std::string displayText, int x, int y) {
        this->displayText = displayText.c_str();
        this->renderer = render;
        //text_surface = TTF_RenderText_Solid(font, this->displayText, font_color);
        text_surface = TTF_RenderText_Blended(font, this->displayText, font_color);
        text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        textRect = { x, y, text_surface->w, text_surface->h };
    }

    void SetText(std::string newText) {
        this->displayText = newText.c_str();
        text_surface = TTF_RenderText_Blended(font, this->displayText, font_color);
        text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        textRect = { textRect.x, textRect.y, text_surface->w, text_surface->h };
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
    SDL_Renderer* renderer;
};

//union UiGroup
//{
//    StaticImage staticImage;
//    Button button;
//    Text text;
//};

struct UiGroup {
    /// How the order of elements added is represented
    /// 0 = staticImage
    /// 1 = button
    /// 2 = text
    std::vector<int> order;
    bool active = true;
    std::vector<StaticImage> staticImages;
    std::vector<Button> buttons;
    std::vector<Text> texts;

};

class UiManager {
public:
	UiManager() {
        numberOfGroups = 0;
	}

    void addNewGroup() {
        //UiGroup newGroup = new UiGroup;
        UiGroup newUi;
        uiGroups.push_back(newUi);
        numberOfGroups++;
    }

    void addUi(int groupNumber, StaticImage newImage) {
        //newImage.SetPosition(400, 200);
        uiGroups[groupNumber].staticImages.push_back(newImage);
        uiGroups[groupNumber].order.push_back(0);
    }

    void addUi(int groupNumber, const Button newButton) {
        uiGroups[groupNumber].buttons.push_back(newButton);
        uiGroups[groupNumber].order.push_back(1);
    }

    void addUi(int groupNumber, const Text newText) {
        uiGroups[groupNumber].texts.push_back(newText);
        uiGroups[groupNumber].order.push_back(2);
    }

    void setGroupActive(int groupNumber,bool active) {
        uiGroups[groupNumber].active = active;
    }
    
    void renderUi() {
        for (UiGroup& currentUiGroup : uiGroups) {
            if (!currentUiGroup.active) {
                continue;
            }
            int groupLength = currentUiGroup.order.size();
            int images = 0;
            int buttons = 0;
            int text = 0;
            for (int i = 0; i < groupLength; i++) {
                switch (currentUiGroup.order[i])
                {
                case(0):
                    currentUiGroup.staticImages[images].Draw();
                    images++;
                    break;
                case(1):
                    currentUiGroup.buttons[buttons].Draw();
                    buttons++;
                    break;
                case(2):
                    currentUiGroup.texts[text].Draw();
                    text++;
                    break;
                default:
                    break;
                }
            }
        }
    }

    StaticImage* getUi(int groupNumber, int uiId, StaticImage overrider) {
        UiGroup* currentUiGroup = &(uiGroups[groupNumber]);
        int currentUiType = 0;
        int index = std::count(currentUiGroup->order.begin(), currentUiGroup->order.end(), currentUiType) - 1;
        StaticImage* selectedImage = &(currentUiGroup->staticImages[index]);
        return selectedImage;
    }
    Button* getUi(int groupNumber, int uiId, Button overrider) {
        UiGroup* currentUiGroup = &(uiGroups[groupNumber]);
        int currentUiType = 1;
        int index = std::count(currentUiGroup->order.begin(), currentUiGroup->order.end(), currentUiType) - 1;
        Button* selectedButton = &(currentUiGroup->buttons[index]);
        return selectedButton;
    }
    Text* getUi(int groupNumber, int uiId, Text overrider) {
        UiGroup* currentUiGroup = &(uiGroups[groupNumber]);
        int currentUiType = 2;
        int index = std::count(currentUiGroup->order.begin(), currentUiGroup->order.end(), currentUiType) - 1;
        Text* selectedText = &(currentUiGroup->texts[index]);
        return selectedText;
    }

    bool buttonClickCheck(int xMousePos, int yMousePos) {
        for (int i = 0; i < uiGroups.size(); i++) {
            if (uiGroups[i].active == false) {
                continue;
            }
            for (int j = 0; j < uiGroups[i].buttons.size(); j++) {
                if (uiGroups[i].buttons[j].ClickHandler(xMousePos, yMousePos) == true) {
                    return true;
                }
            }
        }
        return false;
    }

private:
    int numberOfGroups;
    std::vector<UiGroup> uiGroups;
};
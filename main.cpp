#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_audio.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <random>

double percentageChance = 0.01;
double chance = 0;

bool timerMode = false;
bool streamerMode = false;

bool foxyTime = false;
bool quit = false;
SDL_Window *window;
SDL_Window *streamerWindow;
SDL_Renderer *renderer;
SDL_Renderer *streamerRenderer;
const std::string assetPath = "assets/";
int foxyFrames[] = {386, 387, 389, 390, 391, 392, 393, 394, 395, 396, 397, 398, 399};
const std::string jumpscareSoundPath = "jumpscare.wav";
SDL_Texture* foxyTextures[13];
SDL_Texture* foxyStreamerTextures[13];
long unsigned int lastSecond = 0;

std::string getSettingValue(std::string buffer) {
    return buffer.substr(buffer.find('=') + 1, buffer.length() - buffer.find('='));
}

std::string getSettingTitle(std::string buffer) {
    return buffer.substr(0, buffer.find('='));
}

void setSettings(){
    std::string readBuffer;
    std::ifstream configFile("settings.conf");
    std::string title;

    while(std::getline(configFile, readBuffer)) {
        title = getSettingTitle(readBuffer);
        
        // Chance
        if(title == "chance"){
            percentageChance = std::stold(getSettingValue(readBuffer));
            chance = percentageChance/100;
        }

        // Timer mode
        if(title == "timerMode"){
            timerMode = getSettingValue(readBuffer) == "true";
        }

        // Streamer mode
        if(title == "streamerMode"){
            streamerMode = getSettingValue(readBuffer) == "true";
        }

    }
}

int main(){ 
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    setSettings();
    
    SDL_DisplayID *displayID = SDL_GetDisplays(NULL);
    const SDL_DisplayMode *display = SDL_GetDesktopDisplayMode(*displayID);
    if(display == NULL){
        std::cerr << "DisplayMode Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("Foxy", display->w, display->h, SDL_WINDOW_TRANSPARENT | SDL_WINDOW_BORDERLESS | SDL_WINDOW_ALWAYS_ON_TOP);
    renderer = SDL_CreateRenderer(window, NULL);
    if(streamerMode){
        streamerWindow = SDL_CreateWindow("Duplicate Foxy", display->w/2, display->h/2, SDL_WINDOW_TRANSPARENT);
        streamerRenderer = SDL_CreateRenderer(streamerWindow, NULL);
    } 
    lastSecond = time(0);

    srand(time(0));

    for(int i = 0; i < 13; i++) {
        std::string texturePath = assetPath + std::to_string(foxyFrames[i]) + ".png";
        SDL_Surface* surface = SDL_LoadPNG(texturePath.c_str());
        foxyTextures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        if(streamerMode)
            foxyStreamerTextures[i] = SDL_CreateTextureFromSurface(streamerRenderer, surface);
        SDL_DestroySurface(surface);
    }

    SDL_AudioSpec jumpscareAudioSpec;
    Uint8 *jumpscareAudioData;
    Uint32 jumpscareAudioLen;
    if(!SDL_LoadWAV((assetPath + jumpscareSoundPath).c_str(), &jumpscareAudioSpec, &jumpscareAudioData, &jumpscareAudioLen)){
        std::cerr << "Audio Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_AudioStream *audioStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &jumpscareAudioSpec, NULL, NULL);
    SDL_ResumeAudioStreamDevice(audioStream);

    while(!quit){
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        if(!timerMode){
            for(int keyCode = 8; keyCode <= 190; keyCode++)
                if(GetAsyncKeyState(keyCode) & 0x8000){
                    if(rand() % (int)(1/chance) == 0)
                        foxyTime = true;
                    while(GetAsyncKeyState(keyCode) & 0x8000);
                }
        }
        else
        {
            if(lastSecond < time(NULL)){
                if(rand() % (int)(1/chance) == 0)
                    foxyTime = true;
                lastSecond = time(0);
            }
        }
        
        if(foxyTime) {
            SDL_ShowWindow(window);
            SDL_PutAudioStreamData(audioStream, jumpscareAudioData, jumpscareAudioLen);
            for(int i = 0; i < 13; i++) {
                SDL_RenderClear(renderer);
                SDL_FRect foxtangle = {0.0f, 0.0f, (float)display->w, (float)display->h};
                SDL_RenderTexture(renderer, foxyTextures[i], NULL, &foxtangle);
                SDL_RenderPresent(renderer);
                if(streamerMode) {
                    SDL_RenderClear(streamerRenderer);
                    SDL_FRect streamerFoxtangle = {0.0f, 0.0f, (float)display->w/2, (float)display->h/2};
                    SDL_RenderTexture(streamerRenderer, foxyStreamerTextures[i], NULL, &streamerFoxtangle);
                    SDL_RenderPresent(streamerRenderer);
                }
                SDL_Delay(1000/24);
            }
            SDL_Delay(200);
            if(streamerMode){
                SDL_RenderClear(streamerRenderer);
                SDL_RenderPoint(streamerRenderer, 0, 0);
                SDL_RenderPresent(streamerRenderer);
            }
            foxyTime = false;
        }
        else {
            SDL_HideWindow(window);
            SDL_Delay(6);
        }
    }
}
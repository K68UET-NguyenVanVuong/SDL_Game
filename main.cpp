#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <bits/stdc++.h>
using namespace std;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gTexture = NULL;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
bool ai = false;
bool aishootself;
bool init() {
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        success = false;
    } else {
        gWindow = SDL_CreateWindow("Game quay co", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
            success = false;
        } else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL) {
                cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
                success = false;
            } else {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
                    success = false;
                }
            }
        }
        SDL_Init(SDL_INIT_AUDIO);
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    }


    return success;
}

SDL_Texture* loadTexture(const string& path) {
    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        cout << "Unable to load image " << path.c_str() << "! SDL_image Error: " << IMG_GetError() << endl;
    } else {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            cout << "Unable to create texture from " << path.c_str() << "! SDL Error: " << SDL_GetError() << endl;
        }
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

bool loadMedia() {
    gTexture = loadTexture("Rules.jpg");
    return gTexture != NULL;
}

bool loadGame(int turn) {
    bool success = true;
    SDL_Surface* loadedSurface;
    if (turn == 0) loadedSurface = IMG_Load("anhloadgame/youshootyou.jpg");
    else if (turn == 1) loadedSurface = IMG_Load("anhloadgame/youshootai.jpg");
    else if (turn == 2) loadedSurface = IMG_Load("anhloadgame/youwaiting.jpg");
    else if (turn == 3) loadedSurface = IMG_Load("anhloadgame/aiwaiting.jpg");
    else if (turn == 4) loadedSurface = IMG_Load("anhloadgame/aishootyou.jpg");
    else if (turn == 5) loadedSurface = IMG_Load("anhloadgame/aishootai.jpg");
    else if (turn == 6) loadedSurface = IMG_Load("anhloadresult/youloseaishootyou.jpg");
    else if (turn == 7) loadedSurface = IMG_Load("anhloadresult/youwinaishootai.jpg");
    else if (turn == 8) loadedSurface = IMG_Load("anhloadresult/youwinyoushootai.jpg");
    else if (turn == 9) loadedSurface = IMG_Load("anhloadresult/youloseyoushootyou.jpg");
    if (loadedSurface == NULL) {
        cout << "Unable to load image: " << IMG_GetError() << endl;
        success = false;
    } else {
        gTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (gTexture == NULL) {
            cout << "Unable to create texture from surface! SDL Error: " << SDL_GetError() << endl;
            success = false;
        }
        SDL_FreeSurface(loadedSurface);
    }
    return success;
}
bool soundGame(int turn){
                bool success = true;
                Mix_Chunk* sound = NULL;
                if (turn == 1) sound = Mix_LoadWAV("amthanh/tiếng súng trúng.wav");
                else if (turn == 2) sound = Mix_LoadWAV("amthanh/tiếng súng cạch.wav");
                else sound = Mix_LoadWAV("amthanh/tiếng nạp đạn.wav");
                if (sound == NULL){
                        cout << "Loi khai bao am thanh";
                        success = false;
                }
                else{
                    Mix_PlayChannel(-1, sound, 0);
                    while (Mix_Playing(-1) != 0);
                     // Giải phóng bộ nhớ
                        Mix_FreeChunk(sound);

                }
                return success;
}

void close() {
    SDL_DestroyTexture(gTexture);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);

    gTexture = NULL;
    gRenderer = NULL;
    gWindow = NULL;
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

int randomInRange(int n) {

    srand(time(0));
    int randomNumber = rand() % n;

    return randomNumber;
}
bool randomAi(){
    srand(time(0));
    int random = rand() % 2;
    return (random == 1);
}
int main(int argc, char* arg[]) {
    int turn = 2;
    vector <int> nongsung(6,0);
    int danthat = randomInRange(5);
    nongsung[danthat] = 1;
    int thutu = 0;
    if (!init()) {
        cout << "Failed to initialize!" << endl;
    } else {
        if (!loadMedia()) {
            cout << "Failed to load media!" << endl;
        } else {//Vong lap game bat dau
            bool quit = false;
            SDL_Event e;

            while (!quit) {
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    } else if (e.type == SDL_KEYDOWN) {

                        if (e.key.keysym.sym == SDLK_SPACE) {
                            if (!loadGame(turn)) {
                                cout << "Failed to load game!" << endl;
                            }
                        }
                        if (ai == false){
                        //Bắn đối thủ
                        if (e.key.keysym.sym == SDLK_RIGHT) {
                        SDL_Delay(1000);
                        loadGame(1);
                        SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
                        SDL_RenderPresent(gRenderer);
                        if (nongsung[0] == 0){
                            SDL_Delay(1000);
                            loadGame(3);
                            danthat -= 1;
                            SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
                            SDL_RenderPresent(gRenderer);
                            soundGame(2);
                            ai = true;
                            nongsung.erase(nongsung.begin());
                        }
                        else if (nongsung[0] == 1){
                            SDL_Delay(1000);
                            loadGame(8);
                            soundGame(1);
                            SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
                            SDL_RenderPresent(gRenderer);
                            SDL_Delay(1000);
                            quit = true;
                        }
                        }

                        //Bắn bản thân
                        else if (e.key.keysym.sym == SDLK_LEFT) {
                            SDL_Delay(1000);
                            if (!loadGame(0)) {
                                cout << "Failed to load game!" << endl;
                            }
                            SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
                            SDL_RenderPresent(gRenderer);
                            //Bắn bản thân, bắn trượt nên được bắn tiếp
                            if (nongsung[thutu] == 0){
                                nongsung.erase(nongsung.begin());
                                danthat -= 1;
                                ai = false;
                                SDL_Delay(1000);
                                loadGame(2);
                            SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
                            SDL_RenderPresent(gRenderer);
                            soundGame(2);
                            SDL_Delay(1000);
                            }
                            //Bắn bản thân, bắn đạn trúng nên người chơi thua
                            else if (nongsung[thutu] == 1){
                                soundGame(1);
                                SDL_Delay(1000);
                                loadGame(9);
                                SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
                                SDL_RenderPresent(gRenderer);
                                SDL_Delay(1000);
                                quit = true;
                            }
                        }
                        //Xoay nòng súng
                        else if (e.key.keysym.sym == SDLK_UP){
                            int dancu = danthat;
                            danthat = randomInRange(nongsung.size());
                            nongsung[dancu] = 0;
                            nongsung[danthat] = 1;
                            soundGame(0);
                        }
                    }//Vòng lặp xử lí người chơi
                        while (ai){
                            SDL_RenderClear(gRenderer);
                            aishootself = randomAi();
                            bool swiss = randomAi();
                            //Máy bắn bản thân
                            if ((aishootself == true)&&(nongsung.size() != 1)){
                                SDL_Delay(1000);
                                //vòng lặp nạp đạn của bot
                                while(swiss == true){
                                    int dancu = danthat;
                                    danthat = randomInRange(nongsung.size());
                                    cout << dancu << " " << danthat;
                                    nongsung[dancu] = 0;
                                    nongsung[danthat] = 1;
                                    soundGame(0);
                                    swiss = randomAi();
                                }
                                loadGame(5);
                                SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
                                SDL_RenderPresent(gRenderer);
                                if (nongsung[0] == 0){
                                        danthat -= 1;
                                       SDL_Delay(1000);
                                       loadGame(3);
                                    SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
                                    SDL_RenderPresent(gRenderer);
                                    nongsung.erase(nongsung.begin());
                                    soundGame(2);
                                    ai = true;

                                }
                                    else if (nongsung[0] == 1){
                                    SDL_Delay(1000);
                                    danthat -= 1;
                                    loadGame(7);
                                    SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
                                    SDL_RenderPresent(gRenderer);
                                    soundGame(1);
                                    SDL_Delay(1000);
                                    quit = true;
                                    break;
                                }
                            }
                            //Máy bắn người chơi
                            if ((aishootself == false)||(nongsung.size()== 1)){
                                SDL_Delay(1000);
                                swiss = randomAi();
                                while (swiss == true){
                                    int dancu = danthat;
                                    danthat = randomInRange(nongsung.size());
                                    nongsung[dancu] = 0;
                                    nongsung[danthat] = 1;
                                    soundGame(0);
                                    swiss = randomAi();
                                }
                                loadGame(4);
                                SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
                                SDL_RenderPresent(gRenderer);


                                if (nongsung[0] == 0){
                                    SDL_Delay(1000);
                                    loadGame(2);
                                    SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
                                    SDL_RenderPresent(gRenderer);
                                    danthat -= 1;
                                    nongsung.erase(nongsung.begin());
                                    soundGame(2);
                                    ai = false;
                                    SDL_Delay(1000);

                                    break;
                                }
                                else if (nongsung[0] == 1){
                                    SDL_Delay(1000);
                                    soundGame(1);
                                    loadGame(6);
                                    SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
                                    SDL_RenderPresent(gRenderer);
                                    danthat -= 1;
                                    SDL_Delay(1000);

                                    quit = true;
                                    break;
                                }
                            }
                    }//Vòng lặp xử lí máy
                    }//vong lap chạy ứng dụng
                SDL_RenderClear(gRenderer);
                SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
                SDL_RenderPresent(gRenderer);
                }
            }
        }//Vong lap ket thuc
    }

    close();
    return 0;
}

#include <cmath>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <random>
#include "Screen.h"
#include "GoL.h"

namespace methods{
    Screen::Screen():
    m_window(nullptr),
    m_renderer(nullptr),
    m_texture(nullptr),
    m_buffer(nullptr),
    m_buffer_copy(nullptr),
    zoom(5),
    clearMem(true),
    autoScroll(0),
    animation_speed(0.0001),
    golInitiated(false){}

    bool Screen::init(){
        if(SDL_Init(SDL_INIT_VIDEO < 0)){
            SDL_GetError();
            return false;
        }
        m_window = SDL_CreateWindow("Particle Explosion",
                                               SDL_WINDOWPOS_CENTERED,
                                               SDL_WINDOWPOS_CENTERED,
                                               SCREEN_WIDTH,
                                               SCREEN_HEIGHT,
                                               SDL_WINDOW_SHOWN);
        if(m_window == nullptr){
            SDL_GetError();
            SDL_Quit();
            return false;
        }

        m_renderer = SDL_CreateRenderer(m_window, -1, 0);
//        SDL_RenderSetScale(m_renderer, 2, 1);
//        SDL_RenderSetLogicalSize(m_renderer, 1600, 1200);
        m_texture = SDL_CreateTexture(m_renderer,

                                                 SDL_PIXELFORMAT_ABGR8888,
                                                 SDL_TEXTUREACCESS_STATIC,
                                                 SCREEN_WIDTH,
                                                 SCREEN_HEIGHT);
        if(m_renderer == nullptr){
            SDL_DestroyWindow(m_window);
            SDL_GetError();
            SDL_Quit();
            return false;
        }
        if(m_texture == nullptr){
            SDL_DestroyRenderer(m_renderer);
            SDL_DestroyWindow(m_window);
            SDL_GetError();
            SDL_Quit();
            return false;
        }

        // screen buffer
        m_buffer = new Uint32[SCREEN_WIDTH*SCREEN_HEIGHT];
        memset(m_buffer, 0, SCREEN_WIDTH*SCREEN_HEIGHT*sizeof(Uint32));
        // buffer copy
        m_buffer_copy = new Uint32[SCREEN_WIDTH*SCREEN_HEIGHT];
        memset(m_buffer_copy, 0, SCREEN_WIDTH*SCREEN_HEIGHT*sizeof(Uint32));
        return true;
    }
    unsigned char Screen::menu(){
        std::cout<<"Press number for graphics:"
                   "\n1.\tSolid Color"
                   "\n2.\tGrid"
                   "\n3.\tCursor"
                   "\n4.\tGame of Life"<<std::endl;
        int choice;
        int gridSize = 0;
        std::cin>>choice;
        if(choice==2){
            std::cout<<"What grid size will you like to use?"<<std::endl;

            std::cin>>gridSize;
            if(gridSize > 15){gridSize=15;}
        }
        unsigned char answer = choice;
        answer <<= 4;
        answer += gridSize;
        return answer;
    }
    void Screen::update(){
        SDL_UpdateTexture(m_texture, nullptr, m_buffer, SCREEN_WIDTH*sizeof(Uint32));
        SDL_RenderClear(m_renderer);
        SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
        SDL_RenderPresent(m_renderer);
    }
    void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue){

        Uint32 color = 0;

        color += 0xFF;
        color <<=8;
        color += blue;
        color <<=8;
        color += green;
        color <<=8;
        color += red;

//        std::cout<<std::setfill('0')<<std::setw(8)<<std::hex<<color<<std::endl;

        m_buffer[(y*SCREEN_WIDTH)+x] = color;
    }
    void Screen::updateColors(unsigned char &red, unsigned char &green, unsigned char &blue, Uint32 elapsed) const{
        red = (unsigned char) ((1 + std::sin(elapsed * animation_speed)) * 128);
        green = (unsigned char) ((1 + std::sin(elapsed * animation_speed*2)) * 128);
        blue = (unsigned char) ((1 + std::sin(elapsed * animation_speed*3)) * 128);
    }
    void Screen::solidColorScreen(Uint8 red, Uint8 green, Uint8 blue) {
        for(int y=0; y<SCREEN_HEIGHT; y++){
            for(int x=0; x<SCREEN_WIDTH; x++){
                setPixel(x, y,red,green,blue);
            }
        }
    }
    void Screen::colorGrid(Uint8 red, Uint8 green, Uint8 blue, int gridSize){
        if(clearMem) {
            memset(m_buffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
        }
        for(int y=0; y<SCREEN_HEIGHT; y++){
            for(int x=0; x<SCREEN_WIDTH; x++){
                if(x%(int)(gridSize*zoom+1)==0 || y%(int)(gridSize*zoom+1)==0 || x==Screen::SCREEN_WIDTH-1 || y==Screen::SCREEN_HEIGHT-1) {
                    setPixel(x, y, red, green, blue);
                }
            }
        }
    }
    void Screen::cursorLines(Uint8 red, Uint8 green, Uint8 blue){
        std::cout<<autoScroll<<std::endl;
        int mouse_x;
        int mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);
        if(clearMem) {
            memset(m_buffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
        }
        if(autoScroll<0) {
            for (int y = 0; y < SCREEN_HEIGHT; y++) {
                setPixel(mouse_x, y, red, green, blue);
            }
            for (int x = 0; x < SCREEN_WIDTH; x++) {
                setPixel(x, mouse_y, red, green, blue);
            }
        }else if(autoScroll>0){
            setPixel(mouse_x, mouse_y, red, green, blue);
        }
    }

    bool Screen::gameOfLifeRandom(){
        // originate a random board
        if(!golInitiated){
            for(int x=0; x<SCREEN_WIDTH; x++){
                for(int y=0; y<SCREEN_HEIGHT; y++) {
                    m_buffer[(y*SCREEN_WIDTH)+x] = (0xFFFFFFFF)*(rand()%10==0);
                }
            }
            golInitiated=true;
            return false;
        }
        // Update every 1 second
//        return false;
        SDL_Delay(5000);
//        std::cout<<"Updating..."<<std::endl;

        // update cells in copy under laws described in GoL class logic
        for(int x=0; x<SCREEN_WIDTH; x++){
            for(int y=0; y<SCREEN_HEIGHT; y++){
                // live or dead cell
                m_buffer_copy[(y*SCREEN_WIDTH)+x] = 0xFFFFFFFF*(GoL::liveOrDead(x, y, SCREEN_WIDTH, SCREEN_HEIGHT, m_buffer));
            }
        }
        // copy buffer_copy into buffer
        for(int x=0; x<SCREEN_WIDTH; x++){
            for(int y=0; y<SCREEN_HEIGHT; y++){
                // live or dead cell
                m_buffer[(y*SCREEN_WIDTH)+x] = 0xFFFFFFFF*(m_buffer_copy[(y*SCREEN_WIDTH)+x] == 0xFFFFFFFF);
            }
        }
        return true;
    }

    void Screen::mouseEvents(SDL_Event &event){
        if(event.type == SDL_MOUSEWHEEL && autoScroll==0){
            std::cout<<event.wheel.y<<std::endl;
            std::cout<<zoom<<std::endl;
            zoom += event.wheel.y * 0.1;
            if (zoom < 0.0) {
                zoom = 0.0;
            }
        }
        if(event.type == SDL_MOUSEBUTTONDOWN){
            if(event.button.button==SDL_BUTTON_MIDDLE){
                clearMem = !clearMem;
            }
            if(event.button.button==SDL_BUTTON_RIGHT) {
                if(autoScroll<0){
                    autoScroll=0;
                }else{
                    autoScroll=-0.02;
                }
            }else if(event.button.clicks==1 && event.button.button==SDL_BUTTON_LEFT){
                if (autoScroll == 0) {
                    autoScroll = 0.02;
                } else if(autoScroll>0) {
                    autoScroll = 0;
                }
            }
        }
    }

    bool Screen::processEvents(){
        SDL_Event event;
        if(autoScroll!=0){
            zoom+=autoScroll;
        }
        while(SDL_PollEvent(&event)){
            mouseEvents(event);
            if(event.type == SDL_QUIT){
                return false;
            }
        }
        return true;
    }
    float Screen::getAnimationSpeed() {return animation_speed;}
    void Screen::close(){
        delete []m_buffer;
        delete []m_buffer_copy;
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyTexture(m_texture);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }
}
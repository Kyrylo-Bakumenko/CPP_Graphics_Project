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
    sm_x(-1),
    sm_y(-1),
    zoom(5),
    clearMem(true),
    autoScroll(0),
    animation_speed(0.0001),
    golInitiated(false),
    golStepSpeed(100){}

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
                   "\n4.\tCursor Filled"
                   "\n5.\tGame of Life"
                   "\n6.\tGame of Life (Seed & Density)"
                   "\n7.\tGrowth Random"
                   "\n8.\tGrowth (Seed & Density)"
                   "\n9.\tStarry Sky"<<std::endl;
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

        if(x<0 || x>=SCREEN_WIDTH || y<0 || y>=SCREEN_HEIGHT){return;}

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
            // cross-hair fill
            for (int y = 0; y < SCREEN_HEIGHT; y++) {
                setPixel(mouse_x, y, red, green, blue);
            }
            for (int x = 0; x < SCREEN_WIDTH; x++) {
                setPixel(x, mouse_y, red, green, blue);
            }
        // single pixel brush
        }else if(autoScroll>0){
            setPixel(mouse_x, mouse_y, red, green, blue);
        }
    }

    void Screen::cursorLinesFilled(Uint8 red, Uint8 green, Uint8 blue){
        std::cout<<autoScroll<<std::endl;
        int mouse_x;
        int mouse_y;
        SDL_GetMouseState(&mouse_x, &mouse_y);
        if(clearMem) {
            memset(m_buffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
        }
        if(autoScroll<0) {
            // cross-hair fill
            // smooth transitions
            if(sm_x != -1) {
                if (sm_x < mouse_x) {
                    for (int x = sm_x; x <= mouse_x; x++) {
                        for (int y = 0; y < SCREEN_HEIGHT; y++) {
                            setPixel(x, y, red, green, blue);
                        }
                    }
                } else {
                    for (int x = mouse_x; x <= sm_x; x++) {
                        for (int y = 0; y < SCREEN_HEIGHT; y++) {
                            setPixel(x, y, red, green, blue);
                        }
                    }
                }
                if (sm_y < mouse_y) {
                    for (int y = sm_y; y <= mouse_y; y++) {
                        for (int x = 0; x < SCREEN_WIDTH; x++) {
                            setPixel(x, y, red, green, blue);
                        }
                    }
                }else{
                    for (int y = mouse_y; y <= sm_x; y++) {
                        for (int x = 0; x < SCREEN_WIDTH; x++) {
                            setPixel(x, y, red, green, blue);
                        }
                    }
                }
            }
            else {
                for (int y = 0; y < SCREEN_HEIGHT; y++) {
                    setPixel(mouse_x, y, red, green, blue);
                }
                for (int x = 0; x < SCREEN_WIDTH; x++) {
                    setPixel(x, mouse_y, red, green, blue);
                }
            }
            // single pixel brush
        }else if(autoScroll>0){
            if(sm_x != -1){
                while(sm_x != mouse_x || sm_y != mouse_y){
                    setPixel(sm_x, sm_y, red, green, blue);
                    if( abs(mouse_x-sm_x) >= abs(mouse_y-sm_y) ){
                        // update x
                        sm_x += (mouse_x>sm_x)*2 -1;
                    }else{
                        // update y
                        sm_y += (mouse_y>sm_y)*2 -1;
                    }
                }
                setPixel(mouse_x, mouse_y, red, green, blue);
            }
//                int counter = 0;
//                int x = sm_x;
//                int y = sm_y;
//                std::cout<<"Start x: "<<sm_x<<"\tEnd x: "<<mouse_x<<std::endl;
//                if(sm_x != mouse_x && sm_y != mouse_y) {
//                    for (x=sm_x; x <= mouse_x; x++) {
//                        std::cout << "X: " << x << "\t Y: " << y << std::endl;
//                        counter++;
//                        std::cout << (mouse_x - sm_x) << std::endl;
//                        std::cout << (mouse_y - sm_y) << std::endl;
//                        if((mouse_x - sm_x)>abs(mouse_y - sm_y)) {
//                            if ((counter + 1) % (mouse_x - sm_x)/(abs(mouse_y - sm_y)) == 0) {
//                                y += (mouse_y > sm_y) * 2 - 1;
//                            }
//                        }else{
//
//                        }
//                        std::cout << "Check 1" << std::endl;
//                        setPixel(x, y, red, green, blue);
//                    }
//                }
//                std::cout<<"Check 2"<<std::endl;
//                // fill to account for round downs
//                while(x<=mouse_x){
//                    setPixel(x, y, red, green, blue);
//                    x++;
//                }
//                std::cout<<"Check 3"<<std::endl;
//                while(y<=mouse_y){
//                    setPixel(x, y, red, green, blue);
//                    y++;
//                }
//            }else {
//                setPixel(mouse_x, mouse_y, red, green, blue);
//            }
        }
        // update last known location of cursor
        sm_x = mouse_x;
        sm_y = mouse_y;
    }

    void Screen::gameOfLifeRandom(){
        // originate a random board
        if(!golInitiated){
            // use random to assign every pixel 0xFF (white) or 0x00 (black)
            for(int x=0; x<SCREEN_WIDTH; x++){
                for(int y=0; y<SCREEN_HEIGHT; y++) {
                    m_buffer[(y*SCREEN_WIDTH)+x] = (0xFFFFFFFF)*(std::rand()%50==0);
                }
            }
            golInitiated=true;
            return;
        }
        // Update game frame every folStepSpeed milliseconds
        SDL_Delay(golStepSpeed);

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
    }

    void Screen::gameOfLifeFromSeed(unsigned int seed, unsigned int density){
        // originate a board from seed
        if(!golInitiated){
            // generate new seed for random
            srand(seed);
            // use random to assign every pixel 0xFF (white) or 0x00 (black) with frequency according to density
            for(int x=0; x<SCREEN_WIDTH; x++){
                for(int y=0; y<SCREEN_HEIGHT; y++) {
                    m_buffer[(y*SCREEN_WIDTH)+x] = (0xFFFFFFFF)*(std::rand()%density==0);
                }
            }
            golInitiated=true;
            return;
        }
        // Update game frame every folStepSpeed milliseconds
        SDL_Delay(golStepSpeed);

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
    }

    void Screen::growthRandom(){
        // originate a random board
        if(!golInitiated){
            // use random to assign every pixel 0xFF (white) or 0x00 (black)
            for(int x=0; x<SCREEN_WIDTH; x++){
                for(int y=0; y<SCREEN_HEIGHT; y++) {
                    m_buffer[(y*SCREEN_WIDTH)+x] = (0xFFFFFFFF)*(std::rand()%50==0);
                }
            }
            golInitiated=true;
            return;
        }
        // Update game frame every folStepSpeed milliseconds
        SDL_Delay(golStepSpeed);

        // update cells in copy under laws described in GoL class logic
        for(int x=0; x<SCREEN_WIDTH; x++){
            for(int y=0; y<SCREEN_HEIGHT; y++){
                // live or dead cell
                m_buffer[(y*SCREEN_WIDTH)+x] = 0xFFFFFFFF*(GoL::liveOrDead(x, y, SCREEN_WIDTH, SCREEN_HEIGHT, m_buffer));
            }
        }
    }

    void Screen::growthFromSeed(unsigned int seed, unsigned int density){
        // originate a board from seed
        if(!golInitiated){
            // generate new seed for random
            srand(seed);
            // use random to assign every pixel 0xFF (white) or 0x00 (black) with frequency according to density
            for(int x=0; x<SCREEN_WIDTH; x++){
                for(int y=0; y<SCREEN_HEIGHT; y++) {
                    m_buffer[(y*SCREEN_WIDTH)+x] = (0xFFFFFFFF)*(std::rand()%density==0);
                }
            }
            golInitiated=true;
            return;
        }
        // Update game frame every folStepSpeed milliseconds
        SDL_Delay(golStepSpeed);

        // update cells in copy under laws described in GoL class logic
        for(int x=0; x<SCREEN_WIDTH; x++){
            for(int y=0; y<SCREEN_HEIGHT; y++){
                // live or dead cell
                m_buffer[(y*SCREEN_WIDTH)+x] = 0xFFFFFFFF*(GoL::liveOrDead(x, y, SCREEN_WIDTH, SCREEN_HEIGHT, m_buffer));
            }
        }
    }

    void Screen::mouseEvents(SDL_Event &event){
        if(golInitiated){
            if (event.type == SDL_MOUSEWHEEL) {
                golStepSpeed += 2 * event.wheel.y;
                if (golStepSpeed < 0) {
                    golStepSpeed = 0;
                }
                if(golStepSpeed==0){
                    std::cout<<"MAXIMUM OVERDRIVE!!!!!!"<<std::endl;
                }else{
                    std::cout<<"New frame every: "<<golStepSpeed<<" milliseconds"<<std::endl;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    golStepSpeed -= 100;
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    golStepSpeed += 100;
                }
                if (golStepSpeed < 0) {
                    golStepSpeed = 0;
                }
                if(golStepSpeed==0){
                    std::cout<<"MAXIMUM OVERDRIVE!!!!!!"<<std::endl;
                }else{
                    std::cout<<"New frame every: "<<golStepSpeed<<" milliseconds"<<std::endl;
                }
            }
        }else {
            if (event.type == SDL_MOUSEWHEEL && autoScroll == 0) {
                std::cout << event.wheel.y << std::endl;
                std::cout << zoom << std::endl;
                zoom += event.wheel.y * 0.1;
                if (zoom < 0.0) {
                    zoom = 0.0;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_MIDDLE) {
                    clearMem = !clearMem;
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    if (autoScroll < 0) {
                        autoScroll = 0;
                    } else {
                        autoScroll = -0.02;
                    }
                } else if (event.button.clicks == 1 && event.button.button == SDL_BUTTON_LEFT) {
                    if (autoScroll == 0) {
                        autoScroll = 0.02;
                    } else if (autoScroll > 0) {
                        autoScroll = 0;
                    }
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
    bool Screen::isGolInitiated(){return golInitiated;}
    void Screen::close(){
        delete[] m_buffer;
        delete[] m_buffer_copy;
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyTexture(m_texture);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
    }
}
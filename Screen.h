
#ifndef COLLECTOR_SCREEN_H
#define COLLECTOR_SCREEN_H
#include <SDL.h>

namespace methods {
    class Screen {
    public:
        const static int SCREEN_WIDTH = 1600;
        const static int SCREEN_HEIGHT = 1200;
    private:
        SDL_Window *m_window;
        SDL_Renderer *m_renderer;
        SDL_Texture *m_texture;
        Uint32 *m_buffer;
        Uint32 *m_buffer_copy;
        int sm_x;
        int sm_y;
        float zoom;
        bool clearMem;
        float autoScroll;
        float animation_speed;
        bool golInitiated;
        int golStepSpeed;
    public:
        Screen();
        bool init();
        static unsigned char menu();
        void update();
        void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue);
        void updateColors(unsigned char &red, unsigned char &green, unsigned char &blue, Uint32 elapsed) const;
        void solidColorScreen(Uint8 red, Uint8 green, Uint8 blue);
        void colorGrid(Uint8 red, Uint8 green, Uint8 blue, int gridSize);
        void cursorLines(Uint8 red, Uint8 green, Uint8 blue);
        void cursorLinesFilled(Uint8 red, Uint8 green, Uint8 blue);
        void gameOfLifeRandom();
        void gameOfLifeFromSeed(unsigned int seed, unsigned int density);
        void growthRandom();
        void growthFromSeed(unsigned int seed, unsigned int density);
        void mouseEvents(SDL_Event &event);
        bool processEvents();
        float getAnimationSpeed();
        bool isGolInitiated();
        void close();

    };
}


#endif //COLLECTOR_SCREEN_H

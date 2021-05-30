
#ifndef COLLECTOR_SCREEN_H
#define COLLECTOR_SCREEN_H
#include <SDL.h>

namespace methods {
    class Screen {
    public:
        const static int SCREEN_WIDTH = 800;
        const static int SCREEN_HEIGHT = 600;
    private:
        SDL_Window *m_window;
        SDL_Renderer *m_renderer;
        SDL_Texture *m_texture;
        Uint32 *m_buffer;
        Uint32 *m_buffer_copy;
        float zoom;
        bool clearMem;
        float autoScroll;
        float animation_speed;
        bool golInitiated;
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
        bool gameOfLifeRandom();
        void mouseEvents(SDL_Event &event);
        bool processEvents();
        float getAnimationSpeed();
        void close();

    };
}


#endif //COLLECTOR_SCREEN_H

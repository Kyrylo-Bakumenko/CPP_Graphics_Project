
#ifndef COLLECTOR_GOL_H
#define COLLECTOR_GOL_H
#include <SDL.h>

class GoL {

public:
    static int numSurroundingCellsAlive(const int x, const int y, const int width, const int height, const Uint32 *m_buffer){
        int count = 0;
        for(int row = y-1; row<=y+1; row++){
            for(int col = x-1; col<=x+1; col++){
                if(row >= 0 && row < height && col >=0 && col < width && row!=y && col!=x){
                    if(m_buffer[row*width+y]==0xFFFFFFFF){
                        count++;
                    }
                }
            }
        }
        return count;
    }
    static bool liveOrDead(const int x, const int y, const int width, const int height, Uint32 *m_buffer){
        int count;
        count = numSurroundingCellsAlive(x, y, width, height, m_buffer);
        // rules for GoL
        // Any live cell with two or three live neighbours survives.
        // Any dead cell with three live neighbours becomes a live cell.
        // All other live cells die in the next generation. Similarly, all other dead cells stay dead.
        return (count==2 || count==3);
    }
};


#endif //COLLECTOR_GOL_H

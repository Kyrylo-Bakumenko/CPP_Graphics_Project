
#ifndef COLLECTOR_GOL_H
#define COLLECTOR_GOL_H
#include <SDL.h>

class GoL {

public:
    static int numSurroundingCellsAlive(const int x, const int y, const int width, const int height, const Uint32 *m_buffer){
        int count = 0;
        for(int row = x-1; row<=x+1; row++){
            for(int col = y-1; col<=y+1; col++){
                if(row >= 0 && row < width && col >=0 && col < height && !(row==x && col==y)){
                    if(m_buffer[col*width+row]==0xFFFFFFFF){
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

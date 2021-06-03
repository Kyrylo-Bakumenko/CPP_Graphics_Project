
#ifndef COLLECTOR_SWARM_H
#define COLLECTOR_SWARM_H

#include "Particle.h"

namespace methods {

class Swarm {
public:
    const static int NPARTICLES = 5000;
private:
    Particle *m_pParticles;

public:
    Swarm();
    virtual ~Swarm();
    const Particle *const getParticles(){return m_pParticles;}
};

}


#endif //COLLECTOR_SWARM_H

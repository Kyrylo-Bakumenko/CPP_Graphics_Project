
#include "Swarm.h"
namespace methods{

Swarm::Swarm() {
    m_pParticles = new Particle[NPARTICLES];
}

Swarm::~Swarm() {
    delete[] m_pParticles;
}

}
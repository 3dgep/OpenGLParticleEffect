#pragma once;

#include "Particle.h"

class ParticleEmitter
{
public:
    virtual ~ParticleEmitter() {}
    virtual void EmitParticle( Particle& particle ) = 0;

    virtual void DebugRender() {}
};
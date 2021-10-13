#include "ParticleSystemPCH.h"
#include "Random.h"
#include "CubeEmitter.h"
CubeEmitter::CubeEmitter()
: MinWidth(-1)
, MaxWidth(1)
, MinHeight(-1)
, MaxHeight(1)
, MinDepth(-1)
, MaxDepth(1)
, MinSpeed(10)
, MaxSpeed(20)
, MinLifetime(3)
, MaxLifetime(5)
, Origin(0)
{}

void CubeEmitter::EmitParticle( Particle& particle )
{
    float X = RandRange( MinWidth, MaxWidth );
    float Y = RandRange( MinHeight, MaxHeight );
    float Z = RandRange( MinDepth, MaxDepth );

    float lifetime = RandRange( MinLifetime, MaxLifetime );
    float speed = RandRange( MinSpeed, MaxSpeed );

    glm::vec3 vector( X, Y, Z );

    particle.m_Position = vector + Origin;
    particle.m_Velocity = glm::normalize(vector) * speed;

    particle.m_fLifeTime = lifetime;
    particle.m_fAge = 0;
}

void CubeEmitter::DebugRender()
{
    glTranslatef( Origin.x, Origin.y, Origin.z );

    glColor4f( 1.0f, 0.0f, 1.0f, 1.0f );
    
    glutWireCube( MaxWidth * 2 );

    glTranslatef( -Origin.x, -Origin.y, -Origin.z );
}

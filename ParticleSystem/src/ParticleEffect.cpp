#include "ParticleSystemPCH.h"
#include "ParticleEmitter.h"
#include "Camera.h"
#include "Random.h"
#include "ParticleEffect.h"

ParticleEffect::ParticleEffect( unsigned int numParticles /* = 0 */ )
: m_pCamera( NULL )
, m_pParticleEmitter( NULL )
, m_ColorInterpolator( glm::vec4(1) )
, m_LocalToWorldMatrix(1)
, m_TextureID(0)
, m_Force( 0, -9.81f, 0 ) 
{
    Resize(numParticles);
}

ParticleEffect::~ParticleEffect()
{
    if ( m_TextureID != 0 )
    {
        glDeleteTextures( 1, &m_TextureID );
        m_TextureID = 0;
    }
}

void ParticleEffect::SetCamera( Camera* pCamera )
{
    m_pCamera = pCamera;
}

void ParticleEffect::SetParticleEmitter( ParticleEmitter* pEmitter )
{
    m_pParticleEmitter = pEmitter;
}

void ParticleEffect::SetColorInterplator( const ColorInterpolator& colors )
{
    m_ColorInterpolator = colors;
}

bool ParticleEffect::LoadTexture( const std::string& fileName )
{
    if ( m_TextureID != 0 )
    {
        glDeleteTextures(1, &m_TextureID );
    }

    m_TextureID = SOIL_load_OGL_texture( fileName.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS );

    return ( m_TextureID != 0 );
}

void ParticleEffect::RandomizeParticle( Particle& particle )
{
    particle.m_fAge = 0.0f;
    particle.m_fLifeTime = RandRange( 3, 5 );

    glm::vec3 unitVec = RandUnitVec();

    particle.m_Position = unitVec * 1.0f;
    particle.m_Velocity = unitVec * RandRange( 10, 20 );
}

void ParticleEffect::RandomizeParticles()
{
    for ( unsigned int i = 0; i < m_Particles.size(); ++i )
    {
        RandomizeParticle(m_Particles[i]);
    }
}

void ParticleEffect::EmitParticle( Particle& particle )
{
    assert( m_pParticleEmitter != NULL );
    m_pParticleEmitter->EmitParticle( particle );
}

void ParticleEffect::EmitParticles()
{
    if ( m_pParticleEmitter == NULL )
    {
        RandomizeParticles();
    }
    else 
    {
        for ( unsigned int i = 0; i < m_Particles.size(); ++i )
        {
            EmitParticle(m_Particles[i]);
        }
    }
}

void ParticleEffect::BuildVertexBuffer()
{
    const glm::vec3 X( 0.5, 0, 0 );
    const glm::vec3 Y( 0, 0.5, 0 );
    const glm::vec3 Z( 0, 0 ,1.0 );

    glm::quat cameraRotation;

    if ( m_pCamera != NULL )
    {
        cameraRotation = glm::quat( glm::radians(m_pCamera->GetRotation()) );    
    }

    // Make sure the vertex buffer has enough vertices to render the effect
    // If the vertex buffer is already the correct size, no change is made.
    m_VertexBuffer.resize(m_Particles.size() * 4, Vertex() );

    for ( unsigned int i = 0; i < m_Particles.size(); ++i )
    {
        Particle& particle = m_Particles[i];
        glm::quat rotation = glm::angleAxis( particle.m_fRotate, Z );

        unsigned int vertexIndex = i * 4;
        Vertex& v0 = m_VertexBuffer[vertexIndex + 0];   // Bottom-left
        Vertex& v1 = m_VertexBuffer[vertexIndex + 1];   // Bottom-right
        Vertex& v2 = m_VertexBuffer[vertexIndex + 2];   // Top-right
        Vertex& v3 = m_VertexBuffer[vertexIndex + 3];   // Top-left

        // Bottom-left
        v0.m_Pos = particle.m_Position + ( rotation * ( -X - Y ) * particle.m_fSize ) * cameraRotation;
        v0.m_Tex0 = glm::vec2( 0, 1 );
        v0.m_Diffuse = particle.m_Color;

        // Bottom-right
        v1.m_Pos = particle.m_Position + ( rotation * ( X - Y ) * particle.m_fSize ) * cameraRotation;
        v1.m_Tex0 = glm::vec2( 1, 1 );
        v1.m_Diffuse = particle.m_Color;

        // Top-right
        v2.m_Pos = particle.m_Position + ( rotation * ( X + Y ) * particle.m_fSize ) * cameraRotation;
        v2.m_Tex0 = glm::vec2( 1, 0 );
        v2.m_Diffuse = particle.m_Color;

        // Top-left
        v3.m_Pos = particle.m_Position + ( rotation * ( -X + Y ) * particle.m_fSize ) * cameraRotation;
        v3.m_Tex0 = glm::vec2( 0, 0 );
        v3.m_Diffuse = particle.m_Color;
    }
    
}

void ParticleEffect::Update(float fDeltaTime)
{
    for ( unsigned int i = 0; i < m_Particles.size(); ++i )
    {
        Particle& particle = m_Particles[i];

        particle.m_fAge += fDeltaTime;
        if ( particle.m_fAge > particle.m_fLifeTime )
        {
            if ( m_pParticleEmitter != NULL ) EmitParticle(particle);
            else RandomizeParticle(particle);
        }

        float lifeRatio = glm::saturate(particle.m_fAge / particle.m_fLifeTime);
        particle.m_Velocity += ( m_Force * fDeltaTime );
        particle.m_Position += ( particle.m_Velocity * fDeltaTime );
        particle.m_Color = m_ColorInterpolator.GetValue( lifeRatio );
        particle.m_fRotate = glm::lerp<float>( 0.0f, 720.0f, lifeRatio );
        particle.m_fSize = glm::lerp<float>( 5.0f, 0.0f, lifeRatio );
    }

    BuildVertexBuffer();
}

void ParticleEffect::Render()
{
    glDisable(GL_DEPTH_TEST);           // Disables Depth Testing
    glEnable(GL_BLEND);                 // Enable Blending
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);   // Type Of Blending To Perform
    glEnable(GL_TEXTURE_2D);            // Enable textures

    glPushMatrix();
    glMultMatrixf( glm::value_ptr(m_LocalToWorldMatrix) );
    
    glBindTexture( GL_TEXTURE_2D, m_TextureID );

    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );

    glVertexPointer( 3, GL_FLOAT, sizeof(Vertex), &(m_VertexBuffer[0].m_Pos) );
    glTexCoordPointer( 2, GL_FLOAT, sizeof(Vertex), &(m_VertexBuffer[0].m_Tex0) );
    glColorPointer( 4, GL_FLOAT, sizeof(Vertex), &(m_VertexBuffer[0].m_Diffuse) );

    glDrawArrays( GL_QUADS, 0, m_VertexBuffer.size() );

    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    glDisableClientState( GL_COLOR_ARRAY );

    glBindTexture( GL_TEXTURE_2D, 0 );

#if _DEBUG
    if ( m_pParticleEmitter != NULL )
    {
        m_pParticleEmitter->DebugRender();
    }
#endif

    glPopMatrix();

}

void ParticleEffect::Resize( unsigned int numParticles )
{
    m_Particles.resize( numParticles,  Particle() );
    m_VertexBuffer.resize( numParticles * 4, Vertex() );
}

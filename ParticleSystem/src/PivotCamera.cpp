#include "ParticleSystemPCH.h"
#include "PivotCamera.h"

PivotCamera::PivotCamera()
: m_PivotPoint( 0 )
{

}

glm::vec3 PivotCamera::ProjectOntoUnitSphere( glm::vec2 screenPos )
{
    float x, y, z;
    float radius = std::min( m_ViewportWidth, m_ViewportHeight );

    x = ( screenPos.x / radius ) * 2.0f;    // range [0 .. 2]
    y = ( screenPos.y / radius ) * 2.0f;   // range [0 .. 2]
    x -= 1.0f;                                       // range [-1 .. 1]
    y -= 1.0f; //  - result.y;                             // range [
    z = 1.0f - ( x * x ) + ( y * y );
 
    z = ( z > 0.0f ) ? sqrtf( z ) : 0.0f;

    return glm::vec3( x, y, z );
}

void PivotCamera::Update( float fDeltaTime )
{
    base::Update(fDeltaTime);

}
void PivotCamera::TranslatePivotX( float x )
{
    glm::quat rot = glm::quat( glm::radians(m_Rotate) );
    m_PivotPoint += glm::vec3( x, 0, 0 ) * rot;
}

void PivotCamera::TranslatePivotY( float y )
{
    glm::quat rot = glm::quat( glm::radians(m_Rotate) );
    m_PivotPoint += glm::vec3( 0, y, 0 ) * rot;
}

void PivotCamera::TranslatePivotZ( float z )
{
    glm::quat rot = glm::quat( glm::radians(m_Rotate) );
    m_PivotPoint += glm::vec3( 0, 0, z ) * rot;
}

void PivotCamera::SetPivot(  const glm::vec3& pivot )
{
    m_PivotPoint = pivot;
}

glm::vec3 PivotCamera::GetPivot() const
{
    return m_PivotPoint;
}

void PivotCamera::ApplyViewTransform()
{
    glTranslatef( 0.0f, 0.0f, -m_Translate.z );
    glRotatef( m_Rotate.z, 0.0f, 0.0f, 1.0f );
    glRotatef( m_Rotate.y, 0.0f, 1.0f, 0.0f );
    glRotatef( m_Rotate.x, 1.0f, 0.0f, 0.0f );
    glTranslatef( -m_PivotPoint.x, -m_PivotPoint.y, -m_PivotPoint.z );
}


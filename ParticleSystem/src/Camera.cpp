#include "ParticleSystemPCH.h"
#include "Camera.h"

void Camera::Update( float fDeltaTime )
{

}

void Camera::SetViewport( int x, int y, int width, int height )
{
    m_ViewportX = x;
    m_ViewportY = y;
    m_ViewportWidth = width;
    m_ViewportHeight = std::max( height, 1 );
}

void Camera::SetProjection( float fovy, float aspect, float zNear, float zFar )
{
    m_fVFOV = fovy;
    m_fAspect = aspect;
    m_fNear = zNear;
    m_fFar = zFar;
}

void Camera::AddPitch( float fPitch )
{
    m_Rotate.x += fPitch;
}

void Camera::AddYaw( float fYaw )
{
    m_Rotate.y += fYaw;
}

void Camera::AddRoll( float fRoll )
{
    m_Rotate.z += fRoll;
}

void Camera::AddRotation( const glm::quat& rot )
{
    glm::quat finalRotate = rot * glm::quat( m_Rotate );
    m_Rotate = glm::eularAngles( finalRotate );
}

void Camera::TranslateX( float x )
{
    m_Translate.x += x;
}

void Camera::TranslateY( float y )
{
    m_Translate.y += y;
}

void Camera::TranslateZ( float z )
{
    m_Translate.z += z;
}

void Camera::SetTranslate( const glm::vec3& translate )
{
    m_Translate = translate;
}

void Camera::SetRotate( float pitch, float yaw, float roll )
{
    m_Rotate = glm::vec3( pitch, yaw, roll );
}

void Camera::SetRotate( const glm::vec3& rotate )
{
    m_Rotate = rotate;
}

glm::vec3 Camera::GetTranslation() const
{
    return m_Translate;
}

glm::vec3 Camera::GetRotation() const
{
    return m_Rotate;
}

void Camera::ApplyViewport()
{
    glViewport( m_ViewportX, m_ViewportY, m_ViewportWidth, m_ViewportHeight );
}

void Camera::ApplyProjectionTransform()
{
    glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
    gluPerspective( m_fVFOV, m_fAspect, m_fNear, m_fFar );
}

void Camera::ApplyViewTransform()
{
    glMatrixMode( GL_MODELVIEW );
    glTranslatef( m_Translate.x, m_Translate.y, m_Translate.z );
    glRotatef( m_Rotate.x, 1.0f, 0.0f, 0.0f );
    glRotatef( m_Rotate.y, 0.0f, 1.0f, 0.0f );
    glRotatef( m_Rotate.z, 0.0f, 0.0f, 1.0f );
}

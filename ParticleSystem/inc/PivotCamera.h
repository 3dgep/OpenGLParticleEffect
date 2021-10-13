#pragma once;

#include "Camera.h"

class PivotCamera : public Camera
{
public:
    typedef Camera base;

    PivotCamera();

    virtual void Update( float fDeltaTime );
    virtual void ApplyViewTransform();

    // Project a position in screen coordinates onto a unit sphere 
    // centered at the origin and return the projected point .
    glm::vec3 ProjectOntoUnitSphere( glm::vec2 screenPos );

    void TranslatePivotX( float x );
    void TranslatePivotY( float y );
    void TranslatePivotZ( float z );

    void SetPivot( const glm::vec3& pivot );
    glm::vec3 GetPivot() const;

protected:
    // The point around which the camera will pivot.
    glm::vec3   m_PivotPoint;

private:

};
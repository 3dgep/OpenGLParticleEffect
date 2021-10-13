#pragma once;

class Camera
{
public:
    Camera() : m_Translate(0) {}
    virtual ~Camera() {};

    // Set viewport parameters
    void SetViewport( int x, int y, int width, int height );
    // Set projection prameters
    void SetProjection( float fovy, float aspect, float zNear, float zFar );

    // Add this pitch (rotation about the X-axis) in degrees
    // to the current camera's pitch 
    void AddPitch( float fPitch );
    // Add this yaw (rotation about the Y-axis) in degrees
    // to the current camera's yaw
    void AddYaw( float fYaw );
    // Add this roll (rotation about the Z-axis) in degrees 
    // to the current camera's roll
    void AddRoll( float fRoll );

    void AddRotation( const glm::quat& rot );

    // Translate the along the camera's X-axis
    void TranslateX( float x );
    // Translate along the camera's Y-axis
    void TranslateY( float y );
    // Translate along the camera's Z-axis
    void TranslateZ( float z );

    // Set the current translation of the camera, replacing the current value.
    void SetTranslate( const glm::vec3& translate );
    // Set the current euler angles in degrees, replacing the current values.
    void SetRotate( float pitch, float yaw, float roll );
    void SetRotate( const glm::vec3& rotate );

    glm::vec3 GetTranslation() const;
    glm::vec3 GetRotation() const;

    virtual void Update( float fDeltaTime ) = 0;

    virtual void ApplyViewport();
    virtual void ApplyProjectionTransform();

    virtual void ApplyViewTransform() = 0;
protected:

    // Viewport parameters
    int         m_ViewportX;
    int         m_ViewportY;
    int         m_ViewportWidth;
    int         m_ViewportHeight;
    
    // Projection parameters
    float       m_fVFOV;
    float       m_fAspect;
    float       m_fNear;
    float       m_fFar;

    // View space parameters
    glm::vec3   m_Translate;
    // Rotate in pitch, yaw, roll
    glm::vec3   m_Rotate;
private:

};
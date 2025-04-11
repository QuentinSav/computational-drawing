#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default values
const float YAW          = -90.0f;
const float PITCH        =  0.0f;
const float ROLL         =  0.0f;
const float SPEED        =  1.0f;
const float SENSITIVITY  =  0.1f;
const float ZOOM         =  45.0f;
const glm::vec3 POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 UP       = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 FRONT    = glm::vec3(0.0f, 0.0f, -1.0f);

class Camera 
{
public:
    Camera(glm::vec3 position = POSITION, glm::vec3 up = UP, float yaw = YAW, float pitch = PITCH);
    
    void processTranslation(Camera_Movement direction, const float& frame_delta_time);
    void processRotation(float& offset_x, float& offset_y);
    void processZoom(float& offset_y);
    float getZoom();
    glm::vec3 getPosition();
    glm::mat4 getViewMatrix();
    
private:
    void updateCameraVectors();
    
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_world_up;
    
    float m_yaw;
    float m_pitch;
    float m_roll;
    
    float m_movement_speed;
    float m_mouse_sensitivity;
    float m_zoom;
};

#endif // CAMERA_H
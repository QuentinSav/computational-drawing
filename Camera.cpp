#include "Camera.h"

// TODO: implement quaternion's method for camera to avoid Gimbal lock

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch):
        m_position(position),
        m_up(up),
        m_yaw(yaw),
        m_pitch(pitch),
        m_world_up(up),
        m_front(FRONT),
        m_movement_speed(SPEED),
        m_mouse_sensitivity(SENSITIVITY),
        m_zoom(ZOOM) 
{
    updateCameraVectors();
}

void Camera::processTranslation(Camera_Movement direction, const float& frame_delta_time)
{
    // Compute velocity based on the frame delta time and movement speed constant
    float velocity = frame_delta_time * m_movement_speed;

    // Compute new position of the camera
    if (direction == FORWARD)
        m_position += velocity * m_front;
    if (direction == BACKWARD)
        m_position -= velocity * m_front;
    if (direction == LEFT)
        m_position -= velocity * m_right;
    if (direction == RIGHT)
        m_position += velocity * m_right;
}

void Camera::processRotation(float& offset_x, float& offset_y)
{
    // Compute Euler angles
    m_yaw   += (offset_x * m_mouse_sensitivity);
    m_pitch += (offset_y * m_mouse_sensitivity);

    // Set max and min angles for pitch
    if (m_pitch > 89.0f) m_pitch = 89.0f;
    if (m_pitch < -89.0f) m_pitch = -89.0f;

    // Update vectors
    updateCameraVectors();
}

void Camera::processZoom(float& offset_y)
{
    // Compute zoom
    m_zoom -= offset_y;
    
    // Set min and max angles for zoom
    if (m_zoom < 1.0f) m_zoom = 1.0f;
    if (m_zoom > 45.0f) m_zoom = 45.0f;
}

glm::mat4 Camera::getViewMatrix()
{
    // Return view coordinate matrix
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::updateCameraVectors()
{
    // Compute the new front direction vector
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    // Compute each direction based on the front vector and the world up
    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_world_up));  
    m_up    = glm::normalize(glm::cross(m_right, m_front));
}

glm::vec3 Camera::getPosition()
{
    return m_position;
}

float Camera::getZoom()
{
    return m_zoom;
}
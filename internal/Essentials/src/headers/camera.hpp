#pragma once
#include "functions.hpp"

class Camera {
public:
    enum Mode {
        perspective,
        orthographic
    };

private:
    glm::mat4 rollMatrix;
    glm::vec3 scaleV;
    glm::vec3 position;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::vec3 forwardAxis;
    glm::vec3 upAxis = glm::vec3(0.0, 1.0, 0.0);

    Mode projectionMode;
    float yaw, pitch, roll;

    void updateYawAndPitch();

public:
    static Camera mainCamera;
    Camera(glm::vec3 startPos, Mode projectionMode);
    void setProjectionMode(Mode projectionMode);
    Mode getProjectionMode();
    void rollRotate(float degree);
    void pitchRotate(float degree);
    void yawRotate(float degree);
    void scale(glm::vec3 scale);
    void apply(unsigned int shader) const;
    void recalculate();
    void localTranslate(glm::vec3 translate);
    void globalTranslate(glm::vec3 translate);
    void lookAt(glm::vec3 targetPos);
    glm::vec3 right();
    glm::vec3 up();
    glm::vec3 forward();
    glm::vec3 getPos();
};

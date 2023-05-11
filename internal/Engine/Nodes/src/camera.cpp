#include "Node/camera.hpp"
#include "Node/node.hpp"
#include "engine.hpp"

#define PI 3.14159265359f
#define EPSILONE 0.1

namespace Engine::Node {
// Camera Engine::Nodes::Camera::mainCamera = Camera(glm::vec3(0.0, 0.0, 3.0), Camera::Mode::perspective);

Camera::Camera(glm::vec3 startPos, Mode projection) {
    this->position = startPos;
    this->rollMatrix = glm::mat4(1.0f);
    this->scaleV = glm::vec3(1.0, 1.0, 1.0);
    this->setProjectionMode(projection);
    this->lookAt(glm::vec3(0.0, 0.0, -1.0) + startPos);
    std::cout << pitch << "," << yaw << ":" << this->forwardAxis << std::endl;
    this->recalculate();
}

void Camera::setProjectionMode(Mode mode) {
    this->projectionMode = mode;
    if (this->projectionMode == Mode::perspective) {
        this->projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    } else if (this->projectionMode == Mode::orthographic) {
        this->projectionMatrix = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        // this->projectionMatrix = glm::mat4(1.0f);
    }
}

Camera::Mode Camera::getProjectionMode() {
    return this->projectionMode;
}

void Camera::rollRotate(float degree) {
    this->roll += glm::radians(degree);
    auto axis = glm::vec3(0.0, 0.0, 1.0);

    auto rdeg = glm::radians(degree);
    this->rollMatrix = glm::rotate(glm::mat4(1.0f), rdeg, axis) * this->rollMatrix;
    // glm::rotate(this->rollMatrix, glm::radians(degree), axis);
    // this->forwardAxis = rt * this->forwardAxis;

    // std::cout << "actuall pitch : " << this->pitch << " | calculated : " << glm::asin(-this->forwardAxis.y) << std::endl;
    // std::cout << "actual yaw : " << this->yaw << " | calculated : " << atan2(this->forwardAxis.x, -this->forwardAxis.y) << std::endl;
    // this->pitch = glm::asin(-this->forwardAxis.y);
    // this->yaw = atan2(this->forwardAxis.x, this->forwardAxis.z);
    this->recalculate();
    // this->updateYawAndPitch();
}

void Camera::pitchRotate(float degree) {
    auto rdeg = glm::radians(degree);
    auto add_yaw = rdeg * sin(this->roll);
    auto add_pitch = rdeg * cos(this->roll);
    this->pitch += add_pitch; // glm::radians(degree);
    this->yaw += add_yaw;

    this->updateYawAndPitch();
}

void Camera::yawRotate(float degree) {
    auto rdeg = glm::radians(degree);
    auto add_yaw = rdeg * cos(this->roll);
    auto add_pitch = rdeg * sin(this->roll);
    this->pitch += add_pitch; // glm::radians(degree);
    this->yaw += add_yaw;

    this->updateYawAndPitch();
}

void Camera::updateYawAndPitch() {
    // this->forwardAxis = this->rollMatrix * this->forwardAxis;

    this->forwardAxis.x = cos(yaw) * cos(pitch);
    this->forwardAxis.y = sin(pitch);
    this->forwardAxis.z = sin(yaw) * cos(pitch);

    std::cout << this->pitch << " ," << this->yaw << std::endl;

    // std::cout << "pitch : " << this->pitch << " inversed : " << asin(this->forwardAxis.y) << std::endl;

    // double y = this->forwardAxis.y;
    // if (abs(y) < EPSILONE) {
    //     y = EPSILONE;
    // }
    // std::cout << this->forwardAxis << std::endl;
    // std::cout << cos(this->pitch) << " : " << this->forwardAxis.z << std::endl;
    // std::cout << "yaw : " << this->yaw << " inversed : " << asin(cos(this->pitch) / this->forwardAxis.z) << std::endl;

    // this->forwardAxis = glm::inverse(this->rollMatrix) * this->forwardAxis;

    this->recalculate();
}

void Camera::scale(glm::vec3 scale) {
    this->scaleV.x *= scale.x;
    this->scaleV.y *= scale.y;
    this->scaleV.z *= scale.z;
    this->recalculate();
}

void Camera::apply(unsigned int shader) const {
    unsigned int viewLoc = glGetUniformLocation(shader, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(this->globalViewMatrix));
    unsigned int projectionLoc = glGetUniformLocation(shader, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(this->projectionMatrix));
}

void Camera::recalculate() {
    // glm::mat4 trans = glm::mat4(1.0f);
    // trans = trans * this->rotationMatrix;
    // trans = glm::translate(trans, -this->position);
    // trans = glm::scale(trans, this->scaleV);

    // this->viewMatrix = trans;
    this->viewMatrix = this->rollMatrix * glm::lookAt(this->position, this->position - this->forwardAxis, this->up());
    this->viewMatrix = glm::scale(this->viewMatrix, this->scaleV);

    if (this->parent != nullptr) {
        this->updateTransformMatrix(this->parent->getTransformMatrix());
    } else {
        this->updateTransformMatrix(glm::mat4(1.0f));
    }
    // std::cout << upAxis << " : " << this->up() << std::endl;
}

void Camera::updateTransformMatrix(glm::mat4 const parentTransformMatrix) {
    // globalTransform is where the camera actually is in world space.
    // remember that inorder to draw everthing we need to translate everything in word in the inverse of the camera position and rotation
    // so global vewmatrix is simply the inverse of the trasformation matrix. ( so we dont have to calculate it every frame. )
    this->globalTransformMatrix = parentTransformMatrix * glm::inverse(this->viewMatrix);
    this->globalViewMatrix = glm::inverse(this->globalTransformMatrix);
    for (auto node : nodes) {
        node->updateTransformMatrix(this->globalTransformMatrix);
    }
}

void Camera::localTranslate(glm::vec3 translate) {
    // glm::vec4 t = glm::vec4(translate, 1.0f);
    // this->position = this->position + glm::inverse(this->rotationMatrix) * t;
    // auto z = translate.z;
    // auto vz = glm::vec3(0.0f, 0.0f, 0.0f);
    // if (abs(translate.z) > 0.01) {
    //     vz = glm::inverse(glm::lookAt(this->position, this->position - this->forwardAxis, this->up())) * glm::vec3(0.0, 0.0, translate.z);
    // }
    // std::cout << vz << std::endl;
    // translate.z = 0;
    // auto vv = this->rotationMatrix * translate;

    translate = glm::inverse(this->rollMatrix) * translate;
    this->position += translate.x * this->right() + translate.y * this->up() + translate.z * this->forward();
    std::cout << this->position << std::endl;
    this->recalculate();
}

void Camera::globalTranslate(glm::vec3 translate) {
    this->position += translate;
    this->recalculate();
}

void Camera::lookAt(glm::vec3 targetPos) {
    // this->upAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    // this->upAxis = this->rotationMatrix * this->upAxis;
    // this->rollMatrix = glm::mat4(1.0f);
    this->forwardAxis = glm::normalize(this->position - targetPos);

    // yaw is Z->up and x->right
    // angle = tan-1(up/right)
    if (abs(forwardAxis.x) > 0.01f) {
        this->yaw = atan(this->forwardAxis.z / this->forwardAxis.x);
    } else {
        this->yaw = PI / 2;
    }

    float xz = this->forwardAxis.x + this->forwardAxis.z;
    if (abs(xz) > 0.01f) {
        this->pitch = atan(this->forwardAxis.y / xz);
    } else {
        this->pitch = PI / 2;
    }
    std::cout << "ff" << forwardAxis << ": " << this->position << std::endl;
    std::cout << pitch << "," << yaw << ":" << this->forwardAxis << std::endl;

    auto test = glm::vec3(cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch));
    std::cout << "recalculated " << test << std::endl;
    this->updateYawAndPitch();
    // this->recalculate();
}

glm::vec3 Camera::right() {
    return glm::normalize(glm::cross(this->upAxis, this->forwardAxis));
}

glm::vec3 Camera::up() {
    return glm::cross(this->forwardAxis, this->right());
}

glm::vec3 Camera::forward() {
    return this->forwardAxis;
}

glm::vec3 Camera::getPos() {
    return this->position;
}

void Camera::onEngineUpdate(Engine &engine) {
    engine.addCameraCall(*this);
}
} // namespace Engine::Node

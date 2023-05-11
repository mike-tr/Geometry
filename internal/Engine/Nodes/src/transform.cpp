#include "Node/transform.hpp"

namespace Engine {
namespace Node {
Transform::Transform() {
    this->_localPosition = glm::vec3(0.0, 0.0, 0.0);
    this->rotationMatrix = glm::mat4(1.0f);
    this->_localScale = glm::vec3(1.0, 1.0, 1.0);
    this->recalculate();
}

void Transform::scale(glm::vec3 scale) {
    this->_localScale.x *= scale.x;
    this->_localScale.y *= scale.y;
    this->_localScale.z *= scale.z;
    this->recalculate();
}

const glm::vec3 Transform::getLocalPosition() {
    return this->_localPosition;
}

const glm::vec3 Transform::getGlobalPosition() {
    if (parent != nullptr)
        return this->parent->globalTransformMatrix * this->_localPosition;
    return this->_localPosition;
}

void Transform::setGlobalPosition(const glm::vec3 &pos) {
    // Need to be fixed this is local!
    this->_localPosition = pos;
    this->recalculate();
}

void Transform::setLocalPosition(const glm::vec3 &pos) {
    this->_localPosition = pos;
    this->recalculate();
}

void Transform::translate(const glm::vec3 &direction) {
    this->_localPosition = this->_localPosition + direction;
    this->recalculate();
    // this->recalculate();
}

// void Transform::localTranslate(const glm::vec3 &direction) {
//     glm::vec3 moveVector = this->rotationMatrix * direction;
//     // moveVector = glm::normalize(moveVector);
//     // float magnitude = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
//     this->_localPosition = this->_localPosition + moveVector;
//     // this->recalculate();
// }

void Transform::rotate(float degRadians, glm::vec3 axis) {
    this->rotationMatrix = glm::rotate(this->rotationMatrix, degRadians, axis);
    this->recalculate();
}

void Transform::recalculate() {
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, this->_localPosition);
    trans = trans * this->rotationMatrix;
    trans = glm::scale(trans, this->_localScale);

    this->transformationMatrix = trans;
    if (this->parent != nullptr) {
        this->updateTransformMatrix(this->parent->getTransformMatrix());
    } else {
        this->updateTransformMatrix(glm::mat4(1.0f));
    }
}

void Transform::updateTransformMatrix(glm::mat4 const parentTransformMatrix) {
    this->globalTransformMatrix = parentTransformMatrix * this->transformationMatrix;
    for (auto node : nodes) {
        node->updateTransformMatrix(this->globalTransformMatrix);
    }
}

void Transform::lookAt(glm::vec3 target) {
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, this->_localPosition);

    // The commented code changes position!

    // this->rotationMatrix = glm::inverse(trans * glm::lookAt(this->position, target, glm::vec3(0.0f, 1.0f, 0.0f)));
    // this->transformationMatrix = this->rotationMatrix * glm::scale(trans, this->scaleV);
    // if (this->parent != nullptr) {
    //     this->updateTransformMatrix(this->parent->getTransformMatrix());
    // } else {
    //     this->updateTransformMatrix(glm::mat4(1.0f));
    // }

    this->rotationMatrix = glm::inverse(glm::lookAt(this->_localPosition, target, glm::vec3(0.0f, 1.0f, 0.0f)) * trans);
    this->recalculate();
}

const glm::vec3 Transform::forward() {
    return this->rotationMatrix * glm::vec3(0.0f, 0.0f, 1.0f);
}

const glm::vec3 Transform::right() {
    return this->rotationMatrix * glm::vec3(1.0f, 0.0f, 0.0f);
}

const glm::vec3 Transform::up() {
    return this->rotationMatrix * glm::vec3(0.0f, 1.0f, 0.0f);
}

} // namespace Node
} // namespace Engine

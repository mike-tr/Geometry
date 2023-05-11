#include "gameObject.hpp"
#include "Node/camera.hpp"

GameObject::GameObject(unsigned int model, unsigned int numVertices, bool indexed, const Material &material) : material(material) {
    this->object = model;
    this->numVertices = numVertices;
    this->position = glm::vec3(0.0, 0.0, 0.0);
    this->rotationMatrix = glm::mat4(1.0f);
    this->scaleV = glm::vec3(1.0, 1.0, 1.0);
    this->indexed = indexed;
    this->recalculate();
}

void GameObject::localScale(glm::vec3 scale) {
    this->scaleV.x *= scale.x;
    this->scaleV.y *= scale.y;
    this->scaleV.z *= scale.z;
    this->recalculate();
}

void GameObject::draw(const Camera &camera) {
    this->material.use();
    unsigned int shaderID = this->material.shader.ID;
    camera.apply(shaderID);
    unsigned int transformLoc = glGetUniformLocation(shaderID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(this->transformationMatrix));

    glBindVertexArray(this->object);
    if (indexed) {
        glDrawElements(GL_TRIANGLES, this->numVertices, GL_UNSIGNED_INT, 0);
        return;
    }
    glDrawArrays(GL_TRIANGLES, 0, this->numVertices);
}

void GameObject::draw2(const Engine::Node::Camera &camera) {
    this->material.use();
    unsigned int shaderID = this->material.shader.ID;
    camera.apply(shaderID);
    unsigned int transformLoc = glGetUniformLocation(shaderID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(this->transformationMatrix));

    glBindVertexArray(this->object);
    if (indexed) {
        glDrawElements(GL_TRIANGLES, this->numVertices, GL_UNSIGNED_INT, 0);
        return;
    }
    glDrawArrays(GL_TRIANGLES, 0, this->numVertices);
}

glm::vec3 GameObject::getPosition() {
    return this->position;
}

void GameObject::setPosition(glm::vec3 pos) {
    this->position = pos;
}

void GameObject::translate(glm::vec3 direction) {
    this->position = this->position + direction;
}

void GameObject::localTranslate(glm::vec3 direction) {
    glm::vec4 t = glm::vec4(direction, 1.0f);
    this->position = this->position + glm::inverse(this->rotationMatrix) * t;
    this->recalculate();
}

void GameObject::rotate(float degRadians, glm::vec3 axis) {
    this->rotationMatrix = glm::rotate(this->rotationMatrix, degRadians, axis);
    this->recalculate();
}

void GameObject::recalculate() {
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, this->position);
    trans = trans * this->rotationMatrix;
    trans = glm::scale(trans, this->scaleV);

    this->transformationMatrix = trans;
}

Material &GameObject::getMaterial() {
    return material;
}

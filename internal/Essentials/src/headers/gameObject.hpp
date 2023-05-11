#pragma once
#include "Node/nodes.fwd.hpp"
#include "camera.hpp"
#include "material.hpp"

class GameObject {
private:
    unsigned int object;
    unsigned int numVertices;
    Material material;
    glm::mat4 rotationMatrix;
    glm::vec3 scaleV;
    glm::mat4 transformationMatrix;
    glm::vec3 position;
    bool indexed;

public:
    GameObject(unsigned int model, unsigned int numVertices, bool indexed, const Material &material);
    void draw(const Camera &camera);
    void draw2(const Engine::Node::Camera &camera);
    glm::vec3 getPosition();
    Material &getMaterial();
    void setPosition(glm::vec3 pos);
    void translate(glm::vec3 direction);
    void localTranslate(glm::vec3 direction);
    void localScale(glm::vec3 scale);
    void rotate(float degRadians, glm::vec3 axis);
    void recalculate();
};
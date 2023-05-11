#include "Componenet/meshRenderer.hpp"
#include "Node/camera.hpp"
#include "engine.hpp"
#include "functions.hpp"
// #include "functions.hpp"
#include "Node/node.hpp"
//#include "transformNode.hpp"

namespace Engine {
namespace Components {
MeshRenderer::MeshRenderer(Node::Node &parent, unsigned int model, unsigned int numVertices, bool indexed, const Material &material) : material(material), Componenet(&parent) {
    this->object = model;
    this->numVertices = numVertices;
    this->indexed = indexed;
}

void MeshRenderer::draw(const Node::Camera &camera) {
    this->material.use();
    unsigned int shaderID = this->material.shader.ID;
    camera.apply(shaderID);
    unsigned int transformLoc = glGetUniformLocation(shaderID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(this->parent->getTransformMatrix()));
    //  this->transform.getTransformMatrix();
    // transform.applyTransformation(transformLoc);

    glBindVertexArray(this->object);
    if (indexed) {
        glDrawElements(GL_TRIANGLES, this->numVertices, GL_UNSIGNED_INT, 0);
        return;
    }
    glDrawArrays(GL_TRIANGLES, 0, this->numVertices);
}

MeshRenderer::~MeshRenderer() {
}

void MeshRenderer::update() {
}

void MeshRenderer::onEngineUpdate(Engine &engine) {
    // std::cout << "rendering some mesh" << std::endl;
    engine.addDrawCall(*this);
}

Material &MeshRenderer::getMaterial() {
    return material;
}

} // namespace Components
} // namespace Engine
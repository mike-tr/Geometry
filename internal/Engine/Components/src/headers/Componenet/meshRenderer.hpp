#pragma once
#define FORWARDS
#include "Node/nodes.fwd.hpp"
#include "component.hpp"
// #include "engine.hpp"
#include "material.hpp"
//#include "node.hpp"
//#include "cameraNode.hpp"
//#include "transformNode.hpp"

namespace Engine {
class Engine;
namespace Components {
class MeshRenderer final : public Componenet {
private:
    /* data */
    Material material;
    // Node::Transform &transform;
    unsigned int object;
    unsigned int numVertices;
    bool indexed;

protected:
    void onEngineUpdate(Engine &engine) override;

public:
    void draw(const Node::Camera &camera);
    MeshRenderer(Node::Node &parent, unsigned int model, unsigned int numVertices, bool indexed, const Material &material);
    ~MeshRenderer();
    Material &getMaterial();
    void update() override;
};
} // namespace Components
} // namespace Engine

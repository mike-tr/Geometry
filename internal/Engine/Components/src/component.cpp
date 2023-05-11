#include "Componenet/component.hpp"
#include "Node/node.hpp"

namespace Engine::Components {

void Componenet::onEngineUpdate(Engine &engine) {
}

void Componenet::engineUpdate(Engine &engine) {
    this->update();
    this->onEngineUpdate(engine);
}

void Componenet::update() {
}

Componenet::Componenet(Node::Node *parent) {
    parent->addComponenet(this);
    this->parent = parent;
}

} // namespace Engine::Components
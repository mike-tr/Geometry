#pragma once
//#include "engine.hpp"
#include "Node/nodes.fwd.hpp"

namespace Engine {
class Engine;

namespace Components {
class Componenet {
protected:
    friend Engine;
    friend Node::Node;
    Node::Node *parent;
    virtual void onEngineUpdate(Engine &engine);

    void engineUpdate(Engine &engine);

public:
    Componenet(Node::Node *parent);
    virtual void update();
};
} // namespace Components
} // namespace Engine
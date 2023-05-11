
#pragma once
#include "Componenet/componenets.fwd.hpp"
#include "Node/nodes.fwd.hpp"
#include "inputHandler.hpp"

#include "stb_image.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stack>
#include <vector>

// typedef class InputHandler;

namespace Engine {
class Engine {
private:
    friend class InputHandler;
    friend class Components::MeshRenderer;
    friend class Node::Camera;
    static Engine *instance;
    InputHandler input;

    std::stack<Components::MeshRenderer *> drawcalls;
    std::vector<Node::Camera *> cameraCalls;
    //  std::stack<Cam>

    void addDrawCall(Components::MeshRenderer &mesh);
    void addCameraCall(Node::Camera &camera);
    void drawcall();
    /* data */
public:
    GLFWwindow *window;
    Node::Scene *scene;
    Engine(int width, int hight, const char *windowName);
    ~Engine();
    static void exit();
    // Call each frame.
    void update();
    // void addNode(Node::Node *node);
    void setScene(Node::Scene *scene);
    bool running();

    GLFWwindow &getWindow();

    static Engine &getEngine();
};
} // namespace Engine
#include "engine.hpp"
//#include "cameraNode.hpp"
#include "Componenet/meshRenderer.hpp"
// #include "node.hpp"
#include "Node/scene.hpp"
#include "etime.hpp"
#include <iostream>

namespace Engine {

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

Engine *Engine::instance = nullptr;

Engine::Engine(int width, int hight, const char *windowName) : input(*this) {
    std::cout << (instance == nullptr) << std::endl;
    if (instance != nullptr) {
        std::cerr << "Can have only 1 instance of engine running!" << std::endl;
    }
    instance = this;

    if (!glfwInit())
        return;

    this->window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    input.init();

    // cout << glGetString(GL_VERSION) << endl;
}

void Engine::exit() {
    glfwSetWindowShouldClose(instance->window, true);
}

void Engine::update() {
    ETime::update();
    input.update_keys();
    scene->engineUpdate(*this);
    this->drawcall();
    // for (auto node : nodes) {
    //     //(*it)->engineUpdate(*this);
    // }
}

void Engine::setScene(Node::Scene *scene) {
    this->scene = scene;
}

void Engine::addCameraCall(Node::Camera &camera) {
    cameraCalls.push_back(&camera);
}

void Engine::addDrawCall(Components::MeshRenderer &mesh) {
    drawcalls.push(&mesh);
}

void Engine::drawcall() {
    while (!drawcalls.empty()) {
        Components::MeshRenderer *renderer = drawcalls.top();
        for (auto camera : cameraCalls) {
            renderer->draw(*camera);
        }
        drawcalls.pop();
    }
    cameraCalls.clear();
}

Engine::~Engine() {
    InputHandler::exit();
    glfwTerminate();
}

Engine &Engine::getEngine() {
    return (*instance);
}

GLFWwindow &Engine::getWindow() {
    return (*this->window);
}

bool Engine::running() {
    return !glfwWindowShouldClose(window);
}
} // namespace Engine

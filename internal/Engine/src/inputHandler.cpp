#include "engine.hpp"
#include <queue>

namespace Engine {
#define KEY_DOWN 1
#define KEY_PRESSED 2
#define KEY_UP 3
#define KEY_RELEASED 0
InputHandler *instance;

void InputHandler::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    switch (action) {
    case GLFW_PRESS:
        instance->keyPendingDown.push(key);
        break;
    case GLFW_RELEASE:
        instance->keyPendingUp.push(key);
        break;
    default:
        break;
    }
}

void InputHandler::update_keys() {
    auto temp = std::stack<int>();

    while (!keyPendingDown.empty()) {
        int next = keyPendingDown.top();
        keyPendingDown.pop();
        if (keyStates[next] != KEY_DOWN) {
            keyStates[next] = KEY_DOWN;
            temp.push(next);
        } else {
            keyStates[next] = KEY_PRESSED;
        }
    }
    while (!temp.empty()) {
        keyPendingDown.push(temp.top());
        temp.pop();
    }

    while (!keyPendingUp.empty()) {
        int next = keyPendingUp.top();
        keyPendingUp.pop();
        if (keyStates[next] != KEY_UP) {
            keyStates[next] = KEY_UP;
            temp.push(next);
        } else {
            keyStates[next] = KEY_RELEASED;
        }
    }

    while (!temp.empty()) {
        keyPendingUp.push(temp.top());
        temp.pop();
    }
}

InputHandler::InputHandler(Engine &engine) : engine(engine) {
    if (instance != NULL) {
        std::cerr << "InputHandler already defined!" << std::endl;
        return;
    }
    keyStates.clear();
    keyMaps.clear();
    keyPendingDown = std::stack<int>();
    keyPendingUp = std::stack<int>();
    instance = this;
}

void InputHandler::init() {
    glfwSetKeyCallback(engine.window, InputHandler::key_callback);
    glfwSetCursorPosCallback(engine.window, InputHandler::mouse_callback);
}

bool InputHandler::getKey(int key) {
    return instance->keyStates[key] > 0;
}

bool InputHandler::getKeyDown(int key) {
    return instance->keyStates[key] == KEY_DOWN;
}

bool InputHandler::getKeyUp(int key) {
    return instance->keyStates[key] == KEY_UP;
}

bool InputHandler::getKeyDown(std::string key) {
    return getKeyDown(instance->keyMaps[key]);
}

bool InputHandler::getKey(std::string key) {
    return getKey(instance->keyMaps[key]);
}

bool InputHandler::getKeyUp(std::string key) {
    return getKeyUp(instance->keyMaps[key]);
}

void InputHandler::bindKey(std::string keyName, int key) {
    instance->keyMaps[keyName] = key;
}

void InputHandler::disableCursor(bool state) {
    glfwSetInputMode(instance->engine.window, GLFW_CURSOR, state ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void InputHandler::mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    instance->pos.x = xpos;
    instance->pos.y = ypos;
}

const mousePos &InputHandler::getMousePosition() {
    return instance->pos;
}

void InputHandler::exit() {
    glfwSetWindowShouldClose(instance->engine.window, true);
}

} // namespace Engine
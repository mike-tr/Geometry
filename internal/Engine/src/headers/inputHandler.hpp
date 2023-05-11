#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include <stack>
#include <string>

namespace Engine {
typedef class Engine Engine;

typedef struct mousePos {
    double x;
    double y;
} mousePos;

class InputHandler {
    friend class Engine;
    Engine &engine;
    std::map<int, int> keyStates;
    std::map<std::string, int> keyMaps;
    std::stack<int> keyPendingDown;
    std::stack<int> keyPendingUp;
    mousePos pos;

    InputHandler(Engine &engine);
    void update_keys();
    void init();

    // static InputHandler &init(Engine *engine);

public:
    // Returns if key is pressed
    static bool getKey(int key);
    //  Returns true if during this frame the key was pressed
    static bool getKeyDown(int key);
    // Returns true if during this frame the key was realesed
    static bool getKeyUp(int key);
    static bool getKeyDown(std::string key);
    static bool getKey(std::string key);
    static bool getKeyUp(std::string key);
    static void bindKey(std::string keyName, int key);
    static const mousePos &getMousePosition();

    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow *window, double xpos, double ypos);

    // void (*key_callback)

    static void disableCursor(bool state);
    static void exit();
}; // namespace InputHandler
} // namespace Engine
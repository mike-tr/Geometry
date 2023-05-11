// //#include "simpleCamera.hpp"

// #include "simpleCamera.hpp"
// #include "engine.hpp"
// #include "inputHandler.hpp"

// namespace Engine {

// SimpleCamera::SimpleCamera(glm::vec3 startPos, glm::vec3 lookPos, Camera::Mode projectionMode) : mainCamera(startPos, projectionMode) {
//     this->mainCamera.lookAt(lookPos);
// }

// void SimpleCamera::update() {
//     if (InputHandler::getKeyDown(GLFW_KEY_ESCAPE)) {
//         // glfwSetWindowShouldClose(window, true);
//         std::cout << "camera : escape was pressed!" << std::endl;
//     }
// }
// } // namespace Engine
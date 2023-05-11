// #define GLEW_STATIC
#include "Componenet/meshRenderer.hpp"
#include "Node/camera.hpp"
#include "Node/scene.hpp"
#include "Node/transform.hpp"
// #include "camera.hpp"
#include "engine.hpp"
#include "etime.hpp"
#include "gameObject.hpp"
#include "math.h"
#include "shaders.hpp"
#include "simpleCamera.hpp"
#include <iomanip>

using namespace std;
using namespace Engine;

int old();
int start() {
    cout << "creating scene!" << endl;
    Node::Scene scene = Node::Scene();

    cout << "creating engine" << endl;
    Engine::Engine engine = Engine::Engine(800, 600, "test e3ngine");

    cout << "done!" << endl;

    engine.setScene(&scene);

    cout << "added scene" << endl;
    Node::Transform camNode = Node::Transform();

    Node::Camera camera = Node::Camera(glm::vec3(0.0f, 0.0f, 0.0f), Node::Camera::perspective);
    camNode.addNode(&camera);
    // camNode.setGlobalPosition(glm::vec3(0.0f, 0.0f, 10.0f));
    camNode.position = glm::vec3(0.0f, 0.0f, 10.0f);

    scene.addNode(&camNode);

    cout << "added camera" << endl;

    unsigned int brickTex = create_texture("assets/images/brick.jpg");
    unsigned int rabbitTex = create_texture("assets/images/rabbit.jpg");
    unsigned int normalTex = create_texture("assets/images/normalmap.jpg");
    unsigned int skeleTex = create_texture("assets/images/skeletor.jpg");

    Shader mainShader = Shader("assets/shaders/shader.vs", "assets/shaders/shader.fs");
    // mainShader.use();
    // mainShader.setInt("blendTex", 1);
    // mainShader.setInt("normalTex", 2);
    Shader normalShader = Shader("assets/shaders/shader.vs", "assets/shaders/normal.fs");
    Shader GroundShader = Shader("assets/shaders/shader.vs", "assets/shaders/ground.fs");
    Shader simplexShader = Shader("assets/shaders/simplex.vs", "assets/shaders/simplex.fs");

    Material mainMaterial = Material(mainShader);
    Material groundMaterial = Material(GroundShader);
    Material normalMaterial = Material(normalShader);
    Material simplexMaterial = Material(simplexShader);

    Node::Transform obj1 = Node::Transform();
    unsigned int cubeb = create_cube();

    // Components::MeshRenderer cube = Components::MeshRenderer(obj1, cubeb, 36, true, normalMaterial);
    unsigned int simplex1 = create_simplex();
    Components::MeshRenderer cube = Components::MeshRenderer(obj1, simplex1, 6, true, simplexMaterial);
    // obj1.setGlobalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    obj1.position = glm::vec3(0.0f, 0.0f, 0.0f);
    obj1.localScale = glm::vec3(1.0f, 1.0f, 2.0f);

    cube.getMaterial().setFloat("time", 1);
    cube.getMaterial().setFloat("offset", 1);
    cube.getMaterial().setTexture("mainTex", brickTex);

    // obj1.addComponenet(&cube);

    Node::Transform obj2 = Node::Transform();
    Components::MeshRenderer cube2 = Components::MeshRenderer(obj2, cubeb, 36, true, normalMaterial);
    // obj2.setGlobalPosition(glm::vec3(2.0f, -3.0f, 0.0f));
    obj2.position = glm::vec3(2.0f, -3.0f, 0.0f);
    obj2.scale(glm::vec3(1.0f, 1.0f, 2.0f));
    // obj2.localScale = glm::vec3(1.0f, 1.0f, 2.0f);

    cube2.getMaterial().setFloat("time", 1);
    cube2.getMaterial().setFloat("offset", 1);
    cube2.getMaterial().setTexture("blendTex", rabbitTex);
    cube2.getMaterial().setTexture("mainTex", skeleTex);

    // obj2.addComponenet(&cube2);

    Node::Transform obj3 = Node::Transform();
    Components::MeshRenderer cube3 = Components::MeshRenderer(obj3, cubeb, 36, true, normalMaterial);
    obj3.position = glm::vec3(1.5f, 0.0f, -1.0f);
    // obj3.setGlobalPosition(glm::vec3(1.5f, 0.0f, -1.0f));
    //  obj3._localScale(glm::vec3(1.0f, 1.0f, 1.0f));

    cube3.getMaterial().setFloat("time", 1);
    cube3.getMaterial().setFloat("offset", 1);
    cube3.getMaterial().setTexture("mainTex", brickTex);

    // obj3.addComponenet(&cube3);

    cout << "adding objects" << endl;

    Node::Transform obj4 = Node::Transform();
    Components::MeshRenderer cube4 = Components::MeshRenderer(obj4, cubeb, 36, true, normalMaterial);
    obj4.position = glm::vec3(-2.0f, -3.0f, 0.0f);
    // obj4.setGlobalPosition(glm::vec3(-2.0f, -3.0f, 0.0f));
    //  obj4._localScale(glm::vec3(1.0f, 1.0f, 1.0f));

    cube4.getMaterial().setFloat("time", 1);
    cube4.getMaterial().setFloat("offset", 1);
    cube4.getMaterial().setTexture("blendTex", rabbitTex);
    cube4.getMaterial().setTexture("mainTex", skeleTex);

    // Components::MeshRenderer cube4 = Components::MeshRenderer(scene, cubeb, 36, true, normalMaterial);

    scene.addNode(&obj1);
    obj1.addNode(&obj3);
    scene.addNode(&obj2);
    scene.addNode(&obj4);
    // // scene.addNode(&obj3);
    // obj1.addNode(&obj3);

    // camera.lookAt(obj1.getPosition());

    camNode.lookAt(obj4.position);
    camNode.position = glm::vec3(0.0f, 0.0f, 10.0f);
    // camNode.setGlobalPosition(glm::vec3(0.0f, 0.0f, 10.0f));

    obj3.rotate(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    cout << "starting while loop" << endl;

    int i = 0;
    glEnable(GL_DEPTH_TEST);

    float deg = glm::radians(90.0f);
    while (engine.running()) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        engine.update();

        obj1.rotate(0.01f, glm::vec3(0.0f, 1.0f, 0.0f));

        auto t = sin(ETime::timePassed * 5);

        camNode.position += camNode.forward() * t * 0.05f;
        camNode.lookAt(obj3.position);

        if (((int)(ETime::timePassed * 10)) % 10 == 0) {
            cout << "moved! " << ETime::timePassed << endl;
            obj3.position = glm::vec3(1.5f, 0.0f, -1.0f);
        }

        if (InputHandler::getKey(GLFW_KEY_W)) {
            cout << "pressed w" << endl;
        }
        if (InputHandler::getKeyUp(GLFW_KEY_W)) {
            cout << "w realesed" << endl;
        }
        obj2.lookAt(obj3.position);

        glfwSwapBuffers(engine.window);
        glfwPollEvents();
    }

    return 0;
}

int main(void) {
    // return old();
    return start();
    //     return 0;
}

// int old() {
//     GLFWwindow *window;

//     /* Initialize the library */
//     if (!glfwInit())
//         return -1;

//     /* Create a windowed mode window and its OpenGL context */
//     window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
//     if (!window) {
//         glfwTerminate();
//         return -1;
//     }

//     /* Make the window's context current */
//     glfwMakeContextCurrent(window);

//     GLenum err = glewInit();
//     if (err != GLEW_OK) {
//         fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
//     }

//     glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//     glfwSetKeyCallback(window, key_callback);
//     // glfwSetKeyCallback(window, camera.key_callback);

//     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

//     glfwSetCursorPosCallback(window, mouse_callback);

//     cout << glGetString(GL_VERSION) << endl;

//     // Engine::SimpleCamera camera = Engine::SimpleCamera(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), Camera::Mode::perspective);

//     Shader ourShader = Shader("assets/shaders/shader.vs", "assets/shaders/shader.fs");
//     Shader sphereShader = Shader("assets/shaders/shader.vs", "assets/shaders/shader.fs");
//     unsigned int mainTex = create_texture("assets/images/brick.jpg");
//     unsigned int blendTex = create_texture("assets/images/rabbit.jpg");
//     unsigned int normalTex = create_texture("assets/images/normalmap.jpg");
//     unsigned int mainTex2 = create_texture("assets/images/skeletor.jpg");
//     // Set blend tex as uint 1.
//     ourShader.use();
//     ourShader.setInt("blendTex", 1);
//     ourShader.setInt("normalTex", 2);

//     Shader GroundShader = Shader("assets/shaders/shader.vs", "assets/shaders/ground.fs");
//     // GroundShader.setVec4("ObjColor", 0.0f, 1.0f, 0.0f, 1.0f);

//     // unsigned int shaderProgram = load_shaders();
//     // glUseProgram(shaderProgram);
//     std::cout << std::fixed;
//     std::cout << std::setprecision(2);

//     glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
//     // glm::mat4 trans = glm::mat4(1.0f);
//     // // trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
//     // trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
//     // trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

//     // vec = trans * vec;
//     vec = vec + vec;
//     vec.y += 1;
//     std::cout << "( " << vec.x << ", " << vec.y << ", " << vec.z << " )" << std::endl;

//     unsigned int cube = create_cube_bad();
//     unsigned int cubeb = create_cube();

//     Material mat = Material(ourShader);
//     GameObject obj[] = {GameObject(cubeb, 36, true, mat), GameObject(cubeb, 36, true, mat), GameObject(cubeb, 36, true, mat)};
//     Material groundMaterial = Material(GroundShader);
//     GameObject ground = GameObject(cubeb, 36, true, groundMaterial);
//     ground.setPosition(glm::vec3(0.0f, -5.0f, 0.0f));
//     ground.localScale(glm::vec3(1000.0f, 1.0f, 1000.0f));
//     ground.getMaterial().setFv4("ObjColor", 0.0f, 1.0f, 0.0f, 1.0f);
//     ground.getMaterial().setFloat("TestVal", 1.0f);

//     // auto testobj = Engine::Components::Mesh(cubeb, 36, true, mat);

//     GameObject *obj2[8];
//     auto distance = 10.0f;
//     for (size_t i = 0; i < 8; i++) {
//         obj2[i] = new GameObject(cubeb, 36, true, mat);
//         obj2[i]->setPosition(glm::vec3(cos(i * 3.14 / 4), 0, sin(i * 3.14 / 4)) * distance);
//         obj2[i]->localScale(glm::vec3(1.0f, 1.0f, 1.0f));
//         // obj2[i].setPosition(glm::vec3(cos(i * PI)))
//     }

//     obj[0].setPosition(glm::vec3(0.5f, -0.5f, -2.0f));
//     obj[0].localScale(glm::vec3(1.0f, 1.0f, 1.0f) * 0.5f);
//     obj[0].rotate(0.1, glm::vec3(0.0, 0.0, 1.0));

//     obj[1].setPosition(glm::vec3(0.5f, 0.5f, -1.0f));
//     obj[1].localScale(glm::vec3(1.0f, 1.0f, 1.0f) * 0.33f);
//     obj[1].rotate(-0.1, glm::vec3(0.0, 0.0, 1.0));

//     obj[2].setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
//     obj[2].localScale(glm::vec3(1.0f, 1.0f, 1.0f) * 0.1f);

//     for (unsigned int i = 0; i < 3; i++) {
//         obj[i].getMaterial().setFloat("time", 1);
//         obj[i].getMaterial().setFloat("offset", 1);
//         obj[i].getMaterial().setTexture("mainTex", mainTex);
//         obj[i].getMaterial().setTexture("blendTex", blendTex);
//         obj[i].getMaterial().setTexture("normalTex", normalTex);
//     }

//     obj[2].getMaterial().setTexture("blendTex", mainTex2);
//     obj[1].getMaterial().setTexture("blendTex", normalTex);

//     glEnable(GL_DEPTH_TEST);
//     cout << "start while" << endl;

//     // Camera::mainCamera = Camera(glm::vec3(0.0, 0.0, 3.0), Camera::Mode::perspective);

//     GameObject testobj = GameObject(cubeb, 36, true, mat);
//     testobj.setPosition(glm::vec3(0.0f, -1.0f, -1.0f));
//     testobj.localScale(glm::vec3(1.0f, 1.0f, 1.0f));
//     testobj.getMaterial().setFloat("time", 1);
//     testobj.getMaterial().setFloat("offset", 1);
//     testobj.getMaterial().setInt("blendTex", mainTex);
//     testobj.getMaterial().setInt("normalTex", blendTex);
//     testobj.getMaterial().setTexture("normalTex", normalTex);
//     testobj.getMaterial().setTexture("blendTex", blendTex);

//     Camera::mainCamera.lookAt(testobj.getPosition());

//     ETime::update();
//     /* Loop until the user closes the window */
//     while (!glfwWindowShouldClose(window)) {
//         // ETime::update();
//         //  processInput(window);
//         /* Render here */
//         glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//         for (int i = 0; i < 3; i++) {
//             glm::vec3 t = glm::vec3(0.001 * (1 - i), 0.001 * (2 - i), 0.0);
//             if (i != 2) {
//                 obj[i].localTranslate(t);
//             }
//             glm::vec3 axis = glm::vec3(0.0, 0.0, 0.0);
//             if (i == 0) {
//                 axis.x = sqrt(2);
//                 axis.y = sqrt(2);
//             } else if (i == 1) {
//                 axis.y = 1;
//             } else {
//                 axis.x = -sqrt(2);
//                 axis.z = sqrt(2);
//             }
//             obj[i].rotate(0.005 * (i + 1), axis);
//             // obj[i].draw(Camera::mainCamera);
//         }
//         // obj[0].draw(Camera::mainCamera);
//         ground.draw(Camera::mainCamera);
//         testobj.draw(Camera::mainCamera);

//         for (size_t i = 0; i < 8; i++) {
//             obj2[i]->draw(Camera::mainCamera);
//         }
//         // obj[1].draw(Camera::mainCamera);
//         //  obj[2].draw(Camera::mainCamera);
//         /* Swap front and back buffers */
//         cout << "what?" << endl;
//         glfwSwapBuffers(window);

//         /* Poll for and process events */
//         glfwPollEvents();
//     }

//     // optional: de-allocate all resources once they've outlived their purpose:
//     // ------------------------------------------------------------------------
//     // glDeleteVertexArrays(1, &VAO);
//     // glDeleteBuffers(1, &VBO);
//     // glDeleteProgram(shaderProgram);

//     glfwTerminate();
//     return 0;
// }

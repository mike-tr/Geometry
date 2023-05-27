// #define GLEW_STATIC
#include "Componenet/meshRenderer.hpp"
#include "Node/camera.hpp"
#include "Node/scene.hpp"
#include "Node/transform.hpp"
// #include "camera.hpp"
#include "convexHull3D.hpp"
#include "engine.hpp"
#include "etime.hpp"
#include "gameObject.hpp"
#include "math.h"
#include "shaders.hpp"
#include "simpleCamera.hpp"
#include <iomanip>

using namespace std;
using namespace Engine;

void DrawCircle(float cx, float cy, float r, int num_segments) {
    float theta = 2 * 3.1415926 / float(num_segments);
    float tangetial_factor = tanf(theta); // calculate the tangential factor

    float radial_factor = cosf(theta); // calculate the radial factor

    float x = r; // we start at angle = 0

    float y = 0;

    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < num_segments; ii++) {
        glVertex2f(x + cx, y + cy); // output vertex

        // calculate the tangential vector
        // remember, the radial vector is (x, y)
        // to get the tangential vector we flip those coordinates and negate one of them

        float tx = -y;
        float ty = x;

        // add the tangential vector

        x += tx * tangetial_factor;
        y += ty * tangetial_factor;

        // correct using the radial factor

        x *= radial_factor;
        y *= radial_factor;
    }
    glEnd();
}

int old();
int default_scene();
int simplex_scene() {
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

    // unsigned int brickTex = create_texture("assets/images/brick.jpg");
    // unsigned int rabbitTex = create_texture("assets/images/rabbit.jpg");
    // unsigned int normalTex = create_texture("assets/images/normalmap.jpg");
    // unsigned int skeleTex = create_texture("assets/images/skeletor.jpg");

    // Shader mainShader = Shader("assets/shaders/shader.vs", "assets/shaders/shader.fs");
    // mainShader.use();
    // mainShader.setInt("blendTex", 1);
    // mainShader.setInt("normalTex", 2);
    // Shader normalShader = Shader("assets/shaders/shader.vs", "assets/shaders/normal.fs");
    // Shader GroundShader = Shader("assets/shaders/shader.vs", "assets/shaders/ground.fs");
    Shader simplexShader = Shader("assets/shaders/simplex.vs", "assets/shaders/simplex.fs");
    Shader circleShader = Shader("assets/shaders/circle.vs", "assets/shaders/circle.fs");

    // Material mainMaterial = Material(mainShader);
    // Material groundMaterial = Material(GroundShader);
    // Material normalMaterial = Material(normalShader);
    Material simplexMaterial = Material(simplexShader);
    Material circleMaterial = Material(circleShader);

    Node::Transform obj1 = Node::Transform();
    Node::Transform circle = Node::Transform();
    // Components::MeshRenderer cube = Components::MeshRenderer(obj1, cubeb, 36, true, normalMaterial);

    unsigned int simplex1 = create_simplex();

    cout << "Enter input filename with path ('default'/'d' for 'assets/files/input.txt'):" << endl;
    std::string file_name;
    cin >> file_name;
    if (file_name.compare("default") == 0 || file_name.compare("d") == 0) {
        file_name = "assets/files/input.txt";
    }

    cout << "input file target is " << file_name << endl;

    GeometryEx2::ConvexHull3D convexHull(file_name);
    convexHull.start(-1);
    convexHull.createMeshData();
    cout << "created mesh" << endl;

    int numVertices = convexHull.numVectors * 3;
    float *verticesSimplex = new float[numVertices];

    for (size_t i = 0; i < numVertices; i++) {
        verticesSimplex[i] = convexHull.vertices[i] / 100.0f - 1;

        // cout << verticesSimplex[i] << " ";
    }
    // cout << endl;

    // cout << convexHull.numberOfTriangles * 3 * sizeof(int) << endl;
    // unsigned int simplex2 = create_mesh_colored(vertices, colors, 12, indices, sizeof(indices));
    // Components::MeshRenderer cube = Components::MeshRenderer(obj1, simplex2, 9, true, simplexMaterial);
    int triangleArrSize = convexHull.numberOfTriangles * 3 * sizeof(int);
    unsigned int simplex2 = create_mesh_colored(verticesSimplex, convexHull.colors, numVertices, convexHull.triangles, triangleArrSize);
    Components::MeshRenderer cube = Components::MeshRenderer(obj1, simplex2, convexHull.numberOfTriangles * 3, true, simplexMaterial);
    //  obj1.setGlobalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    obj1.position = glm::vec3(0.0f, 0.0f, 0.0f);
    obj1.localScale = glm::vec3(1.0f, 1.0f, 1.0f);

    cube.getMaterial().setFloat("time", 1);
    cube.getMaterial().setFloat("offset", 1);
    // cube.getMaterial().setTexture("mainTex", brickTex);

    unsigned int cubeb = create_cube();
    Components::MeshRenderer nextPoint = Components::MeshRenderer(circle, cubeb, 36, true, circleMaterial);

    float pn[3];
    convexHull.getNextPoint(pn);
    // cout << pn[0] / 200.0f << " " << pn[1] / 100.0f << " " << pn[2] / 100.0f << endl;
    circle.localPosition = glm::vec3(pn[0] / 100.0f - 1, pn[1] / 100.0f - 1, pn[2] / 100.0f - 1);
    circle.localScale = glm::vec3(0.05f, 0.2f, 0.05f);

    auto pos = (glm::vec3)circle.position;
    nextPoint.getMaterial().setFv4("center", 1, 1, 1, 1);

    // cout << "binding objects" << endl;

    // Components::MeshRenderer cube4 = Components::MeshRenderer(scene, cubeb, 36, true, normalMaterial);

    // obj2.localScale = glm::vec3(1.0f, 1.0f, 2.0f);

    scene.addNode(&obj1);
    // scene.addNode(&circle);
    obj1.addNode(&circle);
    // scene.addNode(&obj2);
    // scene.addNode(&obj4);
    // // scene.addNode(&obj3);
    // obj1.addNode(&obj3);

    // vector<Node::Transform *> cubes;
    // vector<Components::MeshRenderer *> meshes;

    // cout << "adding visual points" << endl;
    // for (size_t i = 0; i < 6; i++) {
    //     cubes.push_back(new Node::Transform());
    //     meshes.push_back(new Components::MeshRenderer(*cubes[i], cubeb, 36, true, circleMaterial));
    //     meshes[i]->getMaterial().setFloat("time", 1);
    //     meshes[i]->getMaterial().setFloat("offset", 1);
    //     // meshes[i]->getMaterial().setFv4("center", convexHull.colors[i * 3 + 0], convexHull.colors[i * 3 + 1], convexHull.colors[i * 3 + 2], 1);

    //     convexHull.getPoint(pn, i);
    //     cubes[i]->localPosition = glm::vec3(pn[0] / 100.0f - 1, pn[1] / 100.0f - 1, pn[2] / 100.0f - 1);
    //     cubes[i]->localScale = glm::vec3(0.1f, 0.1f, 0.1f);
    //     obj1.addNode(cubes[i]);
    // }
    // cout << "done \n"
    //      << endl;

    // Node::Transform c1 = Node::Transform();
    // Node::Transform &c1 = *cubes[0];
    // Material nm = Material(circleShader);
    // Components::MeshRenderer p1 = Components::MeshRenderer(c1, cubeb, 36, true, nm);

    // convexHull.getPoint(pn, 1);
    // c1.localPosition = glm::vec3(pn[0] / 200.0f, pn[1] / 200.0f, pn[2] / 200.0f);
    // c1.localScale = glm::vec3(0.1f, 0.1f, 0.1f);
    // obj1.addNode(&c1);

    // camera.lookAt(obj1.getPosition());

    camNode.lookAt(obj1.position);
    camNode.position = glm::vec3(0.0f, 0.0f, 10.0f);
    // camNode.setGlobalPosition(glm::vec3(0.0f, 0.0f, 10.0f));

    // obj3.rotate(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    cout << "starting while loop" << endl;

    int i = 0;
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float deg = glm::radians(90.0f);

    float Speed = 5.0f;
    bool hide_mouse = true;
    InputHandler::disableCursor(hide_mouse);
    auto prevPos = InputHandler::getMousePosition();

    bool line_mode = false;
    cout << "--------------------"
         << "\n press R to view in wireframe mode\n Press N to add next point\n " << endl;
    cout << "You can move with mouse\n Press Escape to enable cursor\n Press A,D,W,S to move around\n--------------------" << endl;
    while (engine.running()) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        engine.update();

        obj1.rotate(0.02f, glm::vec3(0.0f, 1.0f, 0.0f));

        auto t = sin(ETime::timePassed * 5);

        // camNode.position += camNode.forward() * t * 0.05f;
        // camNode.lookAt(obj1.position);

        // if (((int)(ETime::timePassed * 10)) % 10 == 0) {
        //     // cout << "moved! " << ETime::timePassed << endl;
        //     obj3.position = glm::vec3(1.5f, 0.0f, -1.0f);
        // }

        auto currentPos = InputHandler::getMousePosition();
        double offsetX = prevPos.x - currentPos.x;
        double offsetY = prevPos.y - currentPos.y;

        // Camera controlls
        prevPos = currentPos;
        if (abs(offsetX) > 0.1f) {
            camera.yawRotate(ETime::deltaTime * -offsetX);
            camera.pitchRotate(ETime::deltaTime * -offsetY);
        }

        if (InputHandler::getKey(GLFW_KEY_W)) {
            camera.localTranslate(glm::vec3(0.0, 0.0, -1.0) * Speed * ETime::deltaTime);
        }
        if (InputHandler::getKey(GLFW_KEY_S)) {
            camera.localTranslate(glm::vec3(0.0, 0.0, 1.0) * Speed * ETime::deltaTime);
        }
        if (InputHandler::getKey(GLFW_KEY_A)) {
            camera.localTranslate(glm::vec3(-1.0, 0.0, 0.0) * Speed * ETime::deltaTime);
        }
        if (InputHandler::getKey(GLFW_KEY_D)) {
            camera.localTranslate(glm::vec3(1.0, 0.0, 0.0) * Speed * ETime::deltaTime);
        }

        if (InputHandler::getKeyDown(GLFW_KEY_ESCAPE)) {
            hide_mouse = !hide_mouse;
            InputHandler::disableCursor(hide_mouse);
        }

        if (InputHandler::getKeyDown(GLFW_KEY_R)) {
            line_mode = !line_mode;
            glPolygonMode(GL_FRONT_AND_BACK, line_mode ? GL_LINE : GL_FILL);
        }

        if (InputHandler::getKeyDown(GLFW_KEY_N)) {
            cout << "next point" << endl;

            int prev = convexHull.next();
            while (prev == -1) {
                prev = convexHull.next();
            }
            convexHull.createMeshData();

            int triangleArrSize = convexHull.numberOfTriangles * 3 * sizeof(int);
            unsigned int nextSimplex = create_mesh_colored(verticesSimplex, convexHull.colors, numVertices, convexHull.triangles, triangleArrSize);
            cube.setMesh(nextSimplex, convexHull.numberOfTriangles * 3, true);
            cout << convexHull.numberOfTriangles << " triangles" << endl;
            if (prev == 0) {
                cout << "done no more points left" << endl;
            } else {
                float pn[3];
                convexHull.getNextPoint(pn);
                // cout << pn[0] / 200.0f << " " << pn[1] / 100.0f << " " << pn[2] / 100.0f << endl;
                circle.localPosition = glm::vec3(pn[0] / 100.0f - 1, pn[1] / 100.0f - 1, pn[2] / 100.0f - 1);
            }

            cout << "--------------------"
                 << "\n press R to view in wireframe mode\n Press N to add next point\n " << endl;
            cout << " You can move with mouse\n Press Escape to enable cursor\n Press A,D,W,S to move around\n--------------------" << endl;
        }
        // obj2.lookAt(obj3.position);

        glfwSwapBuffers(engine.window);
        glfwPollEvents();
    }

    return 0;
}

int main(void) {
    // return old();
    return simplex_scene();
    //     return 0;
}
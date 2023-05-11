#include "etime.hpp"
#include "functions.hpp"

float ETime::timePassed = 0.0f;
float ETime::deltaTime = 0.0f;

void ETime::update() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - timePassed;
    timePassed = currentFrame;
}

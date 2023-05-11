#pragma once
#include "functions.hpp"
#include "shaders.hpp"
#include <map>
#include <string>

// typedef class GameObject;

class Material {
private:
    std::map<std::string, int> intUniforms;
    std::map<std::string, float> floatUniforms;
    std::map<std::string, bool> boolUniforms;
    std::map<std::string, int> textures;
    std::map<std::string, glm::vec4> fv4s;
    friend class GameObject;

public:
    Shader shader;
    // Material(const Material &material);
    Material(const Shader &shader);
    Material(const Material &material);
    void setBool(const std::string &name, bool value);
    void setInt(const std::string &name, int value);
    void setFloat(const std::string &name, float value);
    void setTexture(std::string textureName, unsigned int textureID);
    void setFv4(const std::string &name, float v1, float v2, float v3, float v4);
    void use();
};

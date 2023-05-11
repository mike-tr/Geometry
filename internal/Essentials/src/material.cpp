#include "material.hpp"

Material::Material(const Shader &shader) : shader(shader) {
}

Material::Material(const Material &material) : shader(material.shader) {
    // std::cout << "copy" << std::endl;
    this->boolUniforms.clear();
    this->floatUniforms.clear();
    this->intUniforms.clear();
    this->textures.clear();
    this->fv4s.clear();
    for (const auto &pair : material.boolUniforms) {
        boolUniforms[pair.first] = pair.second;
    }
    for (const auto &pair : material.intUniforms) {
        intUniforms[pair.first] = pair.second;
    }
    for (const auto &pair : material.floatUniforms) {
        floatUniforms[pair.first] = pair.second;
    }
    for (const auto &pair : material.textures) {
        textures[pair.first] = pair.second;
        // std::cout << pair.first << " ," << pair.second << std::endl;
    }
    for (const auto &pair : material.fv4s) {
        fv4s[pair.first] = pair.second;
    }
}

void Material::setBool(const std::string &name, bool value) {
    this->boolUniforms[name] = value;
}

void Material::setInt(const std::string &name, int value) {
    this->intUniforms[name] = value;
}

void Material::setFloat(const std::string &name, float value) {
    this->floatUniforms[name] = value;
}

void Material::setFv4(const std::string &name, float v1, float v2, float v3, float v4) {
    this->fv4s[name] = glm::vec4(v1, v2, v3, v4);
}

void Material::setTexture(std::string textureName, unsigned int textureID) {
    // this->setInt(textureName, textureUint);
    this->textures[textureName] = textureID;
    // std::cout << textureName << " - "
    //           << "New val : " << textureID << " actuall new :" << this->textures.at(textureUint) << std::endl;
}

void Material::use() {
    shader.use();
    for (const auto &pair : boolUniforms) {
        shader.setBool(pair.first, pair.second);
    }
    for (const auto &pair : intUniforms) {
        shader.setInt(pair.first, pair.second);
    }
    for (const auto &pair : floatUniforms) {
        shader.setFloat(pair.first, pair.second);
    }
    int i = 0;
    for (const auto &pair : textures) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, pair.second);
        shader.setInt(pair.first, i);
        i++;
        // std::cout << pair.first << " ," << pair.second << std::endl;
    }
    for (const auto &pair : fv4s) {
        shader.setVec4(pair.first, pair.second);
    }
}
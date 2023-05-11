#pragma once
#include "functions.hpp"
#include "node.hpp"
#include "property.hpp"

namespace Engine {
namespace Node {
class Transform : public Node {
private:
    glm::mat4 rotationMatrix;
    glm::vec3 _localScale;
    glm::mat4 transformationMatrix;
    glm::vec3 _localPosition;

    const glm::vec3 getLocalPosition();
    const glm::vec3 getGlobalPosition();
    void setLocalPosition(const glm::vec3 &pos);
    void setGlobalPosition(const glm::vec3 &pos);

    void translate(const glm::vec3 &direction);

protected:
    void updateTransformMatrix(glm::mat4 const parentTransformMatrix) override;

public:
    // Property<Transform, glm::vec3> position = Property(*this, _localPosition, recalculate);
    GetterSetter<Transform, glm::vec3> position = GetterSetter(*this, setGlobalPosition, translate, getGlobalPosition);
    GetterSetter<Transform, glm::vec3> localPosition = GetterSetter(*this, setLocalPosition, translate, getLocalPosition);
    Property<Transform, glm::vec3> localScale = Property(*this, _localScale, recalculate);

    Transform();

    // void localTranslate(const glm::vec3 &direction);
    void scale(glm::vec3 scale);
    void rotate(float degRadians, glm::vec3 axis);
    void lookAt(glm::vec3 target);
    void recalculate();
    const glm::vec3 forward();
    const glm::vec3 right();
    const glm::vec3 up();
};
} // namespace Node
} // namespace Engine
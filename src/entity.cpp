#include "entity.h"

Entity::Entity()
{
}

void Entity::drawNow(ShaderProgram &shader) {
    if (!parent) trsMatrix = getTRSMatrix();
    else trsMatrix = getTRSMatrix() * parent->trsMatrix;
    shader.loadMatrix4f("modelMat", trsMatrix);
    for (int i = 0; i < meshes.size(); i++) {
        Material* mat = materials[i];
        if (mat) {
            if (mat->diffuseMaps.size() > 0) mat->diffuseMaps[0]->bind(0);
        }
        meshes[i]->drawNow();
    }
    for (int i = 0; i < children.size(); i++) {
        children[i]->drawNow(shader);
    }
}

void Entity::updateChildrenMatrices() {
    

}

glm::mat4 Entity::getTRSMatrix()
{
    glm::mat4 scl = glm::scale(glm::mat4(1.0f), scale);
    glm::mat4 rot = glm::mat4_cast(rotation);
    glm::mat4 trans = glm::translate(glm::mat4(1.0), position);
    return trans * rot * scl;
    
}

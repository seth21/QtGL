#include "entity.h"

Entity::Entity()
{
    rotation = glm::quat(1.0, 0.0, 0.0, 0.0);
    position = glm::vec3(0);
    scale = glm::vec3(1);
    trsMatrix = glm::mat4(1.0);
    model = nullptr;
}

void Entity::drawNow(ShaderProgram *shader) {
    if (!parent) trsMatrix = getTRSMatrix();
    else trsMatrix = getTRSMatrix() * parent->trsMatrix;
    shader->loadMatrix4f("modelMat", trsMatrix);
    for (int i = 0; i < model->meshes.size(); i++) {
        Mesh* mesh = model->meshes[i];
        Material* mat = model->materials[mesh->materialIndex];
        if (mat) {
            if (mat->diffuseMaps.size() > 0) mat->diffuseMaps[0]->bind(0);
        }
        mesh->drawNow();
    }
    for (int i = 0; i < children.size(); i++) {
        //children[i]->drawNow(shader);
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

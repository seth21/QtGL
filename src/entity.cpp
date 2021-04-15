#include "entity.h"

Entity::Entity()
{
    rotation = glm::quat(1.0, 0.0, 0.0, 0.0);
    position = glm::vec3(0);
    scale = glm::vec3(1);
    trsMatrix = glm::mat4(1.0);
    model = nullptr;
}

void Entity::drawNow(ShaderProgram *shader, Camera* camera, bool frustumCull) {
    if (!parent) trsMatrix = getTRSMatrix();
    else trsMatrix = getTRSMatrix() * parent->trsMatrix;
    shader->loadMatrix4f("modelMat", trsMatrix);
    
    drawCount = 0;
    for (int i = 0; i < model->meshes.size(); i++) {
        Mesh* mesh = model->meshes[i];
        mesh->transformedAabb.setAABB(mesh->aabb);
        mesh->transformedAabb.mul(trsMatrix);
        if (frustumCull && !camera->frustum->boundsInFrustum(mesh->transformedAabb)) continue;
        drawCount++;
        Material* mat = model->materials[mesh->materialIndex];
        if (mat) {
            int counter = 0;
            for (auto tex : mat->getTextures()) {
                shader->loadInt(tex.first, counter);
                tex.second->bind(counter);
                counter++;
            }
            for (auto f : mat->getFloats()) {
                shader->loadFloat(f.first, f.second);
            }
            for (auto v3 : mat->getVec3s()) {
                shader->loadVector3f(v3.first, v3.second);
            }
        }
        mesh->drawNow();
    }
    //qDebug() << "Meshes drawn:" << drawCount;
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

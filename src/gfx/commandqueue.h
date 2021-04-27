#ifndef COMMANDQUEUE_H
#define COMMANDQUEUE_H
#include <vector>
#include "rendercommand.h"
#include "ecs/comp/transformComp.h"
#include "ecs/comp/meshrendererComp.h"
class CommandQueue {
public:
	CommandQueue();
	void createCommand(unsigned int vaoID, unsigned int baseVertex, unsigned int vertexCount, glm::mat4x4 transformMatrix, Material *material, float distSq);
	void sort();
	std::vector<RenderCommand>& getCommands();
	void clear();

private:
	std::vector<RenderCommand> m_Commands;
};
#endif
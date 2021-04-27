#include "commandqueue.h"

CommandQueue::CommandQueue()
{
}

void CommandQueue::createCommand(unsigned int vaoID, unsigned int baseVertex, unsigned int vertexCount, glm::mat4x4 transformMatrix, Material* material, float distSq)
{
	m_Commands.push_back({ vaoID, baseVertex, vertexCount, distSq, material, transformMatrix });
}

void CommandQueue::sort()
{
}

std::vector<RenderCommand>& CommandQueue::getCommands()
{
	return m_Commands;
}

void CommandQueue::clear()
{
	m_Commands.clear();
}

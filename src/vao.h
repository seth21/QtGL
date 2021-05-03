#ifndef VAO_H
#define VAO_H
#include <vector>
#include <QOpenGLExtraFunctions>
#include "VertexAttrib.h"
#include <QDebug>
class VAO : protected QOpenGLExtraFunctions {
public:
	VAO();
	~VAO();
	float *m_interleavedData;
	int interleavedDataSize;
	std::vector<unsigned int> m_indices;
	
	void upload();
	static unsigned int boundVAO;
	void bind();
	void unbind();
	const static float screenQuadUV[];
	const static float screenQuadPos[];
	int createAttribute(std::string name, int location, int attribSize);
	int createAttribute(std::string name, int location, int attribSize, const float data[], unsigned int dataCount);
	VertexAttrib& getAttribute(int index);
private:
	unsigned int vaoHandle;
	unsigned int vboHandle;
	unsigned int eboHandle;
	void interleaveData();
	void calculateVboOffsets();
	int getStride();
	std::vector<VertexAttrib> attributes;
};

#endif
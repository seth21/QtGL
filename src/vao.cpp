#include "vao.h"

unsigned int VAO::boundVAO = 0;
const float VAO::screenQuadPos[12] = {
    // positions 
    -1.0f,  1.0f,
    -1.0f, -1.0f,
     1.0f, -1.0f,

    -1.0f,  1.0f,
     1.0f, -1.0f,
     1.0f,  1.0f,
};
const float VAO::screenQuadUV[12] = {
    // texCoords
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,

    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f
};

VAO::VAO()
{
    f = QOpenGLContext::currentContext()->extraFunctions();
	m_interleavedData = NULL;
    interleavedDataSize = 0;
	vaoHandle = NULL;
    vboHandle = NULL;
    eboHandle = NULL;
}

VAO::~VAO()
{
    delete m_interleavedData;
    f->glDeleteVertexArrays(1, &vaoHandle);
    f->glDeleteBuffers(1, &vboHandle);
    f->glDeleteBuffers(1, &eboHandle);
}

void VAO::upload()
{
    interleaveData();
    // Create buffers/arrays
    f->glGenVertexArrays(1, &vaoHandle);
    f->glGenBuffers(1, &vboHandle);
    if (m_indices.size() > 0) f->glGenBuffers(1, &eboHandle);

    f->glBindVertexArray(vaoHandle);
    // Load data into vertex buffers
    f->glBindBuffer(GL_ARRAY_BUFFER, vboHandle);
    
    f->glBufferData(GL_ARRAY_BUFFER, interleavedDataSize * sizeof(float), &m_interleavedData[0], GL_STATIC_DRAW);
    for (int i = 0; i < 24; i++) qDebug() << m_interleavedData[i];

    if (m_indices.size() > 0) {
        f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboHandle);
        f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
    }

    // Set the vertex attribute pointers
    calculateVboOffsets();
    int stride = getStride();
    for (int i = 0; i < attributes.size(); i++) {
        f->glVertexAttribPointer(attributes[i].m_location, attributes[i].m_size, GL_FLOAT, GL_FALSE, sizeof(float) * stride, (void*)(attributes[i].m_vboOffset * sizeof(float)));
        f->glEnableVertexAttribArray(attributes[i].m_location);
    }

    f->glBindBuffer(GL_ARRAY_BUFFER, 0);
    f->glBindVertexArray(0);
   
}

void VAO::bind()
{
    //if (boundVAO != vaoHandle) {
    f->glBindVertexArray(vaoHandle);
        boundVAO = vaoHandle;
    //}
   
}

void VAO::unbind()
{
    f->glBindVertexArray(0);
    boundVAO = 0;
}

void VAO::interleaveData() {
	int arraySize = 0;
	int stride = 0;
	for (int i = 0; i < attributes.size(); i++) {
		arraySize += attributes[i].m_data.size();
		stride += attributes[i].m_size;
	}
    interleavedDataSize = arraySize;
	if (m_interleavedData) delete m_interleavedData;
	m_interleavedData = new float[arraySize];

	int currStrides = 0;
	for (int i = 0; i < arraySize; i += stride) {

		int currAttrib = 0;
		int currOffset = 0;
		for (int a = 0; a < stride; a++) {

			int currAttribSize = attributes[currAttrib].m_size;
			m_interleavedData[i + a] = attributes[currAttrib].m_data[currStrides * currAttribSize + a - currOffset];

			if (a - currOffset == currAttribSize - 1) {
				currOffset += currAttribSize;
				currAttrib++;
			}
		}
		currStrides++;
	}
}

int VAO::createAttribute(std::string name, int location, int attribSize)
{
    VertexAttrib a(name, location, attribSize);
    attributes.push_back(a);
    return attributes.size()-1;
}

int VAO::createAttribute(std::string name, int location, int attribSize, const float data[], unsigned int dataCount)
{
    VertexAttrib a(name, location, attribSize);
    attributes.push_back(a);
    attributes[attributes.size() - 1].setData(data, dataCount);
    return attributes.size() - 1;
}

VertexAttrib& VAO::getAttribute(int index) {
    return attributes[index];
}

void VAO::calculateVboOffsets()
{
    int currOffset = 0;
    for (int i = 0; i < attributes.size(); i++) {
        attributes[i].m_vboOffset = currOffset;
        currOffset += attributes[i].m_size;
    }
}

int VAO::getStride()
{
    int stride = 0;
    for (int i = 0; i < attributes.size(); i++) stride += attributes[i].m_size;
    return stride;
}

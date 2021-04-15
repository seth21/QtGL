#include "vertexattrib.h"
#include <QDebug>
const std::string VertexAttrib::POSITION = "aPos";
const std::string VertexAttrib::NORMAL = "aNormal";
const std::string VertexAttrib::TEXCOORDS = "aTexCoord";
const std::string VertexAttrib::TANGENT = "aTangent";
const std::string VertexAttrib::COLOR = "aColor";

VertexAttrib::VertexAttrib(std::string name, int location, int attribSize)
{
	m_name = name;
	m_location = location;
	m_size = attribSize;

}

VertexAttrib::~VertexAttrib()
{
	
}

void VertexAttrib::setData(const float data[], unsigned int size)
{
	if (m_data.size() > 0) m_data.clear();
	//qDebug() << m_data.size();
	//qDebug() << "AAAAAAAAAA";
	//m_data.reserve(size);
	for (int i = 0; i < size; i++) m_data.push_back(data[i]);
	//qDebug() << "BBBBBBBB";
}

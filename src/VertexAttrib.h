#ifndef VERTEX_ATTRIB_H
#define VERTEX_ATTRIB_H
#include <string>
#include <vector>

class VertexAttrib {
public:
	std::string m_name;
	int m_location;
	int m_size;
	int m_count;
	int m_vboOffset;
	std::vector<float> m_data;
	VertexAttrib(std::string name, int location, int attribSize);
	~VertexAttrib();
	static const std::string POSITION;
	static const std::string NORMAL;
	static const std::string TEXCOORDS;
	static const std::string TANGENT;
	static const std::string COLOR;
	void setData(const float data[], unsigned int size);
	
private:

};

#endif // !VERTEX_ATTRIB_H

#include "shaderprogram.h"
#include <iostream>
#include <fstream>
#include <QDebug>
GLuint ShaderProgram::currentProg = 0;

ShaderProgram::ShaderProgram(const std::string& fileName, ResourceConfig config) : Resource(fileName, config)
{
    f = QOpenGLContext::currentContext()->extraFunctions();
    this->config = config;

    //addFlag("ALBEDO");
    init(fileName);
}

void ShaderProgram::init(std::string shaderName)
{
    if (programID != 0) std::cout << "ShaderProgram already initialized!" << std::endl;
    //initializeOpenGLFunctions();
#ifdef PYTHON_USE
	std::string shaderDir = QApplication::arguments()[1].toStdString() + "/shaders/" + shaderName;
#else
    std::string shaderDir = QApplication::applicationDirPath().toStdString() + "/resources/shaders/" + shaderName;
#endif
	
    unsigned int vertexShader = prepareShader(GL_VERTEX_SHADER, shaderDir + ".vert");
    unsigned int fragmentShader = prepareShader(GL_FRAGMENT_SHADER, shaderDir +".frag");

    programID = f->glCreateProgram();
    f->glAttachShader(programID, vertexShader);
    f->glAttachShader(programID, fragmentShader);

    //If needed -> setting attributes always before linking, example:
    //glBindAttribLocation(programID, 1, "position");

    f->glLinkProgram(programID);
    f->glValidateProgram(programID);

    // Print linking errors if any
    GLint success;
    GLchar infoLog[512];
    f->glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        f->glGetProgramInfoLog(programID, 512, NULL, infoLog);
        qDebug() << shaderName.c_str() << " ERROR::SHADER::PROGRAM::LINKING_FAILED " << infoLog;
        fileLoaded = false;
    }
    else{
        qDebug() << "Loaded shader ->" << shaderName.c_str() << " with flags:";
        for (auto flag : config.flags) {
            qDebug() << flag.c_str();
        }
        fileLoaded = true;
    }
    //cleanup
    f->glDetachShader(programID, vertexShader);
    f->glDetachShader(programID, fragmentShader);
    f->glDeleteShader(vertexShader);
    f->glDeleteShader(fragmentShader);
}

GLuint ShaderProgram::prepareShader(GLenum type, const std::string& shaderPath){
    std::string shaderStr = readFile(shaderPath);
    const char *shaderSrc = shaderStr.c_str();

    unsigned int shader = f->glCreateShader(type);
    //attach the source and compile
    f->glShaderSource(shader, 1, &shaderSrc, NULL);
    f->glCompileShader(shader);

    GLint success;
    GLchar infoLog[512];
    f->glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        f->glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("ERROR::SHADER::COMPILATION_FAILED\n");
    }
    return shader;
}

bool ShaderProgram::start(){
    if (ShaderProgram::currentProg != programID){
        f->glUseProgram(programID);
        ShaderProgram::currentProg = programID;
        return true;
    }
    return false;
}

void ShaderProgram::stop(){
    f->glUseProgram(0);
    ShaderProgram::currentProg = 0;
}

GLuint ShaderProgram::getUniformLocation(const std::string &name){
    GLintDefault loc = uniformLocs[name];
    if (loc.val == -2)
    {
        
        //hasnt been cached yet
        loc.val = f->glGetUniformLocation(programID, name.c_str());
        uniformLocs[name] = loc;
        if (loc.val == -1){
            qDebug() << "No uniform with name '" << name.c_str() << "' in shader";
        }
        else{
            qDebug() << "Cached uniform with name '" << name.c_str() << "' with id:" << loc.val;
        }
    }
    return loc.val;
}

void ShaderProgram::loadFloat(const std::string &name, GLfloat data){
    f->glUniform1f(getUniformLocation(name), data);
}

void ShaderProgram::loadInt(const std::string &name, GLint data){
    f->glUniform1i(getUniformLocation(name), data);
}

void ShaderProgram::loadBoolean(const std::string &name, bool value){
    f->glUniform1f(getUniformLocation(name), (value ? 1.0f : 0.0f));
}

void ShaderProgram::loadVector3f(const std::string& name, glm::vec3 vec3) {
    f->glUniform3f(getUniformLocation(name), vec3.x, vec3.y, vec3.z);
}

void ShaderProgram::loadVector3f(const std::string &name, GLfloat x, GLfloat y, GLfloat z){
    f->glUniform3f(getUniformLocation(name), x, y, z);
}

void ShaderProgram::loadVector2f(const std::string& name, glm::vec2 vec2)
{
    f->glUniform2f(getUniformLocation(name), vec2.x, vec2.y);
}

void ShaderProgram::loadVector2f(const std::string &name, GLfloat x, GLfloat y){
    f->glUniform2f(getUniformLocation(name), x, y);
}

void ShaderProgram::loadVector4f(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w){
    f->glUniform4f(getUniformLocation(name), x, y, z, w);
}

bool ShaderProgram::loaded()
{
    return fileLoaded;
}

ResourceConfig& ShaderProgram::getResourceConfig()
{
    return config;
}

void ShaderProgram::loadFloat(GLuint loc, GLfloat data){
    f->glUniform1f(loc, data);
}

void ShaderProgram::loadInt(GLuint loc, GLint data){
    f->glUniform1i(loc, data);
}

void ShaderProgram::loadBoolean(GLuint loc, bool value){
    f->glUniform1f(loc, (value ? 1.0f : 0.0f));
}

void ShaderProgram::loadMatrix4f(const std::string& name, glm::mat4 mat4)
{
    f->glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat4));
}

void ShaderProgram::loadMatrix4f(GLuint loc, glm::mat4 mat4)
{
    f->glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat4));
}

void ShaderProgram::loadVector3f(GLuint loc, GLfloat x, GLfloat y, GLfloat z){
    f->glUniform3f(loc, x, y, z);
}

void ShaderProgram::loadVector2f(GLuint loc, GLfloat x, GLfloat y){
    f->glUniform2f(loc, x, y);
}

void ShaderProgram::loadVector4f(GLuint loc, GLfloat x, GLfloat y, GLfloat z, GLfloat w){
    f->glUniform4f(loc, x, y, z, w);
}

std::string ShaderProgram::readFile(const std::string& filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);
    if (!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    bool versionFound = false;
    while (!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
        if (!versionFound && (line.rfind("#version"), 0) == 0) {
            versionFound = true;
            for (auto flag : config.flags)
                content.append("#define " + flag + "\n");
        }
        //std::cout << line << std::endl;
    }

    fileStream.close();
    return content;
}

ShaderProgram::~ShaderProgram()
{
    stop();
    f->glDeleteProgram(programID);
}


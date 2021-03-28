#include "shaderprogram.h"
#include <iostream>
#include <fstream>
#include <QDebug>
GLuint ShaderProgram::currentProg = 0;

ShaderProgram::ShaderProgram(const std::string& fileName, ResourceConfig config) : Resource(fileName, config)
{
    this->config = config;
    for (int i = 0; i < config.flags.size(); i++) {
        addFlag(config.flags[i]);
    }
    //addFlag("ALBEDO");
    init(fileName);
}

void ShaderProgram::addFlag(std::string flag) {
    definedFlags.push_back(flag);
}

void ShaderProgram::init(std::string shaderName)
{
    if (programID != 0) std::cout << "ShaderProgram already initialized!" << std::endl;
    initializeOpenGLFunctions();
    std::string shaderDir = QApplication::applicationDirPath().toStdString() + "/resources/shaders/" + shaderName;
	
    unsigned int vertexShader = prepareShader(GL_VERTEX_SHADER, shaderDir + ".vert");
    unsigned int fragmentShader = prepareShader(GL_FRAGMENT_SHADER, shaderDir +".frag");

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);

    //If needed -> setting attributes always before linking, example:
    //glBindAttribLocation(programID, 1, "position");

    glLinkProgram(programID);
    glValidateProgram(programID);

    // Print linking errors if any
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programID, 512, NULL, infoLog);
        qDebug() << shaderName.c_str() << " ERROR::SHADER::PROGRAM::LINKING_FAILED";
        fileLoaded = false;
    }
    else{
        qDebug() << "Loaded shader ->" << shaderName.c_str();
        fileLoaded = true;
    }
    //cleanup
    glDetachShader(programID, vertexShader);
    glDetachShader(programID, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

GLuint ShaderProgram::prepareShader(GLenum type, const std::string& shaderPath){
    std::string shaderStr = readFile(shaderPath);
    const char *shaderSrc = shaderStr.c_str();

    unsigned int shader = glCreateShader(type);
    //attach the source and compile
    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("ERROR::SHADER::COMPILATION_FAILED\n");
    }
    return shader;
}

void ShaderProgram::start(){
    if (ShaderProgram::currentProg != programID){
        glUseProgram(programID);
        ShaderProgram::currentProg = programID;
    }

}

void ShaderProgram::stop(){
    glUseProgram(0);
    ShaderProgram::currentProg = 0;
}

GLuint ShaderProgram::getUniformLocation(const std::string &name){
    GLintDefault loc = uniformLocs[name];
    if (loc.val == -2)
    {
        //hasnt been cached yet
        loc.val = glGetUniformLocation(programID, name.c_str());
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
    glUniform1f(getUniformLocation(name), data);
}

void ShaderProgram::loadInt(const std::string &name, GLint data){
    glUniform1i(getUniformLocation(name), data);
}

void ShaderProgram::loadBoolean(const std::string &name, bool value){
    glUniform1f(getUniformLocation(name), (value ? 1.0f : 0.0f));
}

void ShaderProgram::loadVector3f(const std::string& name, glm::vec3 vec3) {
    glUniform3f(getUniformLocation(name), vec3.x, vec3.y, vec3.z);
}

void ShaderProgram::loadVector3f(const std::string &name, GLfloat x, GLfloat y, GLfloat z){
    glUniform3f(getUniformLocation(name), x, y, z);
}

void ShaderProgram::loadVector2f(const std::string& name, glm::vec2 vec2)
{
    glUniform2f(getUniformLocation(name), vec2.x, vec2.y);
}

void ShaderProgram::loadVector2f(const std::string &name, GLfloat x, GLfloat y){
    glUniform2f(getUniformLocation(name), x, y);
}

void ShaderProgram::loadVector4f(const std::string &name, GLfloat x, GLfloat y, GLfloat z, GLfloat w){
    glUniform4f(getUniformLocation(name), x, y, z, w);
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
    glUniform1f(loc, data);
}

void ShaderProgram::loadInt(GLuint loc, GLint data){
    glUniform1i(loc, data);
}

void ShaderProgram::loadBoolean(GLuint loc, bool value){
    glUniform1f(loc, (value ? 1.0f : 0.0f));
}

void ShaderProgram::loadMatrix4f(const std::string& name, glm::mat4 mat4)
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat4));
}

void ShaderProgram::loadMatrix4f(GLuint loc, glm::mat4 mat4)
{
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat4));
}

void ShaderProgram::loadVector3f(GLuint loc, GLfloat x, GLfloat y, GLfloat z){
    glUniform3f(loc, x, y, z);
}

void ShaderProgram::loadVector2f(GLuint loc, GLfloat x, GLfloat y){
    glUniform2f(loc, x, y);
}

void ShaderProgram::loadVector4f(GLuint loc, GLfloat x, GLfloat y, GLfloat z, GLfloat w){
    glUniform4f(loc, x, y, z, w);
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
            for (int i = 0; i < definedFlags.size(); i++)
                content.append("#define " + definedFlags[i] + "\n");
        }
        //std::cout << line << std::endl;
    }

    fileStream.close();
    return content;
}

ShaderProgram::~ShaderProgram()
{
    stop();
    glDeleteProgram(programID);
}


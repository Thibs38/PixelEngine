#include "Shader.hpp"
#include "io/FileIO.hpp"
#include "io/Error.hpp"

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>


uint loadShader(constring path, ShaderType shaderType, std::string& shaderCode)
{
	std::ifstream file;
	if (!openFile(&file, "res/shaders/" + path))     //First we open the file and return 0 if couldn't be opened
		return 0;

	readFile(file, shaderCode);           //Read the bytes from the file

    const char* constShaderCode = shaderCode.c_str();
    uint shader = glCreateShader((GLenum)shaderType);
    glShaderSource(shader, 1, &constShaderCode, NULL);
    glCompileShader(shader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        ShaderError errorType;
        switch (shaderType)
        {
        case ShaderType::VERTEX_SHADER:
            errorType = ShaderError::CANT_COMPILE_VERTEX;
            break;
        case ShaderType::FRAGMENT_SHADER:
            errorType = ShaderError::CANT_COMPILE_FRAGMENT;
            break;
        case ShaderType::GEOMETRY_SHADER:
            errorType = ShaderError::CANT_COMPILE_GEOMETRY;
            break;
        }
        ErrorManager::printShaderError(errorType, path, infoLog);
        
        return 0;
    }
    return shader;
}

uint linkShaders(uint id, std::string name, const VertexShader* vertexShader, const GeometryShader* geometryShader, const FragmentShader* fragmentShader)
{
    uint shaderProgram = glCreateProgram();
    int success;
    char infoLog[512];

    glAttachShader(shaderProgram, vertexShader->shaderID);
    if (geometryShader != nullptr)
        glAttachShader(shaderProgram, geometryShader->shaderID);
    glAttachShader(shaderProgram, fragmentShader->shaderID);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        ErrorManager::printShaderError(ShaderError::CANT_LINK, "", infoLog, 
            vertexShader->name, 
            geometryShader != nullptr? geometryShader->name : "", 
            fragmentShader->name);
        return 0;
    }
    new Shader(id, name, shaderProgram, 0, 
        vertexShader->shaderID, 
        geometryShader != nullptr? geometryShader->shaderID : 0, 
        fragmentShader->shaderID);

    return shaderProgram;
}


#pragma region ShaderBase

ShaderBase::ShaderBase(uint id, std::string name, uint shaderID):
    id(id), name(name), shaderID(shaderID){}

ShaderBase::~ShaderBase()
{

}

VertexShader::VertexShader(uint id, std::string name, uint shaderID) : ShaderBase(id,name,shaderID)
{
    VertexShader::vertexShaders.push_back(this);
}

VertexShader::~VertexShader()
{
    VertexShader::vertexShaders.erase(VertexShader::vertexShaders.begin() + id); //Remove the shader from the static array
}

GeometryShader::GeometryShader(uint id, std::string name, uint shaderID) : ShaderBase(id, name, shaderID)
{
    GeometryShader::geometryShaders.push_back(this);
}

GeometryShader::~GeometryShader()
{
    GeometryShader::geometryShaders.erase(GeometryShader::geometryShaders.begin() + id); //Remove the shader from the static array
}

FragmentShader::FragmentShader(uint id, std::string name, uint shaderID) : ShaderBase(id, name, shaderID)
{
    FragmentShader::fragmentShaders.push_back(this);
}

FragmentShader::~FragmentShader()
{
    FragmentShader::fragmentShaders.erase(FragmentShader::fragmentShaders.begin() + id); //Remove the shader from the static array
}

void VertexShader::destroy()
{
    for (const VertexShader* s : vertexShaders)
    {
        delete s;
    }
}

void GeometryShader::destroy()
{
    for (const GeometryShader* s : geometryShaders)
    {
        delete s;
    }
}


void FragmentShader::destroy()
{
    for (const FragmentShader* s : fragmentShaders)
    {
        delete s;
    }
}


#pragma endregion

#pragma region Shader

void Shader::start()
{
    glUseProgram(programID);
}

void Shader::stop()
{
    glUseProgram(0);
}

void Shader::bindAttribute(uint attribute, const char* attribName)
{
    glBindAttribLocation(programID, attribute, attribName);
}

std::vector<Shader&> Shader::shaders;
Shader::Shader(uint id, std::string name, uint programID, uint attribCount, uint vShaderID, uint gShaderID, uint fShaderID) :
    id(id), name(name), programID(programID), attribCount(attribCount), vertexShaderID(vShaderID), geometryShaderID(gShaderID), fragmentShaderID(fShaderID)
{
    shaders.push_back(*this);
}

Shader::~Shader()
{
    glDetachShader(programID, vertexShaderID);
    glDeleteProgram(vertexShaderID);
    if (geometryShaderID != 0)
    {
        glDetachShader(programID, geometryShaderID);
        glDeleteProgram(geometryShaderID);
    }
    glDetachShader(programID, fragmentShaderID);
    glDeleteProgram(fragmentShaderID);
    glDeleteProgram(programID);
    Shader::shaders.erase(Shader::shaders.begin() + id); //Remove the shader from the static array
}

void Shader::destroy()
{
    for (const Shader* s : shaders)
    {
        delete s;
    }
}

#pragma endregion

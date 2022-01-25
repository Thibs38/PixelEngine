#pragma once

#include "util/Utility.hpp"
#include <glad.h>
#include <vector>

#pragma region Classes

enum class ShaderType
{
	VERTEX_SHADER,
	GEOMETRY_SHADER,
	FRAGMENT_SHADER
	//COMPUTE_SHADER Upgrade to opengl 4.6
};

enum class VarType
{
	BOOL,
	INT,
	UINT,
	FLOAT,
	DOUBLE,
	BVEC2, BVEC3, BVEC4,
	IVEC2, IVEC3, IVEC4,
	UVEC2, UVEC3, UVEC4,
	VEC2, VEC3, VEC4,
	DVEC2, DVEC3, DVEC4,//
	MAT2, MAT3, MAT4,   //
	MAT2x3, MAT2x4,     //
	MAT3x2, MAT3x4,     //
	MAT4x2, MAT4x3,     //
	SAMPLER2D,          //
	STRUCT              //Can only be used as uniform
};


/// <summary>
/// Represents an attribute or a uniform in a shader.
/// </summary>
struct UniformAttrib
{
	VarType type;
	std::string name;
	bool isArray;
};

/// <summary>
/// Represents a Vertex, Geometry or Fragment shader. This is temporary data that is deleted after loading
/// </summary>
struct ShaderBase
{
	const uint id;
	const std::string name;
	const uint shaderID;

	ShaderBase(uint id, std::string name, uint shaderID);
	~ShaderBase();
};

struct VertexShader :ShaderBase
{
	static std::vector<const VertexShader*> vertexShaders;
	VertexShader(uint id, std::string name, uint shaderID);
	~VertexShader();
	static void destroy();
};

struct GeometryShader :ShaderBase
{
	static std::vector<const GeometryShader*> geometryShaders;
	GeometryShader(uint id, std::string name, uint shaderID);
	~GeometryShader();
	static void destroy();
};

struct FragmentShader :ShaderBase
{
	static std::vector<const FragmentShader*> fragmentShaders;
	FragmentShader(uint id, std::string name, uint shaderID);
	~FragmentShader();
	static void destroy();
};

/// <summary>
/// Represents a shader program, a combination of a vertex shader and a fragment shader
/// </summary>
class Shader
{
public:

	const uint id;
	const std::string name;
	std::vector<const int> uniforms;  //Uniforms stored in order of appearance in the shader
	std::vector<const int> attributes;//Attributes stored in order of appearance in the shader

	static std::vector<Shader&> shaders;

	void start();

	static void stop();

	void bindAttribute(uint attribute, const char* attribName);

	Shader(uint id, std::string name, uint programID, uint attribCount, uint vShaderID, uint gShaderID, uint fShaderID);
	~Shader();															

	/// <summary>
	/// Destroys all the shaders stored and unloads them 
	/// </summary>
	static void destroy();

private:

	const uint programID;
	const uint attribCount;
	const uint vertexShaderID;
	const uint geometryShaderID;
	const uint fragmentShaderID;
};

#pragma endregion

/// <summary>
/// Loads and compile a shader.
/// </summary>
/// <param name="path">The path to the shader.</param>
/// <param name="shaderType">The type of the shader: Vertex, Fragment or Geometry.</param>
/// <returns> 0 if an error occured, the id of the shader otherwise. </returns>
uint loadShader(constring path, ShaderType shaderType, std::string& shaderCode);

/// <summary>
/// Links shaders together and makes a shader program.
/// </summary>
/// <param name="id"> The id of the shader program. </param>
/// <param name="name"> The name of the shader program. </param>
/// <param name="vertexShader"> The vertex shader to link. </param>
/// <param name="geometryShader"> The geometry shader to link. Put nullptr to not link one.</param>
/// <param name="fragmentShader"> The fragment shader to link. </param>
/// <returns> 0 if a problem occured, the programID otherwise. </returns>
uint linkShaders(uint id, std::string name, const VertexShader* vertexShader, const GeometryShader* geometryShader, const FragmentShader* fragmentShader);



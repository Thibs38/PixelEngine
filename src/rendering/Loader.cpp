#include "Loader.hpp"
#include "io/Error.hpp"
#include "io/FileIO.hpp"
#include "io/JSON.hpp"
//#include "IO/WREN.hpp"

#include <glad.h>

std::vector<unsigned int> Loader::vaos;
std::vector<unsigned int> Loader::vbos;
std::vector<unsigned int> Loader::textures;

void Loader::init() //SAFE Add program exit on fatal errors
{
	/* Every function here are reading a JSON file that specifies how to build the objects
	* or interpret a file. Functions from Shader.h, FileIO.h, Loader.h and Wren.h are called
	* from the JSON.h functions.
	*/

	ErrorManager::init();            //Loads all the errors
	readTextures();					 //Loads all the textures
	RawModel::generateQuad();		 //Loads all the raw models
	readMaterials();                 //Loads all the materials
	//Load components
	readGameObjects();				 //Loads all the Gameobjects
	readShaders();					 //Loads all the shaders
	//WrenManager::init();           <//Loads all the wren scripts

	printf("Loading completed\n"); //TODO Green text
}

void Loader::destroy() //TODO Add model, textures, rawmodels, material and shader destroy
{
	glDeleteVertexArrays(vaos.size(), vaos.data());
	glDeleteBuffers(vbos.size(), vbos.data());
	glDeleteTextures(textures.size(), textures.data());

	vaos.clear();
	vbos.clear();
	textures.clear();

	ErrorManager::destroy();
}

#pragma region VAO STUFF
RawModel* Loader::loadToVao(uint id, constring name, float* positions, unsigned int* indices, float* textureCoords, unsigned int vertexCount)
{
	unsigned int vaoID = createVAO();
	glBindVertexArray(vaoID);
	unsigned int iboID = bindIndiceBuffer(indices);
	storeDataInVertexAttribute(0, positions);
	storeDataInVertexAttribute(1, textureCoords);
	//Be careful, there's no unbinding of the vbos and the vao
	return new RawModel(id, name, vaoID, iboID, vertexCount);
}

unsigned int Loader::createVAO()
{
	unsigned int vaoID;
	glGenVertexArrays(1, &vaoID);
	vaos.push_back(vaoID);
	return vaoID;
}

unsigned int Loader::createVBO()
{
	unsigned int vboID;
	glGenBuffers(1, &vboID);
	vbos.push_back(vboID);
	return vboID;
}

unsigned int Loader::bindIndiceBuffer(unsigned int* indices)
{
	unsigned int iboID = Loader::createVBO();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	return iboID;
}

void Loader::storeDataInVertexAttribute(int attribNumber, float* data)
{
	unsigned int vboID = Loader::createVBO();
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	glVertexAttribPointer(attribNumber, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void Loader::storeDataInVertexAttribute(int attribNumber, uint8* data)
{
	unsigned int vboID = Loader::createVBO();
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	glVertexAttribIPointer(attribNumber, 3, GL_UNSIGNED_BYTE, sizeof(uint8), (void*)0);
}

Texture* Loader::loadTexture(uint id, constring name, unsigned char* data, int width, int height)
{
	unsigned int texture;
	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	return new Texture(id, name, texture);
}

#pragma endregion
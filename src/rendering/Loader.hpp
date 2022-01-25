#pragma once

#include "Rendering/Model.hpp"
#include "util/Utility.hpp"
#include <string>
#include <vector>
#include <array>

/// <summary>
/// A static class that manage the creation of vbos, textures and link them to vaos
/// </summary>
class Loader
{
private:
	static std::vector<unsigned int> vaos;
	static std::vector<unsigned int> vbos;
	static std::vector<unsigned int> textures;

	/// <summary>
	/// Creates a vao.
	/// </summary>
	/// <returns> the ID of the vao</returns>
	static unsigned int createVAO();
		
	/// <summary>
	/// Creates a vbo.
	/// </summary>
	/// <returns>The vbos' ID</returns>
	static unsigned int createVBO();
	
	/// <summary>
	/// Binds the indices to the current binded vao.
	/// </summary>
	/// <param name="indices">The indices.</param>
	/// <returns>The buffers' ID</returns>
	static unsigned int bindIndiceBuffer(unsigned int* indices);
	
	/// <summary>
	/// Stores the data in a vertex attribute binded to the current vao.
	/// </summary>
	/// <param name="attribNumber">The attribute number.</param>
	/// <param name="data">The data.</param>
	static void storeDataInVertexAttribute(int attribNumber, float* data);

	/// <summary>
	/// Stores the data in a vertex attribute binded to the current vao.
	/// </summary>
	/// <param name="attribNumber">The attribute number.</param>
	/// <param name="data">The data.</param>
	static void storeDataInVertexAttribute(int attribNumber, uint8* data);
	
public:

	/// <summary>
	/// Initialise the loading of the game
	/// </summary>
	static void init();	//OPTI  Can be optimized by passing more strings by reference instead of copy

	/// <summary>
	/// Unload all of the vaos
	/// </summary>
	static void destroy();	

	/// <summary>
	/// Loads the given data to a vao.
	/// </summary>
	/// <param name="positions">The positions.</param>
	/// <param name="indices">The indices.</param>
	/// <param name="textureCoords">The texture coords.</param>
	/// <param name="normals">The normals ID.</param>
	/// <param name="vertexCount">The vertex count.</param>
	/// <returns>A reference to a RawModel representing the data</returns>
	static RawModel* loadToVao(uint id, constring name, float* positions, uint* indices, float* textureCoords, uint vertexCount);	
	
	/// <summary>
	/// Loads the given data to a Texture.
	/// </summary>
	/// <param name="data">The data.</param>
	/// <param name="width">The width of the image.</param>
	/// <param name="height">The height of the image.</param>
	/// <returns>A reference to a Texture object.</returns>
	static Texture* loadTexture(uint id, constring name, uint8* data, int width, int height);
};



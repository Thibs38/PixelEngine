#pragma once

#include <util/Utility.hpp>

#include "rendering/Model.hpp"


/// <summary>
/// Opens the ifstream file at the given path
/// </summary>
/// <param name="file">The ifstream file.</param>
/// <param name="path">The path to the file.</param>
/// <param name="printError">Print errors to the console?</param>
/// <returns>True if  success, false otherwise</returns>
bool openFile(std::ifstream* file, constring path, bool printError = true);

/// <summary>
/// Reads the specified file and place its' content in the given string
/// </summary>
/// <param name="path"> The path of the file</param>
/// <param name="content"> The contents' holder</param>
void readFile(std::ifstream& stream, std::string& content);

/// <summary>
/// <para>Loads a texture. Texture must be located within res/textures.</para> 
/// </summary>
/// <param name="fileName">Relative file name from res/textures</param>
/// <returns>A reference to a Texture object representing the texture</returns>
void readTexture(uint id, constring name, constring fileName);

/// <summary>
/// <para>Loads a model. Model must be located within res/gameobjects.</para> 
/// </summary>
/// <param name="fileName">Relative file name from res/gameobjects</param>
/// <returns>A reference to a RawModel object representing the model</returns>
void readObjModel(uint id, constring name, constring fileName);




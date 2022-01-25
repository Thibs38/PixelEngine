#pragma once

#include "util/Comparators.hpp"
#include "util/Utility.hpp"


/// <summary>
/// Loads all the shader programs in the file res/data/shaders.json
/// </summary>
void readShaders();

/// <summary>
/// Loads all the components, all located in files res/data/components/*
/// </summary>
void readComponents();

/// <summary>
/// Loads all the textures in the file res/data/textures.json
/// </summary>
void readTextures();

/// <summary>
/// Loads all the materials in the file res/data/materials.json
/// </summary>
void readMaterials();

/// <summary>
/// Loads all the FinalModels in the file res/data/finalmodels.json
/// </summary>
void readGameObjects();

/// <summary>
/// <para>Loads all the errors in the specified file. The file must be located within res/data.</para> 
/// </summary>
/// <param name="fileName">Relative file name from res/data</param>
void readErrors();
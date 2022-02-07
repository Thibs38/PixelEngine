#include "JSON.hpp"
#include "Error.hpp"
#include "FileIO.hpp"
#include "rendering/shader.hpp"

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "rapidjson/allocators.h"
#include "rapidjson/encodings.h"


#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>

using namespace naming;

#pragma region Error Management
/// <summary>
/// Formats an error for an element of a JSON Array.
/// </summary>
/// <param name="objectsName">Name of the array</param>
/// <param name="n">Index of the element.</param>
/// <returns>C-style string with the error</returns>
inline std::string formatJSONErrorArray(const std::string& arraysName, int n){ return fmt::format("{}[{:d}]", arraysName, n).c_str(); }

/// <summary>
/// Tests if there were errors while parsing the JSON file and print them in the console
/// </summary>
/// <param name="doc">The document where the parse was done</param>
/// <param name="data">The parsed string</param>
/// <param name="path">The path to the document</param>
/// <param name="printError">Print errors to the console?</param>
/// <returns>True if errors, False otherwise</returns>
bool errorParsingJSON(rapidjson::Document& doc, constring data, constring path, bool printError = true)
{
	bool error = doc.HasParseError();
	if (error)
	{
		// rapidjson does not provide line/column, only character offset
		//hacking it here until (if) rapidjson implements it
		size_t const error_char_offset = doc.GetErrorOffset();
		std::string::const_iterator error_it =
			data.cbegin() + error_char_offset;

		// Compute line number, using 1 as base line
		size_t const line = 1 + std::count_if(
			data.cbegin(),
			error_it,
			std::bind(std::equal_to<char>(), '\n'));

		// Compute column (char offset into line), using 1 as base column
		std::string::const_reverse_iterator reverse_error_it{ error_it };
		auto error_line_begin_it = std::find(
			reverse_error_it,
			data.crend(),
			'\n');

		// If this is the first line we can
		size_t const column =
			error_line_begin_it != data.crend()
			? std::distance(reverse_error_it, error_line_begin_it) + 1
			: error_char_offset + 1;

		if (printError)
			ErrorManager::printJSONErrorLine(JSONError::CANT_PARSE, path, rapidjson::GetParseError_En(doc.GetParseError()), line, column);
		else
			ErrorManager::printError("JSONError", "CAN'T PARSE", path, "Couldn't parse the file", rapidjson::GetParseError_En(doc.GetParseError()), line, column);
	}
	return error;
}

template<class T>
inline std::string defaultValueFormat(T defaultValue) { return fmt::format("{} will be used as a default value", defaultValue);  }

//TODO Rewrite documentation for the next 3 functions

/// <summary>
/// Detects if the value exists, if it is a float and if it is in the correct range.
/// </summary>
/// <typeparam name="...T">T = FLOAT! 2 or 3 parameters.</typeparam>
/// <typeparam name="...U">T = FLOAT! 1 or 2 parameters.</typeparam>
/// <param name="value">The value to test.</param>
/// <param name="object">The name of the object containing the tested member.</param>
/// <param name="member">The name of the member tested.</param>
/// <param name="index">The index of the current object.</param>
/// <param name="path">The path of the file.</param>
/// <param name="defaultValue">The default value used if not in range.</param>
/// <param name="comparator">The comparator used for the range of the value.</param>
/// <param name="...comparands">The comparands used by the comparator.</param>
/// <returns>The value if no errors, the default value otherwise.</returns>
template<class ...T>
auto errorParsingJSONFloat (const rapidjson::Value& value, const char* object, const char* member, int index, constring path, const IComparator<T...>& comparator = Always<T...>(), float defaultValue = 0)
{
	if (!value.HasMember(member))
	{
		ErrorManager::printJSONError(JSONError::MISSING_MEMBER, path, defaultValueFormat(defaultValue),
			formatJSONErrorArray(object, index), member);
		return defaultValue;
	}

	bool isFloat = value[member].IsFloat();
	bool isInt = value[member].IsInt();
	if (!isFloat && !isInt)
	{
		ErrorManager::printJSONError(JSONError::WRONG_TYPE, path, defaultValueFormat(defaultValue),
			formatJSONErrorArray(object, index), member, float_s);
		return defaultValue;
	}
	
	float v = isFloat? value[member].GetFloat() : value[member].GetInt();
	if (!comparator.compare(v))
	{
		ErrorManager::printJSONError(JSONError::WRONG_VALUE, path, defaultValueFormat(defaultValue),
			formatJSONErrorArray(object, index), member, comparator.printMessage());
		return defaultValue;
	}
	return value[member].GetFloat();
	
	
}

/// <summary>
/// Detects if the value exists, if it is an int and if it is in the correct range.
/// </summary>
/// <typeparam name="...T">T = FLOAT! 2 or 3 parameters.</typeparam>
/// <typeparam name="...U">T = FLOAT! 1 or 2 parameters.</typeparam>
/// <param name="value">The value to test.</param>
/// <param name="object">The name of the object containing the tested member.</param>
/// <param name="member">The name of the member tested.</param>
/// <param name="index">The index of the current object.</param>
/// <param name="path">The path of the file.</param>
/// <param name="defaultValue">The default value used if not in range.</param>
/// <param name="comparator">The comparator used for the range of the value.</param>
/// <param name="...comparands">The comparands used by the comparator.</param>
/// <returns>The value if no errors, the default value otherwise.</returns>
template<class ...T>
auto errorParsingJSONInt   (const rapidjson::Value& value, const char* object, const char* member, int index, constring path, const IComparator<T...>& comparator = Always<T...>(), int defaultValue = 0)
{
	if (!value.HasMember(member))
	{
		ErrorManager::printJSONError(JSONError::MISSING_MEMBER, path, defaultValueFormat(defaultValue),
			formatJSONErrorArray(object, index), member);
		return defaultValue;
	}

	if (!value[member].IsInt())
	{
		ErrorManager::printJSONError(JSONError::WRONG_TYPE, path, defaultValueFormat(defaultValue),
			formatJSONErrorArray(object, index), member, int_s);
		return defaultValue;
	}

	int v = value[member].GetInt();
	if (!comparator.compare(v))
	{
		ErrorManager::printJSONError(JSONError::WRONG_VALUE, path, defaultValueFormat(defaultValue),
			formatJSONErrorArray(object, index), member, comparator.printMessage());
		return defaultValue;
	}
	return v;
	
}

//SAFE Add String comparison to function and then to each file loaded
//TODO Change IDs by name in JSON and then generate an ID (long)

/// <summary>
/// Detects if the value exists and if it is a string.
/// </summary>
/// <param name="value">The value to test.</param>
/// <param name="object">The name of the object containing the tested member.</param>
/// <param name="member">The name of the member tested.</param>
/// <param name="index">The index of the current object.</param>
/// <param name="path">The path of the file.</param>
/// <returns>The value if no errors, "ERROR" otherwise.</returns>
template<class ...T>
auto errorParsingJSONString(const rapidjson::Value& value, const char* object, const char* member, int index, constring path, const IComparator<T...>& comparator = Always<T...>(), const std::string &defaultValue = "ERROR")
{
	if (!value.HasMember(member))
	{
		ErrorManager::printJSONError(JSONError::MISSING_MEMBER, path, defaultValueFormat(defaultValue),
			formatJSONErrorArray(object, index), member);
		return defaultValue;
	}

	if (!value[member].IsString())
	{
		ErrorManager::printJSONError(JSONError::WRONG_TYPE, path, defaultValueFormat(defaultValue),
			formatJSONErrorArray(object, index), member, int_s);
		return defaultValue;
	}

	return value[member].GetString();
}

#pragma endregion

/// <summary>
/// Parses the file at the given path to the given document. Prints any errors in the console.
/// </summary>
/// <param name="doc">The doc to parse into</param>
/// <param name="path">The path of the file to parse</param>
/// <param name="printError">Print errors to the console?</param>
/// <returns>True if parsing was successful, False otherwise</returns>
bool parseJSON(rapidjson::Document& doc, constring path, bool printError = true)
{
	std::ifstream file;
	if (!openFile(&file, path, printError))             //First we open the file and return false if couldn't be opened
		return false;

	std::string json;
	readFile(file, json);                               //Read the bytes from the file
	doc.Parse(json.c_str());                            //Parsing the file
	return !errorParsingJSON(doc, json, path, printError);
}

#pragma region Shaders
/*
int extractName()
{

}

/// <summary>
/// Extracts the attributes of a given shader code.
/// </summary>
/// <param name="shaderCode">The shader code.</param>
/// <returns> A list of the attributes name in order of appearance</returns>
std::vector<std::string>* extractAttributes(std::string& shaderCode)
{
	std::vector<std::string>* attributes = new std::vector<std::string>();
	char c = 'a';
	
	for (int i = shaderCode.find("//in") + 4; c != '\0'; i++)
	{
		i = skipToNext(shaderCode, i);

		if (c == 'i')
		{
			i += 2;
		}
		else if (c == 'v')
		{
			i += 7;
		}
		else
		{
			//SAFE Error in shader formatting
		}



		if (c == ' ' || c == ';' || c == '\n' || c == '\t')
		{
			bool isIn = shaderCode[i + 1] == 'i' && shaderCode[i + 2] == 'n' && shaderCode[i + 3] == ' ';
			if (isIn)
			{
				i += 4;
				c = shaderCode[i];
				while (c == ' ' || c == '\t' || c == '\n')
				{
					i++;
					c = shaderCode[i];
				}

				int j = i;
				c = shaderCode[j];
				while (c != ' ' || c != ';')
				{
					j++;
					c = shaderCode[j];
				}
				attributes->push_back(shaderCode.substr(i, j - i));
				i = j;
			}
			else
			{
				bool isVarying = 
					shaderCode[++i] == 'v' && 
					shaderCode[++i] == 'a' && 
					shaderCode[++i] == 'r' &&
					shaderCode[++i] == 'y' &&
					shaderCode[++i] == 'i' &&
					shaderCode[++i] == 'n' &&
					shaderCode[++i] == 'g' &&
					shaderCode[++i] == ' ';
				if (isVarying)
				{
					i++;
					int j = i;
					c = shaderCode[j];
					while (c != ' ' || c != ';')
					{
						j++;
						c = shaderCode[j];
					}
					attributes->push_back(shaderCode.substr(i, j - i));
					i = j;
				}
			}
			
		}
	}
}
*/

/// <summary>
/// Loads all the vertex shaders in the file res/data/vertexshaders.json
/// </summary>
void readVertexShaders()
{
	const std::string path = "res/data/vertexshaders.json";
	rapidjson::Document doc;

	if (!parseJSON(doc, path))
		return;

	if (!doc.IsArray()) //We should have an array of raw models
	{
		ErrorManager::printJSONError(JSONError::WRONG_ROOT, path, "", array_s);
		return;
	}

	for (int i = 0; i < doc.Size(); i++)
	{
		int id;
		std::string name;
		std::string fileName; //SAFE must finish by .vert

		const rapidjson::Value& value = doc[i]; 
		
		id = errorParsingJSONInt(value, vertex_s, id_s, i, path, GreaterEqualThan{ 0 }, i);
		name = errorParsingJSONString(value, vertex_s, name_s, i, path);

		fileName = errorParsingJSONString(value, vertex_s, path_s, i, path);

		std::string shaderCode;

		//TODO Read shader code

		uint vertexShader = loadShader(fileName, ShaderType::VERTEX_SHADER, shaderCode);

		if (vertexShader > 0) //Error management done in loadShader()
		{
			new VertexShader(id, name, vertexShader);
		}
	}

}

/// <summary>
/// Loads all the geometry shaders in the file res/data/geometryshaders.json
/// </summary>
void readGeometryShaders()
{
	const std::string path = "res/data/geometryshaders.json";
	rapidjson::Document doc;

	if (!parseJSON(doc, path))
		return;

	if (!doc.IsArray()) //We should have an array of raw models
	{
		ErrorManager::printJSONError(JSONError::WRONG_ROOT, path, "", array_s);
		return;
	}

	for (int i = 0; i < doc.Size(); i++)
	{
		int id;
		std::string name;
		std::string fileName; //SAFE must finish by .geom ??

		const rapidjson::Value& value = doc[i];

		id = errorParsingJSONInt(value, geometry_s, id_s, i, path, GreaterEqualThan{ 0 }, i);

		name = errorParsingJSONString(value, geometry_s, name_s, i, path);

		fileName = errorParsingJSONString(value, geometry_s, path_s, i, path);

		std::string shaderCode;

		//TODO Read shader code

		uint geometryShader = loadShader(fileName, ShaderType::GEOMETRY_SHADER, shaderCode);
		if (geometryShader > 0)
		{
			new ShaderBase(id, name, geometryShader);
		}
	}
}

/// <summary>
/// Loads all the fragment shaders in the file res/data/fragmentshaders.json
/// </summary>
void readFragmentShaders()
{
	const std::string path = "res/data/fragmentshaders.json";
	rapidjson::Document doc;

	if (!parseJSON(doc, path))
		return;

	if (!doc.IsArray()) //We should have an array of raw models
	{
		ErrorManager::printJSONError(JSONError::WRONG_ROOT, path, "", array_s);
		return;
	}

	for (int i = 0; i < doc.Size(); i++)
	{
		int id;
		std::string name;
		std::string fileName; //SAFE must finish by .frag

		const rapidjson::Value& value = doc[i];

		id = errorParsingJSONInt(value, fragment_s, id_s, i, path, GreaterEqualThan{ 0 }, i);

		name = errorParsingJSONString(value, fragment_s, name_s, i, path);

		fileName = errorParsingJSONString(value, fragment_s, path_s, i, path);

		std::string shaderCode;

		//TODO Read shader code

		uint fragmentShader = loadShader(fileName, ShaderType::FRAGMENT_SHADER, shaderCode);
		if (fragmentShader > 0)
		{
			new ShaderBase(id, name, fragmentShader);
		}
	}
}

void readShaders()
{
	//First things first, we load all the scripts
	readVertexShaders();
	readGeometryShaders();
	readFragmentShaders();

	printf("Loaded %d vertex shaders, %d geometry shaders, %d fragment shaders\n", 
		(int)VertexShader::vertexShaders.size(),
		(int)GeometryShader::geometryShaders.size(),
		(int)FragmentShader::fragmentShaders.size());


	//Then we link shaders together
	const std::string path = "res/data/shaders.json";
	rapidjson::Document doc;

	if (!parseJSON(doc, path))
		return;

	if (!doc.IsArray()) //We should have an array of raw models
	{
		ErrorManager::printJSONError(JSONError::WRONG_ROOT, path, "", array_s);
		return;
	}

	for (int i = 0; i < doc.Size(); i++)
	{
		int id;
		std::string name;
		int vertexID;
		int geometryID; //TODO Add the possibility to specify the Shaders by their name
		int fragmentID;

		const rapidjson::Value& value = doc[i];

		id = errorParsingJSONInt(value, shader_s, id_s, i, path, GreaterEqualThan{ 0 }, i);

		name = errorParsingJSONString(value, shader_s, name_s, i, path);

		vertexID = errorParsingJSONInt(value, shader_s, vertex_s, i, path, GreaterEqualThan{ 0 }, -1);

		geometryID = errorParsingJSONInt(value, shader_s, geometry_s, i, path, GreaterEqualThan{ -1 }, -1);

		fragmentID = errorParsingJSONInt(value, shader_s, fragment_s, i, path, GreaterEqualThan{ 0 }, -1);

		//TODO Display message saying that one or two are missing
		if (vertexID == -1 || fragmentID == -1) continue; //We skip the shader if vertex or fragment is missing

		//SAFE Verify if vertexID, geometryID and fragmentID are in range
		uint programID = linkShaders(id, name,
							  VertexShader::vertexShaders[vertexID],
			geometryID >= 0 ? GeometryShader::geometryShaders[geometryID] : nullptr,
							  FragmentShader::fragmentShaders[fragmentID]);
		
		if (programID == 0) continue; //Error managed in linkShaders()
		
		int attribCount;

		glGetProgramiv(programID, GL_ACTIVE_ATTRIBUTES, &attribCount);
		printf("Active Attributes: %d\n", attribCount);
		//TODO Get attrib and uniforms
		/*
		for (i = 0; i < attribCount; i++)
		{
			glGetActiveAttrib(programID, (GLuint)i, bufSize, &length, &size, &type, name);

			printf("Attribute #%d Type: %u Name: %s\n", i, type, name);
		}*/

	}

	//Destroying the temporary sub Shaders
	VertexShader::destroy();
	GeometryShader::destroy();
	FragmentShader::destroy();


	printf("Loaded %d shaders\n",
		(int)Shader::shaders.size());
}

#pragma endregion

#pragma region Components

void readComponents()
{
	readRenderers();
}

void readRenderers()
{
	const std::string path = "res/data/renderers.json";
	rapidjson::Document doc;

	if (!parseJSON(doc, path))
		return;

	if (!doc.IsArray()) //We should have an array of raw models
	{
		ErrorManager::printJSONError(JSONError::WRONG_ROOT, path, "", array_s);
		return;
	}

	for (int i = 0; i < doc.Size(); i++)
	{
		int id;
		std::string name;

		const rapidjson::Value& value = doc[i];

		id = errorParsingJSONInt(value, texture_s, id_s, i, path, GreaterEqualThan{ 0 }, i);

		name = errorParsingJSONString(value, texture_s, name_s, i, path);

		//fileName = errorParsingJSONString(value, texture_s, path_s, i, path);

	}

	printf("Loaded %d textures\n", (int)Texture::textures.size());
}

#pragma endregion

#pragma region Models

void readTextures()
{
	const std::string path = "res/data/textures.json";
	rapidjson::Document doc;

	if (!parseJSON(doc, path))
		return;

	if (!doc.IsArray()) //We should have an array of raw models
	{
		ErrorManager::printJSONError(JSONError::WRONG_ROOT, path, "", array_s);
		return;
	}

	for (int i = 0; i < doc.Size(); i++)
	{
		int id;
		std::string name;
		std::string fileName; //SAFE must finish by .png or .jpg or other supported format

		const rapidjson::Value& value = doc[i];

		id = errorParsingJSONInt(value, texture_s, id_s, i, path, GreaterEqualThan{ 0 }, i);

		name = errorParsingJSONString(value, texture_s, name_s, i, path);

		fileName = errorParsingJSONString(value, texture_s, path_s, i, path);

		readTexture(id, name, fileName);
	}

	printf("Loaded %d textures\n", (int)Texture::textures.size());
}

void readMaterials()
{
	const std::string path = "res/data/materials.json";
	rapidjson::Document doc;

	if (!parseJSON(doc, path))
		return;

	if (!doc.IsArray()) //We should have an array of materials
	{
		ErrorManager::printJSONError(JSONError::WRONG_ROOT, path, "", array_s);
		return;
	}

	for (int i = 0; i < doc.Size(); i++)
	{
		int id;
		std::string name;
		int shaderID;

		const rapidjson::Value& mat = doc[i];

		id = errorParsingJSONInt(mat, material_s, id_s, i, path, GreaterEqualThan{ 0 }, i);

		name = errorParsingJSONString(mat, material_s, name_s, i, path);

		shaderID = errorParsingJSONInt(mat, material_s, shader_s, i, path, GreaterEqualThan{ 0 }, 0);

		new Material(id, name, Shader::shaders[shaderID]);
	}
	printf("Loaded %d materials\n", (int)Material::materials.size());
}

void readGameObjects()
{
	const std::string path = "res/data/gameobjects.json";
	rapidjson::Document doc;

	if (!parseJSON(doc, path))
		return;

	if (!doc.IsArray()) //We should have an array of raw models
	{
		ErrorManager::printJSONError(JSONError::WRONG_ROOT, path, "", array_s);
		return;
	}

	for (int i = 0; i < doc.Size(); i++)
	{
		int id;
		std::string name;
		float posX;
		float posY;
		float zIndex;
		float rotation;
		

		const rapidjson::Value& value = doc[i];

		id = errorParsingJSONInt(value, finalmodel_s, id_s, i, path, GreaterEqualThan{ 0 }, i);

		name = errorParsingJSONString(value, finalmodel_s, name_s, i, path);

		posX = errorParsingJSONFloat(value, finalmodel_s, rawmodel_s, i, path, GreaterEqualThan{ 0 }, 0);

		//texture = errorParsingJSONInt(value, finalmodel_s, texture_s, i, path, 0, GreaterEqualThan{ 0 });

		//material = errorParsingJSONInt(value, finalmodel_s, material_s, i, path, 0, GreaterEqualThan{ 0 });

		GameObject* gameObject = new GameObject(id, name);
		Transform transform = gameObject->transform;
		//transform.position
	}
	printf("Loaded %d GameObjects\n", (int)GameObject::gameobjects.size());
}

#pragma endregion

void readErrors()
{
	std::string path = "res/data/errors.json";
	rapidjson::Document doc;

	if (!parseJSON(doc, path, false))
		return;

	if (!doc.IsObject())
	{
		ErrorManager::printError("JSON ERROR", "WRONG ROOT", "The root of the file must be an {}", "", "Object");
		return;
	}

	//This method is messy because it has to print the errors manually

	int i = 0;
	for (rapidjson::Value::ConstMemberIterator itr = doc.MemberBegin();
		itr != doc.MemberEnd(); ++itr)
	{
		const rapidjson::Value& errorType = itr->value;
		if (!errorType.IsArray())
		{
			ErrorManager::printError("JSON ERROR", "WRONG TYPE",
				"The member {} of the object {} has a wrong type. The expected type is {}", "",
				itr->name.GetString(), "Root", "object");
			continue;
		}

		for (int j = 0; j < errorType.Size(); j++)
		{
			int id;
			std::string name;
			std::string message;

			const rapidjson::Value& error = errorType[j];

			if (!error.IsObject())
			{
				ErrorManager::printError("JSON ERROR", "WRONG TYPE",
					"The member {} of the object {} has a wrong type. The expected type is {}", "",
					fmt::format("{}[{:d}]", itr->name.GetString(), j), "Object");
				continue;
			}

			id = error[id_s].GetInt();              //No error management for clarity, be careful
			name = error[name_s].GetString();
			message = error[message_s].GetString();

			switch (i)
			{
			case 0:
				new IOErrorHolder((IOError)id, name, message);
				break;
			case 1:
				new JSONErrorHolder((JSONError)id, name, message);
				break;
			case 2:
				new ShaderErrorHolder((ShaderError)id, name, message);
				break;
			default:
				std::cerr << "Undefined Error Type" << std::endl;
				break;
			}

		}
		i++;
	}
}
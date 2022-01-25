#pragma once

#include "util/Utility.hpp"
#include "util/Color.hpp"
#include "Shader.hpp"

#include <string>
#include <unordered_map>
#include "glm/vec2.hpp"

/* This class contains all the declaration of structs and classes related to the GameObjects.
 * GameObjects consists of a Transform, a name, an ID (which is for now manually assigned through JSON),
 * and a list of components.
 * 
 * Components are stored in giant "lists" and reference their GameObject. They cannot live without being 
 * attached to a GameObject, and it cannot be changed after instantiation. Thus, once instantiated, they
 * immediately take effect on the GameObject. The "lists" can vary depending on the needs of the Component.
 * 
 * For example:
 * Rigidbodies are used by the Physics Engine one by one without a particular order, so a Vector is used.
 * Renderers however are grouped by their Shaders and their Texture to batch the rendering, Shaders are
 * loaded once, and Textures once too. (Isn't implemented yet)
 */

//TODO static methods to destroy all of the references of these classes properly



#pragma region Components

enum class ComponentType
{
	RENDERER
};

/// <summary>
/// The base class for all Components. Components are added to GameObjects only by instantiating them
/// </summary>
struct Component
{
	const uint id;          //Is unique for each type of component (for now defined through JSON)
	GameObject& gameObject; //Reference to the GameObject that has this component
	bool enabled = true;    //If disabled, the component will be skipped during it's processing
		
	Component(uint id, GameObject& gameObject) :
		id{ id }, gameObject{ gameObject }{}
};

/// <summary>
/// The main Rendering component for all sprites.
/// </summary>
struct Renderer : Component
{
	Texture* texture;     //Pointer to the texture rendered (defaults to blank square if null)
	Material* material;   //Pointer to the material used by the renderer (defaults to default material)

	//TODO replace by map<shader, map<Texture,vector<gameObjects>>
	static std::vector<Renderer&> renderers; //Static list of reference to all renderers	
	
	Renderer(uint id, GameObject& gameObject, Texture* texture, Material* material);
};

#pragma endregion

struct Transform
{
	glm::vec2 position;
	float zIndex;
	float rotation;
	float scale;

};

/// <summary>
/// Represents any object in the game. It has a transform, and then components.
/// </summary>
struct GameObject
{
	const uint id;				//Is unique for all GameObjects
	std::string name;		    //The public name displayed in the editor
	Transform& transform;		//Reference to the transform attached to the GameObject

	static std::vector<GameObject&> gameobjects; //Static list of reference of all GameObjects
	std::vector<Component&> components; //TODO Change for a map<Enum Type, Component>

	GameObject(uint id, std::string name);

	//TODO replace by template<typename T> 	(or Type)
	Component& getComponent(uint position);

};


//TODO It should be possible to Load and Display 3D models
/// <summary>
/// Represents Raw informations about a 2d model
/// </summary>
struct RawModel
{
	const uint id;
	std::string name;
	const uint vaoID;
	const uint iboID;
	const uint vertexCount;

	static RawModel* quad;

	RawModel(uint id, std::string name, uint vaoID, uint iboID, uint vertexCount);	
	//Quad used to draw basically everything
	static void generateQuad();
};

struct Texture
{
	const uint id;          //Unique ID
	std::string name;       //Name displayed in the editor
	const uint textureID;   //ID of the texture in video RAM

	static std::vector<Texture&> textures;  //Static vector of references of all of the textures

	Texture(uint id, std::string name, uint textureID);
};

//TODO Content must be dynamically added based on the shader attributes and uniforms
//TODO Create default Material
struct Material
{
	const uint id;          //Unique id
	std::string name;       //Name displayed in the editor
	Shader& shader;         //Reference of the Shader used by this material

	//Example of property that could be used in a Material:
	//Color color;          //Color used by the Renderer on top of the texture (

	static std::vector<Material&> materials; //Static vector of references of all of the materials

	Material(uint id, std::string name, Shader& shader);
};







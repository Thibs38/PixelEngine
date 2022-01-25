#include "Model.hpp"
#include "Loader.hpp"
#include "io/FileIO.hpp"

#pragma region RawModel

RawModel::RawModel(uint id, std::string name, uint vaoID, uint iboID, uint vertexCount) :
	id(id), name(name), vaoID(vaoID), iboID(iboID), vertexCount(vertexCount) {}

void RawModel::generateQuad()
{
	float positions[8] = {
		0, 0,
		0, 1,
		1, 1,
		1, 0
	};

	uint indices[8] = {
		0, 2, 1,
		0, 3, 2
	};

	float texCoords[8] = {
		0, 0,
		0, 1,
		1, 1,
		1, 0
	};

	RawModel::quad = Loader::loadToVao(0, "quad", positions, indices, texCoords, 6);
}
#pragma endregion

#pragma region Texture

std::vector<Texture&> Texture::textures;

Texture::Texture(uint id, std::string name, uint textureID) :
	id(id), name(name), textureID(textureID)
{
	Texture::textures.push_back(*this);
}

#pragma endregion

#pragma region Material

std::vector<Material&> Material::materials;
	
Material::Material(uint id, std::string name, Shader& shader) :
	id(id), name(name), shader(shader)
{
	Material::materials.push_back(*this);
}

#pragma endregion

#pragma region GameObject

std::vector<GameObject&> GameObject::gameobjects;

GameObject::GameObject(uint id, std::string name) :
	id(id), name(name)
{
	GameObject::gameobjects.push_back(*this);
	transform = *(new Transform());
}

Component& GameObject::getComponent(uint position)
{
	return components[position];
}


#pragma endregion

std::vector<Renderer&> Renderer::renderers;

Renderer::Renderer(uint id,GameObject& gameObject, Texture* texture, Material* material) :
	Component{ id, gameObject }, texture{ texture }, material{ material }
{
	renderers.push_back(*this);
	gameObject.components.push_back(*this);
}



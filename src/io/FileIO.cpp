#include "FileIO.hpp"
#include "rendering/Loader.hpp"
#include "Error.hpp"

#include "stb_image.h"

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <functional>

using namespace naming;


/// <summary>
/// Extracts a number from the given text.
/// </summary>
/// <param name="text">The text.</param>
/// <param name="start">The starting characters' position in the text.</param>
/// <param name="buffer">The buffer where the number will be written into.</param>
/// <param name="bufferSize">The size of the buffer.</param>
/// <returns>The position of the character after the number.</returns>
uint extractNumber(char* text, unsigned int start, char* buffer, int bufferSize)
{
	int i = 0;
	unsigned int k = start;
	while (text[k] != '\0' || text[k] != ' ' && i < bufferSize)
	{
		buffer[i] = text[k];
		i++;
		k++;
	}
	if (buffer[i] != '\0')
		buffer[i] = '\0';
	return k;
}


bool openFile(std::ifstream *file, constring path, bool printError)
{
	file->open(path, std::ios::in);
	if (file->fail())
	{
		if (printError)
			ErrorManager::printIOError(IOError::CANT_OPEN_FILE, path, strerror(errno));
		else
			ErrorManager::printError("[IOERROR]", "CAN'T OPEN FILE", "Couldn't open the file", path, strerror(errno));
		return false;
	}
	return true;
}

void readFile(std::ifstream& stream, std::string& content)
{
	std::string buf(4096, '\0');
	while (stream.read(&buf[0], 4096))
	{
		content.append(buf, 0, stream.gcount());
	}
	content.append(buf, 0, stream.gcount());
}


void readTexture(uint id, constring name, constring fileName)
{
	int width, height, nrChannels;
	std::string path = "res/textures/" + fileName;
	uint8* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (data == NULL)
	{
		std::cerr << "Couldn't open the file: " << path << std::endl;
	}
	Texture* tex = Loader::loadTexture(id, name, data, width, height);
	stbi_image_free(data);
}



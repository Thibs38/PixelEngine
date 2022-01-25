#include "Error.hpp"
#include "FileIO.hpp"
#include "JSON.hpp"

namespace naming //OPTI Do not keep these in memory, they are only needed on loading (excepted error_s, for that move elsewhere)
{
	extern const char* id_s   = "id";
	extern const char* name_s = "name";
	extern const char* path_s = "path";

	extern const char* rawmodel_s   = "rawmodel";
	extern const char* texture_s    = "texture";
	extern const char* finalmodel_s = "finalmodel";

	extern const char* message_s = "message";

	extern const char* error_s   = "ERROR";      

	extern const char* int_s    = "Int";
	extern const char* string_s = "String";
	extern const char* float_s  = "Float";
	extern const char* array_s  = "Array";
	extern const char* object_s = "Object";

	extern const char* material_s     = "material";
	extern const char* shineDamper_s  = "shineDamper";
	extern const char* reflectivity_s = "reflectivity";

	extern const char* vertex_s      = "vertex";
	extern const char* fragment_s    = "fragment";
	extern const char* geometry_s    = "geometry";
	extern const char* shader_s        = "shader";
	extern const char* attribcount_s = "attribcount";
}

Error::Error(const uint id, const std::string name, const std::string message) :
	id(id), name(name), message(message){}


std::vector<const IOErrorHolder*> IOErrorHolder::errors;
const std::string IOErrorHolder::type = "IO ERROR";
IOErrorHolder::IOErrorHolder(IOError id, const std::string name, const std::string message) :
	Error((uint)id, name, message)
{
	errors.push_back(this);
}

std::vector<const JSONErrorHolder*> JSONErrorHolder::errors;
const std::string JSONErrorHolder::type = "JSON ERROR";
JSONErrorHolder::JSONErrorHolder(JSONError id, const std::string name, const std::string message) :
	Error((uint)id, name, message)
{
	errors.push_back(this);
}

std::vector<const ShaderErrorHolder*> ShaderErrorHolder::errors;
const std::string ShaderErrorHolder::type = "SHADER ERROR";
ShaderErrorHolder::ShaderErrorHolder(ShaderError id, const std::string name, const std::string message) :
	Error((uint)id, name, message)
{
	errors.push_back(this);
}

const fmt::text_style ErrorManager::errorStyle = fg(fmt::color::crimson);
void ErrorManager::init()
{
	readErrors();
}

void ErrorManager::destroy()
{
	IOErrorHolder::errors.clear();
	JSONErrorHolder::errors.clear();
	ShaderErrorHolder::errors.clear();
}




#pragma once
#define FMT_HEADER_ONLY

#include "util/Utility.hpp"

#include <fmt/core.h>
#include <fmt/color.h>

#include <vector>

namespace naming //OPTI Do not keep these in memory, they are only needed on loading
{
	extern const char* id_s;
	extern const char* name_s;
	extern const char* path_s;

	extern const char* rawmodel_s;
	extern const char* texture_s;
	extern const char* finalmodel_s;

	extern const char* message_s;

	extern const char* error_s;

	extern const char* int_s;
	extern const char* string_s;
	extern const char* float_s;
	extern const char* array_s;
	extern const char* object_s;

	extern const char* material_s;
	extern const char* shineDamper_s;
	extern const char* reflectivity_s;

	extern const char* vertex_s;
	extern const char* fragment_s;
	extern const char* geometry_s;
	extern const char* shader_s;
	extern const char* attribcount_s;
}

#pragma region Enums

enum class IOError
{
	CANT_OPEN_FILE = 0   //Couldn't open the specified file.

};

enum class JSONError
{
	CANT_PARSE = 0,      //Couldn't parse the specified file.
	MISSING_MEMBER = 1,  //The object is missing a member.
	WRONG_TYPE = 2,      //The value of the member has an incorrect type.
	WRONG_VALUE = 3,     //The value of the member is not correct.
	WRONG_ROOT = 4,      //The root of the file is not correct. (either an array or an object)
};

enum class ShaderError
{
	CANT_COMPILE_VERTEX = 0,
	CANT_COMPILE_FRAGMENT = 1,
	CANT_COMPILE_GEOMETRY = 2,
	CANT_COMPILE_COMPUTE = 3,
	CANT_LINK = 4,
	//ATTRIBUTE_COUNTS_DONT_MATCH = 5 "The attribute count of {} is {} and the count of {} is {}, they have to match." (maybe already done in link)
};

#pragma endregion

//OPTI Do not keep loading errors in memory after the loading

#pragma region Error Classes

/// <summary>
/// Represents an Error.
/// </summary>
struct Error
{
	const uint id;    //Id of the error of the specific type.
	const std::string name;  
	const std::string message;
	
	/// <summary>
	/// Initializes a new <see cref="Error"/>.
	/// </summary>
	/// <param name="id">The id of the error of the specific type.</param>
	/// <param name="name">The name of the error.</param>
	/// <param name="message">The message of the error.</param>
	Error(const uint id, const std::string name, const std::string message);
};

/// <summary>
/// Represents an IO error.
/// </summary>
/// <seealso cref="Error" />
struct IOErrorHolder : Error
{
	static std::vector<const IOErrorHolder*> errors;
	static const std::string type;
	IOErrorHolder(IOError id, const std::string name, const std::string message);
};

/// <summary>
/// Represents a JSON error.
/// </summary>
/// <seealso cref="Error" />
struct JSONErrorHolder : Error
{
	static std::vector<const JSONErrorHolder*> errors;
	static const std::string type;
	
	JSONErrorHolder(JSONError id, const std::string name, const std::string message);
};

/// <summary>
/// Represents a Shader error.
/// </summary>
/// <seealso cref="Error" />
struct ShaderErrorHolder : Error
{
	static std::vector<const ShaderErrorHolder*> errors;
	static const std::string type;

	ShaderErrorHolder(ShaderError id, const std::string name, const std::string message);
};


#pragma endregion


class ErrorManager
{
public:

	static const fmt::text_style errorStyle;
	
	/// <summary>
	/// Initializes the errors.
	/// </summary>
	static void init();
	
	/// <summary>
	/// Destroys all the errors.
	/// </summary>
	static void destroy();

	/// <summary>
	/// Prints an error to the standard error stream.
	/// </summary>
	/// <typeparam name="...Types">Anything that can be used as argument for the message.</typeparam>
	/// <param name="errorType">The type of the error.</param>
	/// <param name="name">The name of the error.</param>
	/// <param name="message">The message of the error.</param>
	/// <param name="additionalMessage">An additional message to better describe the error.</param>
	/// <param name="...args">The arguments that can be passed to the message.</param>
	template<class ... Types>
	static void printError(constring errorType, constring name, constring message, constring additionalMessage, Types ...args);

	/// <summary>
	/// Prints an error to the standard error stream.
	/// </summary>
	/// <typeparam name="...Types">Anything that can be used as argument for the message.</typeparam>
	/// <param name="errorType">The type of the error.</param>
	/// <param name="name">The name of the error.</param>
	/// <param name="path">The path where the error occured.</param>
	/// <param name="message">The message of the error.</param>
	/// <param name="additionalMessage">An additional message to better describe the error.</param>
	/// <param name="...args">The arguments that can be passed to the message.</param>
	template<class ... Types>
	static void printError(constring errorType, constring name, constring path, constring message, constring additionalMessage, Types... args);

	/// <summary>
	/// Prints an error to the standard error stream.
	/// </summary>
	/// <typeparam name="...Types">Anything that can be used as argument for the message.</typeparam>
	/// <param name="errorType">The type of the error.</param>
	/// <param name="name">The name of the error.</param>
	/// <param name="path">The path of the file where the error occured.</param>
	/// <param name="line">The line where the error occured.</param>
	/// <param name="message">The message of the error.</param>
	/// <param name="additionalMessage">An additional message to better describe the error.</param>
	/// <param name="...args">The arguments that can be passed to the message.</param>
	template<class ... Types>
	static void printError(constring errorType, constring name, constring path, constring message, constring additionalMessage, int line, Types... args);

	/// <summary>
	/// Prints an error to the standard error stream.
	/// </summary>
	/// <typeparam name="...Types">Anything that can be used as argument for the message.</typeparam>
	/// <param name="errorType">The type of the error.</param>
	/// <param name="name">The name of the error.</param>
	/// <param name="path">The path of the file where the error occured.</param>
	/// <param name="line">The line where the error occured.</param>
	/// <param name="column">The column where the error occured.</param>
	/// <param name="message">The message of the error.</param>
	/// <param name="additionalMessage">An additional message to better describe the error.</param>
	/// <param name="...args">The arguments that can be passed to the message.</param>
	template<class ... Types>
	static void printError(constring errorType, constring name, constring path, constring message, constring additionalMessage, int line, int column, Types... args);

	/// <summary>
	/// Prints an error related to IO. 
	/// </summary>
	/// <typeparam name="...Types">Anything that can be used as argument for the message.</typeparam>
	/// <param name="error">The error type.</param>
	/// <param name="path">The path to the file that caused this error.</param>
	/// <param name="additionalMessage">An additional message to better describe the error.</param>
	/// <param name="...args">The arguments passed to the message.</param>
	template<class ... Types>
	static void printIOError(IOError error, constring path, constring additionalMessage, Types... args);

	/// <summary>
	/// Prints an error related to JSON. 
	/// </summary>
	/// <typeparam name="...Types">Anything that can be used as argument for the message.</typeparam>
	/// <param name="error">The error type.</param>
	/// <param name="path">The path to the file that caused this error.</param>
	/// <param name="additionalMessage">An additional message to better describe the error.</param>
	/// <param name="...args">The arguments passed to the message.</param>
	template<class ... Types>
	static void printJSONError(JSONError error, constring path, constring additionalMessage, Types... args);

	/// <summary>
	/// Prints an error related to JSON. 
	/// </summary>
	/// <typeparam name="...Types">Anything that can be used as argument for the message.</typeparam>
	/// <param name="error">The error type.</param>
	/// <param name="path">The path to the file that caused this error.</param>
	/// <param name="line">The line where the error occured.</param>
	/// <param name="column">The column where the error occured.</param>
	/// <param name="additionalMessage">An additional message to better describe the error.</param>
	/// <param name="...args">The arguments passed to the message.</param>
	template<class ... Types>
	static void printJSONErrorLine(JSONError error, constring path, constring additionalMessage, int line, int column, Types... args);

	/// <summary>
	/// Prints an error related to a Shader. 
	/// </summary>
	/// <typeparam name="...Types">Anything that can be used as argument for the message.</typeparam>
	/// <param name="error">The error type.</param>
	/// <param name="path">The path to the file that caused this error.</param>
	/// <param name="additionalMessage">An additional message to better describe the error.</param>
	/// <param name="...args">The arguments passed to the message.</param>
	template<class ... Types>
	static void printShaderError(ShaderError error, constring path, constring additionalMessage, Types... args);

	/// <summary>
	/// Prints an error related to a Shader. 
	/// </summary>
	/// <typeparam name="...Types">Anything that can be used as argument for the message.</typeparam>
	/// <param name="error">The error type.</param>
	/// <param name="additionalMessage">An additional message to better describe the error.</param>
	/// <param name="...args">The arguments passed to the message.</param>
	template<class ... Types>
	static void printShaderError(ShaderError error, constring additionalMessage, Types... args);

};

#pragma region printError

template<class ...Types>
void ErrorManager::printError(constring errorType, constring name, constring message, constring additionalMessage, Types ...args)
{
	fmt::print(stderr, errorStyle, "[{}] {}: \n" + message + "\n{}\n===========\n",
		errorType, name, args..., additionalMessage);
}

template<class ...Types>
void ErrorManager::printError(constring errorType, constring name, constring path, constring message, constring additionalMessage, Types ...args)
{
	fmt::print(stderr, errorStyle, "[{}] {}, at \"{}\": \n" + message + "\n{}\n===========\n",
		errorType, name, path, args..., additionalMessage);
}

template<class ...Types>
void ErrorManager::printError(constring errorType, constring name, constring path, constring message, constring additionalMessage, int line, Types ...args)
{
	fmt::print(stderr, errorStyle, "[{}] {}, at \"{}\": line {:d}:\n" + message + "\n{}\n===========\n",
		errorType, name, path, line, args..., additionalMessage);
}

template<class ...Types>
void ErrorManager::printError(constring errorType, constring name, constring path, constring message, constring additionalMessage, const int line, const int column, Types ...args)
{
	fmt::print(stderr, errorStyle, "[{}] {}, at \"{}\": L {:d}; C {:d}:\n" + message + "\n{}\n===========\n",
		errorType, name, path, line, column, args..., additionalMessage);
}

#pragma endregion


#pragma region printIOError

template<class ...Types>
void ErrorManager::printIOError(IOError error, constring path, constring additionalMessage, Types ...args)
{
	const IOErrorHolder* err = IOErrorHolder::errors[(int)error];
	printError(IOErrorHolder::type, err->name, path, err->message, additionalMessage, args...);
}

#pragma endregion

#pragma region printJSONError

template<class ...Types>
void ErrorManager::printJSONError(JSONError error, constring path, constring additionalMessage, Types ...args)
{
	const JSONErrorHolder* err = JSONErrorHolder::errors[(int)error];
	printError(JSONErrorHolder::type, err->name, path, err->message, additionalMessage, args...);
}

template<class ...Types>
void ErrorManager::printJSONErrorLine(JSONError error, constring path, constring additionalMessage, const int line, const int column, Types ...args)
{
	const JSONErrorHolder* err = JSONErrorHolder::errors[(int)error];
	printError(JSONErrorHolder::type, err->name, path, err->message, additionalMessage, line, column, args...);
}

#pragma endregion

#pragma region printShaderError

template<class ... Types>
void ErrorManager::printShaderError(ShaderError error, constring path, constring additionalMessage, Types... args)
{
	const ShaderErrorHolder* err = ShaderErrorHolder::errors[(int)error];
	printError(ShaderErrorHolder::type, err->name, path, err->message, additionalMessage, args...);
}

template<class ... Types>
void ErrorManager::printShaderError(ShaderError error, constring additionalMessage, Types... args)
{
	const ShaderErrorHolder* err = ShaderErrorHolder::errors[(int)error];
	printError(ShaderErrorHolder::type, err->name, err->message, additionalMessage, args...);
}
#pragma endregion
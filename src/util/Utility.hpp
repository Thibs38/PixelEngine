#pragma once

#include <string>

typedef const std::string& constring;

typedef unsigned int uint;
typedef unsigned long ulong;
typedef char int8;
typedef unsigned char uint8;

/// <summary>
/// Skip the blank characters of the given string.
/// </summary>
/// <param name="string">The string.</param>
/// <param name="start">The starting character.</param>
/// <returns>The index of the first non blank character.</returns>
int skipToNext(constring string, int start)
{
	char c = string[start];
	while (c == ' ' || c == '\n' || c == '\t')
	{
		c = string[++start];
	}
	return start;
}

bool endsWith(const std::string &fullString, const std::string &ending) {
	if (fullString.length() < ending.length()) return false;

	return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	
}
#pragma once

#include <string>
#include "Utility.hpp"

/* Super Generic header that allows comparisons between multiple type of data.
 * The main motivation is to have a versatil tool to verify the integrity of data.
 * Comparators are for example used to load JSON files to verify that values are in range.
 *
 *
 */


//OPTI Can maybe be optimized by including the value to compare inside of the Comparator (thus not having to pass the value as a paramater of the caller)

//FIXME Fix the comparators

/// <summary>
/// Interface for Comparators, classes that are used to compare values.
/// </summary>
/// <typeparam name="...T"> Any type</typeparam>
template<typename T>
struct IComparator
{	
	/// <summary>
	/// Compares the given value to the ones given at construction.
	/// </summary>
	/// <param name="a">The value to be compared.</param>
	/// <returns> The result of the comparison. </returns>
	virtual const bool compare(T a) const = 0;
	/// <summary>
	/// Returns the message related to the comparison.
	/// </summary>
	/// <returns> The message related to the comparison.</returns>
	virtual const std::string printMessage() const = 0;
	virtual ~IComparator() = default;
};

template<typename T>
struct Always : public IComparator<T>
{
	Always(){}
	const bool compare() const override { return true; }
	const std::string printMessage() const override { return ""; }
};

#pragma region 2 Values Comparators

/// <summary>
/// Uses the equal == operator to compare two values.
/// </summary>
/// <typeparam name="T"> Any type that implements the equal == operator. </typeparam>
template<typename T>
struct Equal : public IComparator<T>
{
	const T b;	
	Equal(T b) :
		b(b) { }	

	const bool compare(T a) const override { return a == b; }	
	const std::string printMessage() const override { return "be equal to " + b; }
};

template<typename T>
struct NotEqual : public IComparator<T>
{
	const T b;

	NotEqual(T b) :
		b(b) { }

	const bool compare(T a) const override { return a != b; }
	const std::string printMessage() const override { return "be different from " + b; }
};

template<typename T>
struct LessThan : public IComparator<T>
{
	const T b;

	LessThan(T b) :
		b(b) { }

	const bool compare(T a) const override { return a < b; }
	const std::string printMessage() const override { return "be less than " + b; }

};

template<typename T>
struct LessEqualThan : public IComparator<T>
{
	const T b;

	LessEqualThan(T b) :
		 b(b) { }

	const bool compare(T b) const override { return a <= b; }
	const std::string printMessage() const override { return "be less or equal to " + b; }

};

template<typename T>
struct GreaterThan : public IComparator<T>
{
	const T b;

	GreaterThan(T b)  :
		b(b) { }

	const bool compare(T b) const override { return a > b; }
	const std::string printMessage() const override { return "be greater than " + b; }

};

template<typename T>
struct GreaterEqualThan : public IComparator<T>
{
	const T b;

	GreaterEqualThan(T b) :
		b(b) { }

	const bool compare(T a) const override { return a >= b; }
	const std::string printMessage() const override { return "be greater or equal to " + b; }

};

#pragma endregion

#pragma region 3 Values Comparators
template<typename T>
struct InRangeInclusive : public IComparator <T>
{
	const T b;
	const T c;
	InRangeInclusive(T b, T c) :
		b(b), c(c) { }

	const bool compare(T a) const override { return b <= a && a <= c; }
	const std::string printMessage() const override { return "be in the range [" + b + ", " + c + "]"; }

};

template<typename T>
struct InRangeExclusive : public IComparator <T>
{
	const T b;
	const T c;
	InRangeExclusive(T b, T c) :
		b(b), c(c) { }

	const bool compare(T a) const override { return b < a && a < c; }
	const std::string printMessage() const override { return "be in the range ]" + b + ", " + c + "["; }

};

#pragma endregion

#pragma String Comparators

struct StartsWith : public IComparator <std::string>
{
	const std::string b;
	StartsWith(std::string b) :
		b(b) { }

	const bool compare(std::string a) const override { return a.rfind(b,0); }
	const std::string printMessage() const override { return "start with \"" + b +"\""; }
};

struct EndsWith : public IComparator <std::string>
{
	const std::string b;
	EndsWith(std::string b) :
		b(b) { }

	const bool compare(std::string a) const override { return endsWith(a, b); }
	const std::string printMessage() const override { return "end with \"" + b + "\""; }
};

#pragma endregion
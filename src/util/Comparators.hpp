#pragma once

#include <string>
/*
//Compares a with b
template<class ...T>
using CompareFunction = bool(*)(T...);

//OPTI Can maybe be optimized by including the value to compare inside of the Comparator (thus not having to pass the value as a paramater of the caller)

template<typename ...T>
struct Comparator
{
	const CompareFunction<T...> compare;
	const std::string message;

	Comparator(const CompareFunction<T...> compare, const std::string message);
};

template<typename T>
extern const Comparator<T,T> equals;
extern const Comparator<float, float> equalsF;
extern const Comparator<int, int> differentThanI;
extern const Comparator<float, float> differentThanF;

extern const Comparator<int, int> lessThanI;
extern const Comparator<float, float> lessThanF;
extern const Comparator<int, int> lessEqualThanI;
extern const Comparator<float, float> lessEqualThanF;

extern const Comparator<int, int> greaterThanI;
extern const Comparator<float, float> greaterThanF;
extern const Comparator<int, int> greaterEqualThanI;
extern const Comparator<float, float> greaterEqualThanF;

extern const Comparator<int, int, int> inRangeInclusiveI;
extern const Comparator<float, float, float> inRangeInclusiveF;
extern const Comparator<int, int, int> inRangeExclusiveI;
extern const Comparator<float, float, float> inRangeExclusiveF;
*/

/* Super Generic header that allows comparisons between multiple type of data.
 * The main motivation is to have a versatil tool to verify the integrity of data.
 * Comparators are for example used to load JSON files to verify that values are in range.
 *
 *
 */

//Compares a with b
template<class ...T>
using CompareFunction = bool(*)(T...);

//FIXME Fix the comparators

//The Comparators

template<typename ...T>
struct IComparator
{
	static virtual const bool compare(T...) = 0;
	static virtual const std::string prinMessage() = 0;
};

template<typename ...T>
struct Equal : Comparator {

	static const bool compare(T a, T b) const override { return a == b; }
	static const std::string printMessage() const override { return "equal to {}"; }

};

template<typename ...T>
struct NotEqual : Comparator {

	static const bool compare(T a, T b) const override { return a != b; }
	static const std::string printMessage() const override { return "not equal to {}"; }

};

template<typename ...T>
struct LessThan : Comparator {

	static const bool compare(T a, T b) const override { return a < b; }
	static const std::string printMessage() const override { return "less than {}"; }

};

template<typename ...T>
struct LessEqualThan : Comparator {

	static const bool compare(T a, T b) const override { return a <= b; }
	static const std::string printMessage() const override { return "less or equal to {}"; }

};

template<typename ...T>
struct GreaterThan : Comparator {

	static const bool compare(T a, T b) const override { return a > b; }
	static const std::string printMessage() const override { return "greater than {}"; }

};

template<typename ...T>
struct GreaterEqualThan : Comparator {

	static const bool compare(T a, T b) const override { return a >= b; }
	static const std::string printMessage() const override { return "greater or equal to {}"; }

};

template<typename ...T>
struct InRangeInclusive : Comparator {

	static const bool compare(T a, T b, T c) const override { return a >= b && a <= c; }
	static const std::string printMessage() const override { return "in the range [{}, {}]"; }

};

template<typename ...T>
struct InRangeExclusive : Comparator {

	static const bool compare(T a, T b, T c) const override { return a > b && a < c; }
	static const std::string printMessage() const override { return "in the range ]{}, {}["; }

};



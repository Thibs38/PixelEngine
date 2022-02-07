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
struct Comparator
{
	const CompareFunction<T...> compare;
	const std::string message;

	Comparator(const CompareFunction<T...> compare, const std::string message):
		compare(compare), message(message){	}

};

template<typename T>
bool equal(T a, T b) { return a == b; }
template<typename T>
bool notEqual(T a, T b) { return a != b; }
template<typename T>
bool lessThan(T a, T b) { return a < b; }
template<typename T>
bool lessEqualThan(T a, T b) { return a <= b; }
template<typename T>
bool greaterThan(T a, T b) { return a > b; }
template<typename T>
bool greaterEqualThan(T a, T b) { return a >= b; }

template<typename T>
bool inRangeInclusive(T a, T b, T c) { return a >= b && a <= c; }
template<typename T>
bool inRangeExclusive(T a, T b, T c) { return a > b && a < c; }


template<typename T>
extern const Comparator<T, T> equal{ &equals<T>, "equal to {}" };
template<typename T>
extern const Comparator<T, T> notEqual{ &differentThan, "different than {}" };
template<typename T>
extern const Comparator<T, T> lessThan{ &lessThan, "less than {}" };
template<typename T>
extern const Comparator<T, T> lessEqualThan{ &lessEqualThan, "less or equal than {}" };
template<typename T>
extern const Comparator<T, T> greaterThan{ &greaterThan<T>, "greater than {}" };
template<typename T>
extern const Comparator<T, T> greaterEqualThan{ &greaterEqualThan<T>, "greater or equal than {}" };

template<typename T>
extern const Comparator<T, T, T> inRangeInclusive{ &inRangeInclusive, "in the range [{}, ]" };
template<typename T>
extern const Comparator<T, T, T> inRangeExclusive{ &inRangeExclusive, "in the range ]{}, [" };





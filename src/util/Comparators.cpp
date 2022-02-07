#include "Comparators.hpp"
/*
bool equals(int a, int b) { return a == b; }

bool equals(float a, float b) { return a == b; }

bool differentThan(int a, int b) { return a != b; }

bool differentThan(float a, float b) { return a != b; }

bool lessThan(int a, int b) { return a < b; }

bool lessThan(float a, float b) { return a < b; }

bool lessEqualThan(int a, int b) { return a <= b; }

bool lessEqualThan(float a, float b) { return a <= b; }

bool greaterThan(int a, int b) { return a > b; }

bool greaterThan(float a, float b) { return a > b; }

bool greaterEqualThan(int a, int b) { return a >= b; }

bool greaterEqualThan(float a, float b) { return a >= b; }

bool inRangeInclusive(int a, int b, int c) { return a >= b && a <= c; }

bool inRangeInclusive(float a, float b, float c) { return a >= b && a <= c; }

bool inRangeExclusive(float a, float b, float c) { return a > b && a < c; }

bool inRangeExclusive(int a, int b, int c) { return a > b && a < c; }

template<class ...T>
Comparator<T...>::Comparator(const CompareFunction<T...> compare, const std::string message): 
	compare(compare), message(message){}



const Comparator<int, int> equalsI{ &equals, "equal to {}" };
const Comparator<float, float> equalsF{ &equals, "equal to {}" };
const Comparator<int, int> differentThanI{ &differentThan, "different than {}" };
const Comparator<float, float> differentThanF{ &differentThan, "different than {}" };

const Comparator<int, int> lessThanI{ &lessThan, "less than {}" };
const Comparator<float, float> lessThanF{ &lessThan, "less than {}" };
const Comparator<int, int> lessEqualThanI{ &lessEqualThan, "less or equal than {}" };
const Comparator<float, float> lessEqualThanF{ &lessEqualThan, "less or equal than {}" };

const Comparator<int, int> greaterThanI{ &greaterThan, "greater than {}" };
const Comparator<float, float> greaterThanF{ &greaterThan, "greater than {}" };
const Comparator<int, int> greaterEqualThanI{ &greaterEqualThan, "greater or equal than {}" };
const Comparator<float, float> greaterEqualThanF{ &greaterEqualThan, "greater or equal than {}" };

const Comparator<int, int, int> inRangeInclusiveI{ &inRangeInclusive, "in the range [{}, ]" };
const Comparator<float, float, float> inRangeInclusiveF{ &inRangeInclusive, "in the range [{}, ]" };
const Comparator<int, int, int> inRangeExclusiveI{ &inRangeExclusive, "in the range ]{}, [" };
const Comparator<float, float, float> inRangeExclusiveF{ &inRangeExclusive, "in the range ]{}, [" };
*/
#pragma once
#include "miscellaneous.h"
// typedef std::pair<double, double> interval;
struct interval:std::pair<double, double> {
	double height = 0;
	size_t multiplier = 1;
	interval(double x, double y, size_t mult = 1,  double h=0):std::pair<double, double>(x, y) {
		height = h;
		multiplier = mult;
	}
	interval(){};
	bool operator==(const interval& r) const {
		return (height == r.height)
			&& (first == r.first) && (second == r.second);
	};

};


struct cmp_second {
	bool operator() (const interval& a, const interval& b) const {
		return (a.second < b.second);
	}
};


struct lexico_Interval {
	bool operator() (const interval& a, const interval& b) const {
		return (a.first < b.first);
	}
};






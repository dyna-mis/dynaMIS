/*
 * problem_arrow_greedy.h
 *
 *  Created on: Nov 23, 2019
 *      Author: ping
 */

#pragma once
#include <tuple>
#include "line.h"
class Line_greedy :public Line {
private:
	int numberV = 0;
	// greedy solution
	vector<std::set<Point_2>> greedy_solution;

	void greedy();
	void get_greedy_line(size_t l);

	void update_greedy_line(size_t line, const Point_2& i, double last = -DBL_MAX);
	void add_greedy_line(const Point_2& p, size_t l);



protected:
	void add_greedy(const Point_2& p) override;
public:
	Line_greedy() {};
	void set(const shared_ptr<Problem>& P) override;


	void init() override;
	bool delete_point(size_t index) override;


	void recompute() override;
	bool add_point_recompute(const Point_2& p) override;
	bool delete_point_recompute(size_t index)override;


	void set_solution() override;
	//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
	void print() override;
	void draw(cairo_t* cr) override;
#endif
	//xPRINT---------------PRINT-------------------------

	//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
	void debug() override;
#endif
	//!DEBUG---------------debug------------------------
};

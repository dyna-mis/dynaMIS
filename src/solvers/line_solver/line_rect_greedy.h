/*
 * problem_arrow_greedy.h
 *
 *  Created on: Nov 23, 2019
 *      Author: ping
 */

#pragma once
#include <tuple>
#include "line_rect.h"
class Line_rect_greedy :public Line_rect {
private:
	// greedy solution
	vector<std::set<interval, lexico_Interval>> greedy_solution;

	void greedy();

	size_t solution_size = 0;
	void set_solution_size();
	void update_greedy_line(size_t line, const interval& i, double last = -DBL_MAX);
	void get_greedy_line(size_t l);
	void add_greedy_line(const interval& p, size_t l);


protected:
	void add_greedy(const interval& p) override;
public:
	Line_rect_greedy() {};
	void set(const shared_ptr<rect_problem>& P) override;

	bool delete_point_rect(size_t index) override;



	void init() override;
	void recompute() override;
	bool add_point_rect_recompute(const label_point& p) override;
	bool delete_point_rect_recompute(size_t index) override;
	void set_solution() override;
	//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
	void print() override;
	void draw(cairo_t* cr) override;
#endif
	//xPRINT---------------PRINT-------------------------

	//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
	void debug_rect() override;
#endif
	//!DEBUG---------------debug------------------------
};

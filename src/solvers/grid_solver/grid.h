/*
 * problem_sl.h
 *
 *  Created on: Nov 13, 2019
 *      Author: guangping
 */
#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <math.h>    
#include <list>
#include "solver_grid.h"
using namespace std;
class Grid :public solver_grid<std::list<Point_2>> {
public:
	Grid(){};
	void set(const shared_ptr<Problem>& P) override;

	void init() override;
	bool add_point(const Point_2& p) override;
	bool delete_point(size_t index) override;


	void recompute() override;
	bool add_point_recompute(const Point_2& p) override;
	bool delete_point_recompute(size_t index)override;

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

protected:
	void set_solution() override;

};




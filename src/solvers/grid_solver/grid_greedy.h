/*
 * problem_sl_greedy.h
 *
 *  Created on: Nov 19, 2019
 *      Author: guangping
 */

#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <math.h>       /* ceil */
#include "grid.h"

class Grid_greedy:public Grid{
private:
	// greedy solution
	vector<vector<Point_2>> greedy_gridP;
	vector<vector<unsigned int>> greedy_counter;
	vector<vector<Point_2>> greedyEE;//00
	vector<vector<Point_2>> greedyEO;//01
	vector<vector<Point_2>> greedyOE;//10
	vector<vector<Point_2>> greedyOO;//11


	vector<unsigned int> greedyEE_size;
	vector<unsigned int> greedyEO_size;
	vector<unsigned int> greedyOE_size;
	vector<unsigned int> greedyOO_size;


	void greedy_Line();
	void greedy_Between();

	bool check_greedy(bool uperE, bool lowerE, int i, int j, Point_2 current);

	void add_greedy(int x, int y, const Point_2& p);
	void delete_greedy(int x, int y, Point_2& p, bool inSolution);

protected:
	void set_solution() override;
public:
	Grid_greedy() {};
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
};

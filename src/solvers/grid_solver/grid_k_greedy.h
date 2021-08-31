/*
 * problem_slk_greedy.h
 * k lines
 *  Created on: Nov 21, 2019
 *      Author: guangping
 */

#pragma once
#include "grid_k.h"
class Grid_k_greedy :public Grid_k {
	private:
	//greedy solution
	vector<vector<Point_2>> greedy_gridP;
	vector<vector<int>> greedy_counter;
	//greedy between
	vector<vector<vector<vector<Point_2>>>>greedyKK;
	vector<vector<vector<int>>>greedyKKSize;

	void greedy_Line();
	void greedy_Between();


	void get_group(vector<Point_2>& solution, int line, int group);
	void get_group_greedy(vector<Point_2>& solution, int line, int group);
	void get_group_index(vector<Point_2>& solution, int line, int group);



	void add_greedy_gridP(int x, int y);
	void add_greedy_gridP_pre(int x, int y);

	void add_greedy_between(int x, int y);
	void add_greedy_between_pre(int x, int y);
	void delete_greedy(int x, int y, Point_2& p);





	void subPoint_recompute(int grid_x, int grid_y);
	void subPoint_recompute_pre(int grid_x, int grid_y);


public:
		Grid_k_greedy() {};
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
		void getCombineSolution_GreedyBetween(vector<Point_2>& solution, int line_below, int group_below, int line_upper, int group_upper);
		void getCombineSolution(vector<Point_2>& solution, int line_below, int group_below, int line_upper, int group_upper);
		void debugGreedyBetween(bool full);
		void debug() override;
#endif
		//!DEBUG---------------debug------------------------

protected:
	void set_solution() override;
};


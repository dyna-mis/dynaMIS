/*
 * problem_slk.h
 * k lines
 *  Created on: Nov 13, 2019
 *      Author: guangping
 */

#ifndef SRC_PROBLEM_SLK_H_
#define SRC_PROBLEM_SLK_H_
#include "solver_grid.h"
class Grid_k :public solver_grid<std::set<Point_2>> {

public:
	Grid_k() {};
	void set(const shared_ptr<Problem>& P) override;

	void set_solution() override;

	void init() override;
	bool add_point(const Point_2& p) override;
	bool delete_point(size_t index) override;
	void add_point_recompute_update(int x, int y);
	void add_point_recompute_pre_update(int x, int y);
	void delete_point_recompute_update(int x, int y);
	void delete_point_recompute_pre_update(int x, int y);



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
	vector<vector<vector<Point_2>>> gridS;
	vector<vector<vector<Point_2>>> preGridS;

	void get_MIS_k(int grid_x, int grid_y);
	//groups at beginning of size < k
	void get_MIS_pre(int grid_x, int grid_y);





};
#endif /* SRC_PROBLEM_SLK_H_ */

/*
 * problem_cg.h
 *
 *  Created on: Nov 13, 2019
 *      Author: guangping
 */

#pragma once
#include "solver_graph.h"
class MIS_cg:public solver_graph{
public:
	MIS_cg() {};

	void set(const shared_ptr<Problem>& P) override;
	void set(const shared_ptr<rect_problem>& P) override;
	
	void init() override;
	void recompute() override;

	bool add_point(const Point_2& p) override;
	bool delete_point(size_t index) override;

	bool add_point_rect(const label_point& p) override;
	bool delete_point_rect(size_t index) override;


	bool add_point_recompute(const Point_2& p) override;
	bool delete_point_recompute(size_t index) override;
	bool add_point_rect_recompute(const label_point& p) override;
	bool delete_point_rect_recompute(size_t index) override;

	//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
	void print() override;
	void print_rect() override;
#endif
	//xPRINT---------------PRINT-------------------------

	//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
	void debug() override;
	void debug_rect() override;
#endif
	//!DEBUG---------------debug------------------------
protected:
	void set_solution() override;

private:
	vector<unsigned> cVector;
	void add_point_solution(unsigned int index);
};




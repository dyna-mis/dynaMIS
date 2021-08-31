/*
 * problem.h
 *
 *  Created on: Sep 13, 2019
 *      Author: guangping
 */

#pragma once
#include <chrono>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include "graph.h"
#include "bbox.h"
#include "miscellaneous.h"
#include "jsonM.h"
#include "problem.h"
#include "rect_problem.h"

class Solver {
public:
	friend class simulation;
	Solver(){};
	virtual void set(const shared_ptr<Problem>& P) {
		problem_ref = P;
		mVector = vector<short>(2*P->point_size(), 0);
	};
	virtual void set(const shared_ptr<rect_problem>& P) {
		rect_problem_ref = P;
		mVector = vector<short>(2*P->point_size(), 0);
	};

	virtual void init()=0;

	virtual void recompute() = 0;


	virtual bool add_point(const Point_2& p);
	virtual bool delete_point(size_t index);
	virtual bool add_point_rect(const label_point& p) ;
	virtual bool delete_point_rect(size_t index);


	virtual bool add_point_recompute(const Point_2& p);
	virtual bool delete_point_recompute(size_t index);
	virtual bool add_point_rect_recompute(const label_point& p);
	virtual bool delete_point_rect_recompute(size_t index);

	vector<short> mVector;// 1: in, 0: not tested yet, -1: not in
	size_t get_solution_size();
	size_t get_solution_size_rect();


	//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
	virtual void draw(cairo_t* cr);
	virtual void draw_rect(cairo_t* cr);
	virtual void print();
	virtual void print_rect();
#endif
	//xPRINT---------------PRINT-------------------------

	//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
	virtual void debug();
	virtual void debug_rect();
#endif
	//!DEBUG---------------debug------------------------



protected:
	shared_ptr<Problem> problem_ref;
	shared_ptr<rect_problem> rect_problem_ref;
	virtual void set_solution() = 0;





};




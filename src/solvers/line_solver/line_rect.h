#pragma once
#include "solver_line.h"
#include "range_tree.h"
class Line_rect :public solver_line<interval, std::set<interval,lexico_Interval> >{

public:
	Line_rect() {};
	void set(const shared_ptr<rect_problem>& P) override;
	void init() override;

	bool add_point_rect(const label_point& p) override;

	bool delete_point_rect(size_t index) override;


	void recompute() override;
	bool add_point_rect_recompute(const label_point& p) override;
	bool delete_point_rect_recompute(size_t index) override;



	void set_solution();
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

protected:
	void get_MIS_line(size_t grid_y) override;
	void continue_MIS_line(const interval& p, size_t grid_y) override;
	virtual void add_greedy(const interval& p) {};
	vector<range_tree> trees;

};


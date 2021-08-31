#pragma once
#include "solver_line.h"
class Line:public solver_line<Point_2, std::set<Point_2>> {

public:
	Line() {};
	void set(const shared_ptr<Problem>& P) override;

	void init() override;
	bool add_point(const Point_2& p) override;
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

protected:
	void get_MIS_line(size_t grid_y) override;
	void continue_MIS_line(const Point_2& p, size_t grid_y) override;
	virtual void add_greedy(const Point_2& p) {};
};


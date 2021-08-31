#pragma once
#include "solver.h"
class solver_ors : public Solver {

public:
	void set(const shared_ptr<Problem>& P) override;
	void set(const shared_ptr<rect_problem>& P) override;

	bool add_point(const Point_2& p) override;
	bool delete_point(size_t index) override;
	bool add_point_rect(const label_point& p) override;

	bool delete_point_rect(size_t index) override;

	int look_up(const Point_2& Point) const;
protected:
	PointSet_CGAL pSet;
	lookupTable corner_luTable;


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
};
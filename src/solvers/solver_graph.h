#pragma once
#include "solver_ors.h"

class solver_graph : public solver_ors {
public:
	void set(const shared_ptr<Problem>& P) override;
	void set(const shared_ptr<rect_problem>& P) override;

	bool add_point(const Point_2& p) override;
	bool delete_point(size_t index) override;

	bool add_point_rect(const label_point& p) override;
	bool delete_point_rect(size_t index) override;




	//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
	void print() override;
	void print_rect() override;
	void draw(cairo_t* cr) override;
	void draw_rect(cairo_t* cr) override;
#endif
	//xPRINT---------------PRINT-------------------------

	//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
	void debug() override;
	void debug_rect() override;
#endif
	//!DEBUG---------------debug------------------------


private:
	void set_graph();
	void set_graph_rect();

protected:
	Graph conflict_graph;


};

#pragma once
#include "solver.h"
template<typename T, typename C>
class solver_line : public Solver {

public:
	void set(const shared_ptr<Problem>& P) override;
	void set(const shared_ptr<rect_problem>& P) override;

	void init() override;


	
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
	// set of points or set of intervals
	vector<C> lines;
	vector<C> solution;
	int numberH = 0;
	int sumE = 0;
	int sumO = 0;
	bool markE = false;
	virtual void get_MIS_line(size_t grid_y)= 0;
	virtual void continue_MIS_line(const T& p, size_t grid_y) = 0;
	virtual void set_solution() = 0;
};

#include "solver_line.tpp"

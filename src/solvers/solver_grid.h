
#pragma once
#include "solver.h"
template<typename T>
class solver_grid : public Solver {

public:
	void set(const shared_ptr<Problem>& P) override;
	bool add_point(const Point_2& p) override;
	bool delete_point(size_t index) override;
	void init() override;
	void recompute() override;

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
	vector<vector<T>> gridP;
	size_t numberV = 0;
	size_t numberH = 0;

	// solution size pair on each line
	vector<vector<int>> counter;
	// choice of each line


	vector<int> mark;
	// sum of taking all even lines
	int sumE = 0;
	//sum of taking all odd lines
	int sumO = 0;
	// if choosing even lines
	bool markE = false;

	// group parameter
	virtual void set_solution() = 0;


};

#include "solver_grid.tpp"

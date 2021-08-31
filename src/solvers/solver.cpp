/*
 * problem.cpp
 *
 *  Created on: Sep 13, 2019
 *      Author: guangping
 */
#include "solver.h"

bool Solver::add_point(const Point_2& p)
{
	mVector[problem_ref->point_size()] = 0;
	problem_ref->add_point(p);
	return true;
}



bool Solver::add_point_rect(const label_point& p)
{
	mVector[rect_problem_ref->point_size()] = 0;
	rect_problem_ref->add_point(p);
	return true;
}

bool Solver::delete_point(size_t index) {
	problem_ref->delete_point(index);
	mVector[index] = mVector[problem_ref->point_size()];
	return true;
};

bool Solver::delete_point_rect(size_t index) {
	rect_problem_ref->delete_point(index);
	mVector[index] = mVector[rect_problem_ref->point_size()];
	return true;
};


bool Solver::add_point_recompute(const Point_2& p)
{
	mVector[problem_ref->point_size()] = 0;
	problem_ref->add_point(p);
	return true;
}



bool Solver::add_point_rect_recompute(const label_point& p)
{
	mVector[rect_problem_ref->point_size()] = 0;
	rect_problem_ref->add_point(p);
	return true;
}

bool Solver::delete_point_recompute(size_t index) {
	problem_ref->delete_point(index);
	mVector[index] = mVector[problem_ref->point_size()];
	return true;
};

bool Solver::delete_point_rect_recompute(size_t index) {
	rect_problem_ref->delete_point(index);
	mVector[index] = mVector[rect_problem_ref->point_size()];
	return true;
};



//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
void Solver::print() {
	int s = 0;
	std::cout << "current solution:";
	for (unsigned int i = 0; i < problem_ref->pVector.size(); i++) {
		if (mVector[i] == 1) {
			s++;
			std::cout << " " << i;
		};
	}
	std::cout << std::endl;
	std::cout << "solution size: " << s << std::endl;
}



void Solver::print_rect() {
	int s = 0;
	std::cout << "current solution:";
	for (unsigned int i = 0; i < rect_problem_ref->pVector.size(); i++) {
		if (mVector[i] == 1) {
			s++;
			std::cout << " " << i;
		};
	}
	std::cout << std::endl;
	std::cout << "rectangle solution size: " << s << std::endl;
}


void Solver::draw(cairo_t* cr) {
	double x, y;
	int i = 0;
	for (auto e : problem_ref->pVector) {
		x = e.x() + 0.1 * width;
		y = 1.1 * height - e.y();
		if (mVector[i] == 1) {
			drawRect(x, y, true, false, cr, label_width, label_height);
		}
		else drawRect(x, y, false, false, cr, label_width, label_height);
		i++;
	}
};
void Solver::draw_rect(cairo_t* cr) {
	double x, y;
	int i = 0;
	for (const auto& e : rect_problem_ref->pVector) {
		x = e.x() + 0.1 * width;
		y = 1.1 * height - e.y();
		if (mVector[i] == 1) {
			drawRect(x, y, true, false, cr, e.width_single, label_height);
		}
		else drawRect(x, y, false, false, cr, e.width_single, label_height);
		i++;
	}

};

#endif
//xPRINT---------------PRINT-------------------------



//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG

void Solver::debug() {
	int inConflict = 0;
	for (unsigned int i = 0; i < problem_ref->pVector.size(); i++) {
		assert(mVector[i] == -1 || mVector[i] == 0 || mVector[i] == 1);
		if (mVector[i] == 1) {
			for (unsigned int j = 0; j < problem_ref->pVector.size(); j++) {
				if (i == j) continue;
				inConflict = isConflict_debug(problem_ref->pVector[i], problem_ref->pVector[j]);
				if (inConflict == 1) assert(mVector[j] != 1);
				if (mVector[j] == 1) {
					assert(!isConflict(problem_ref->pVector[i], problem_ref->pVector[j]));
				}
			}
		};
	}
	int countS = 0;
	for (unsigned int i = 0; i < problem_ref->pVector.size(); i++) {
		if (mVector[i] == 1) countS++;
	}
	assert(countS == get_solution_size());
}

void Solver::debug_rect() {
	int inConflict = 0;
	for (unsigned int i = 0; i < rect_problem_ref->pVector.size(); i++) {
		assert(mVector[i] == -1 || mVector[i] == 0 || mVector[i] == 1);
		if (mVector[i] == 1) {
			for (unsigned int j = 0; j < rect_problem_ref->pVector.size(); j++) {
				if (i == j) continue;
				inConflict = isConflict(rect_problem_ref->pVector[i], rect_problem_ref->pVector[j]);
				if (inConflict == 1) {
					assert(mVector[j] != 1);
				};
				if (mVector[j] == 1) {
					assert(!isConflict(rect_problem_ref->pVector[i], rect_problem_ref->pVector[j]));
				}
			}
		};
	}
	int countS = 0;
	for (unsigned int i = 0; i < rect_problem_ref->pVector.size(); i++) {
		if (mVector[i] == 1) countS++;
	}
	assert(countS == get_solution_size_rect());
}
#endif
//!DEBUG---------------debug------------------------

size_t Solver::get_solution_size() {
	set_solution();
	int countS = 0;
	for (unsigned int i = 0; i < problem_ref->pVector.size(); i++) {
		if (mVector[i] == 1) countS++;
	}
	return countS;
}

size_t Solver::get_solution_size_rect() {
	set_solution();
	int countS = 0;
	for (unsigned int i = 0; i < rect_problem_ref->pVector.size(); i++) {
		if (mVector[i] == 1) countS++;
	}
	return countS;
}
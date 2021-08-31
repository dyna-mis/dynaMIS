#pragma once
#include "solver.h"
#include "problem.h"
#include "wmis_solver.h"
#include "problem.h"
#include "rect_problem.h"
template<class S>
class Simulation{
public:
	Simulation(){};
	//init
	void set();
	void set_rect();

	void run();
	void run_rect();

	void add_point_recompute(const Point_2& p);

	void delete_point_recompute(size_t index);


	void run_recompute();
	void run_rect_recompute();

	void add_point(const Point_2& p);
	void add_point_rect(const label_point& p);
	void delete_point(size_t index);
	void delete_point_rect(size_t index);
	void dummy_test(const Problem& P) {
		std::cout << "I call the base " << std::endl;

	};
	void dummy_test(const rect_problem& P) {
		std::cout << "I call the derived " << std::endl;

	};
	//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
	void draw(cairo_t* cr) {
		solver.draw(cr);
		if (problem_ref != NULL) {
			problem_ref->draw(cr);
		}
		if (rect_problem_ref != NULL) {
			rect_problem_ref->draw(cr);
		}


	};
	void print() {
		solver.print();
		if (problem_ref != NULL) {
			problem_ref->print();
		}
		if (rect_problem_ref != NULL) {
			rect_problem_ref->print();
		}
	};
#endif
	//xPRINT---------------PRINT-------------------------

	//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
	void debug() {
		if (problem_ref != NULL) {
			solver.debug();
			problem_ref->debug();
		}
		if (rect_problem_ref != NULL) {
			solver.debug_rect();
			rect_problem_ref->debug();
		}
	};

#endif
	//!DEBUG---------------debug------------------------


protected:
	

	void ratio_addition();
	void ratio_addition_rect();

	void ratio_deletion();
	void ratio_deletion_rect();

	void ratio_mixture();
	void ratio_mixture_rect();

	void ratioChange() {
		if (modType.compare("add") == 0) {
			ratio_addition();
			return;
		}
		if (modType.compare("sub") == 0) {
			ratio_deletion();
			return;
		}
		if (modType.compare("mix") == 0) {
			ratio_mixture();
			return;
		}
		exit(-1);
	};


	void ratioChange_rect() {
		if (modType.compare("add") == 0) {
			ratio_addition_rect();
			return;
		}
		if (modType.compare("sub") == 0) {
			ratio_deletion_rect();
			return;
		}
		if (modType.compare("mix") == 0) {
			ratio_mixture_rect();
			return;
		}
		exit(-1);
	};


	void add_point_rect_recompute(const label_point& p);

	void delete_point_rect_recompute(size_t index);

	void ratio_addition_recompute();
	void ratio_addition_rect_recompute();

	void ratio_deletion_recompute();
	void ratio_deletion_rect_recompute();

	void ratio_mixture_recompute();
	void ratio_mixture_rect_recompute();

	void ratioChange_recompute() {
		if (modType.compare("add") == 0) {
			ratio_addition_recompute();
			return;
		}
		if (modType.compare("sub") == 0) {
			ratio_deletion_recompute();
			return;
		}
		if (modType.compare("mix") == 0) {
			ratio_mixture_recompute();
			return;
		}
		exit(-1);
	};


	void ratioChange_rect_recompute() {
		if (modType.compare("add") == 0) {
			ratio_addition_rect_recompute();
			return;
		}
		if (modType.compare("sub") == 0) {
			ratio_deletion_rect_recompute();
			return;
		}
		if (modType.compare("mix") == 0) {
			ratio_mixture_rect_recompute();
			return;
		}
		exit(-1);
	};



private:
	S solver;
	std::shared_ptr <Problem> problem_ref;
	std::shared_ptr <rect_problem> rect_problem_ref;


	inline bool check_valid(const Point_2& p);
	Point_2 rand_point();
	label_point rand_point_rect();

	inline bool look_up(double x, double y) {
		if (points_y.find(y) != points_y.end()) return true;
		if (points_x.find(x) != points_x.end()) return true;

		else return false;
	}


};

#include "simulation.tpp"

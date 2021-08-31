#include <iostream>
#include "dynaMIS.h"
using namespace std;
std::unique_ptr<std::mt19937> gen;

template <typename S>
void test_solver() {
	Simulation<S> Sim;
	Sim.set();
	Sim.run();
};

template <typename S>
void test_solver_rect() {
	Simulation<S> Sim;
	Sim.set_rect();
	Sim.run_rect();
};

template <typename S>
void test_solver_recompute() {
	Simulation<S> Sim;
	Sim.set();
	Sim.run_recompute();
};

template <typename S>
void test_solver_rect_recompute() {
	Simulation<S> Sim;
	Sim.set_rect();
	Sim.run_rect_recompute();
};



void test() {
	if (algoType.compare("wmis") == 0) test_solver<wmis_solver>();

	else if (algoType.compare("graph") == 0) test_solver<MIS_cg>();

	else if (algoType.compare("ors") == 0) test_solver<MIS_ors>();

	else if (algoType.compare("grid") == 0) {
		if (greedy_v)  test_solver<Grid_greedy>();
		else test_solver<Grid>();
	}
	else if (algoType.compare("gridK") == 0) {
		if (greedy_v)  test_solver<Grid_k_greedy>();
		else test_solver<Grid_k>();
	}
	else if (algoType.compare("line") == 0) {
		if (greedy_v)  test_solver<Line_greedy>();
		else test_solver<Line>();
	}
	else {
		exit(-1);
	}
}

void test_rect(){
	if (algoType.compare("wmis") == 0) test_solver_rect<wmis_solver>();

	else if (algoType.compare("graph") == 0) test_solver_rect<MIS_cg>();

	else if (algoType.compare("line") == 0) {
		if (greedy_v)  test_solver_rect<Line_rect_greedy>();
		else test_solver_rect<Line_rect>();
	}
	else {
		exit(-1);
	}
}

void test_recompute() {
	if (algoType.compare("wmis") == 0) test_solver_recompute<wmis_solver>();

	else if (algoType.compare("graph") == 0) test_solver_recompute<MIS_cg>();

	else if (algoType.compare("ors") == 0) test_solver_recompute<MIS_ors>();

	else if (algoType.compare("grid") == 0) {
		if (greedy_v)  test_solver_recompute<Grid_greedy>();
		else test_solver_recompute<Grid>();
	}
	else if (algoType.compare("gridK") == 0) {
		if (greedy_v)  test_solver_recompute<Grid_k_greedy>();
		else test_solver_recompute<Grid_k>();
	}
	else if (algoType.compare("line") == 0) {
		if (greedy_v)  test_solver_recompute<Line_greedy>();
		else test_solver_recompute<Line>();
	}
	else {
		exit(-1);
	}
}

void test_rect_recompute() {
	if (algoType.compare("wmis") == 0) test_solver_rect_recompute<wmis_solver>();

	else if (algoType.compare("graph") == 0) test_solver_rect_recompute<MIS_cg>();

	else if (algoType.compare("line") == 0) {
		if (greedy_v)  test_solver_rect_recompute<Line_rect_greedy>();
		else test_solver_rect_recompute<Line_rect>();
	}
	else {
		exit(-1);
	}
}

int main(int argc, char* argv[]) {
#ifdef __linux__
	std::cout << "run on linux" << std::endl;
#elif _WIN32
	std::cout << "run on windows" << std::endl;
#endif

#ifdef DYNAMIS_PRINT
	std::cout << "run print mode" << std::endl;
#endif

#ifdef DYNAMIS_DEBUG
	std::cout << "run debug mode" << std::endl;
#endif

	parseInitOptions(argc, argv);

	gen = make_unique<std::mt19937>(seed);
	printInitOptions();

	if(recomp_v){
		if (rectangle_v)  test_rect_recompute();
		else test_recompute();
	}
	else {
		if (rectangle_v)  test_rect();
		else test();
	
	}
}

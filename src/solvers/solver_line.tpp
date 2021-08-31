#include "solver_line.h"
template<typename T, typename C>
void solver_line<T,C>::set(const shared_ptr<Problem>& P) {
	Solver::set(P);
	numberH = ceil(height /label_height) +1;
};

template<typename T, typename C>
void solver_line<T,C>::set(const shared_ptr<rect_problem>& P) {
	Solver::set(P);
	numberH = ceil(height / label_height) + 1;
};
template<typename T, typename C>
void solver_line<T,C>::init() {
	for (int i = 0; i < numberH; i += 2) {
		sumE += solution[i].size();
	}
	for (int i = 1; i < numberH; i += 2) {
		sumO += solution[i].size();
	}
	markE = (sumE > sumO);
}

//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
template<typename T, typename C>
void solver_line<T, C>::print() {
	Solver::print();
	std::cout << "numH:" << numberH << std::endl;
	std::cout << "sumE:" << sumE << std::endl;
	std::cout << "sumO:" << sumO << std::endl;
	std::cout << "markE:" << markE << std::endl;

	std::cout << "the lines" << std::endl;
	int i = 0;
	int c = (markE == true) ? 0 : 1;
	string color;
	bool contain;
	for (const auto& h : lines) {
		if (i % 2 != c) color = YELLOW;
		else color = GREEN;
		for (const auto& p : h) {
			contain = (solution[i].find(p) != solution[i].end());
			if (contain) {
				std::cout << color;
				print_point(p);
				std::cout << RESET;
			}
			else {
				std::cout << RED;
				print_point(p);
				std::cout << RESET;
			
			} 
		}
		std::cout << std::endl;
		i++;
	}

};
template<typename T, typename C>
void solver_line<T, C>::draw(cairo_t* cr) {
	set_solution();
	cairo_set_source_rgb(cr, 0.0, 1.0, 1.0);
	// draw lines
	double x, y;
	double end_x = width * 1.1;
	double	end_y = height * 1.1;
	cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
	for (int i = 0; i < numberH; i++) {
		cairo_stroke(cr);
		y = end_y - i * label_height;
		cairo_move_to(cr, 0.1 * width, y);
		cairo_line_to(cr, end_x, y);
		cairo_stroke(cr);
	}
	Solver::draw(cr);
};
#endif
//xPRINT---------------PRINT-------------------------

//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
template<typename T, typename C>
void solver_line<T, C>::debug() {
	set_solution();
	Solver::debug();


	int sE = 0;
	int sO = 0;
	for (int i = 0; i < numberH; i += 2) {
		sE += solution[i].size();
	}
	for (int i = 1; i < numberH; i += 2) {
		sO += solution[i].size();
	}
	assert(sE == sumE);
	if (sO != sumO) {
		for (int i = 1; i < numberH; i += 2) {
			std::cout << "i: " << i << " " << solution[i].size() << std::endl;
		}
		std::cout << "sO: " << sO << std::endl;
		std::cout << "sumO: " << sumO << std::endl;
	}
	assert(sO == sumO);





	if (sumE > sumO) {
		assert(markE == true);
	}
	else {
		if (sumE == sumO) return;
		assert(markE == false);
	}
};
#endif
//!DEBUG---------------debug------------------------s





/*
 * grid.cpp
 *
 *  Created on: Oct 17, 2019
 *      Author: guangping
 */
#include "grid.h"
void Grid::set(const shared_ptr<Problem>& P){
	param_k = 1;
	solver_grid<std::list<Point_2>>::set(P);
};

void Grid::init() {
	for (size_t y = 0; y < numberH; y++) {
		for (size_t x = 0; x < numberV; x++) {
			if (gridP[y][x].size() > 0) {
				if (x % 2 == 0) counter[y][0]++;
				else counter[y][1]++;
			}
		}
	}
	/*
	std::cout << "The counter:" << std::endl;
	for (const auto& e : counter) {

		for (const auto& i : e) {
			std::cout << i << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	*/

	solver_grid<std::list<Point_2>>::init();
}

void Grid::recompute()
{
	solver_grid<std::list<Point_2>>::recompute();
	init();
}


bool Grid::add_point_recompute(const Point_2& p) {
	solver_grid<std::list<Point_2>>::add_point(p);
	recompute();
	return true;
};
bool Grid::delete_point_recompute(size_t index) {
	solver_grid<std::list<Point_2>>::delete_point(index);
	recompute();
	return true;
};

bool Grid::add_point(const Point_2& p) {
	solver_grid<std::list<Point_2>>::add_point(p);
	int x = ceil((p.x() - h_label_width) / label_width);
	int y = ceil((p.y() - h_label_height) / label_height);
	int& sum = ((y % 2) == 0) ? sumE : sumO;

	if (gridP[y][x].size() == 1) {
		if ((x % 2) == 0) {
			counter[y][0]++;
			if (mark[y] == 1) {
				if (counter[y][0] > counter[y][1]) {
					mark[y] = 0;
					sum++;
				}
			}
			else 		sum++;
		}
		else {
			counter[y][1]++;
			if (mark[y] == 0) {
				if (counter[y][0] < counter[y][1]) {
					mark[y] = 1;
					sum++;
				}
			}
			else 		sum++;
		}
		markE = (sumE > sumO); 
	}
	return true;
};
bool Grid::delete_point(size_t index) {
	Point_2 p = problem_ref->pVector[index];
	int x = ceil((p.x() - h_label_width) / label_width);
	int y = ceil((p.y() - h_label_height) / label_height);

	solver_grid<std::list<Point_2>>::delete_point(index);
	int& sum = (y % 2 == 0) ? sumE : sumO;
	if (gridP[y][x].size() == 0) {
		if (x % 2 == 0) {
			counter[y][0]--;
			if (mark[y] == 0) {
				if (counter[y][0] < counter[y][1]) {
					mark[y] = 1;
				}
				else sum--;
			}
		}
		else {
			counter[y][1]--;
			if (mark[y] == 1) {
				if (counter[y][0] > counter[y][1]) {
					mark[y] = 0;
				}
				else {
					sum--;
				}
			}
		}
		markE = (sumE > sumO);
	}
	return true;
};


void Grid::set_solution() {
	std::fill(mVector.begin(), mVector.end(), 0);
	Point_2 e;
	// draw solution
	double x, y;
	int c = (markE == true) ? 0 : 1;
	for (int i = 0; i < numberH; i++) {
		if (i % 2 != c)continue;
		for (int j = 0; j < numberV; j++) {
			if (j % 2 != mark[i])continue;
			if (gridP[i][j].size() > 0) {
				size_t index = problem_ref->look_up(gridP[i][j].back());
				mVector[index] = 1;
			}
		}
	}
}

//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
void Grid::print() {
	set_solution();
	solver_grid::print();
} 
void Grid::draw(cairo_t* cr) {
	set_solution();
	solver_grid::draw(cr);
};

#endif
//xPRINT---------------PRINT-------------------------

//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
void Grid::debug() {
	set_solution();
	solver_grid::debug();
}
#endif
//!DEBUG---------------debug------------------------
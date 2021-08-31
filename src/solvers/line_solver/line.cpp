/*
 * problem_arrow.cpp
 *
 *  Created on: Nov 17, 2019
 *      Author: ping
 */
#include "line.h"



void Line::set(const shared_ptr<Problem>& P) {
	solver_line<Point_2, std::set<Point_2>>::set(P);
	lines = std::vector<std::set<Point_2>>(numberH, std::set<Point_2>());
	solution = std::vector<std::set<Point_2>>(numberH, std::set<Point_2>());


	int y;
	for (const auto& p : problem_ref->pVector) {
		y = ceil((p.y() - h_label_height) / label_height);
		//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
		#ifdef DYNAMIS_DEBUG
		assert(lines[y].find(p) == lines[y].end());
		#endif
		//!DEBUG---------------debug------------------------
		lines[y].insert(p);
	}
}




void Line::recompute() {
	solution = std::vector<std::set<Point_2>>(numberH, std::set<Point_2>());
	sumE = 0;
	sumO = 0;
	markE = false;
	init();


};
bool Line::add_point_recompute(const Point_2& p) {
	Solver::add_point(p);
	int y = ceil((p.y() - h_label_height) / label_height);
	lines[y].insert(p);
	recompute();
	return true;
};
bool Line::delete_point_recompute(size_t index) {
	Point_2 p = problem_ref->pVector[index];
	Solver::delete_point(index);
	int y = ceil((p.y() - h_label_height) / label_height);
	lines[y].erase(p);
	recompute();
	return true;
};


// get maximum independent set of one line
void Line::get_MIS_line(size_t grid_y){
	Point_2 start = Point_2(-1,-1);
	solution[grid_y].clear();
	auto it = lines[grid_y].lower_bound(start);
	while(it != lines[grid_y].end()){
		solution[grid_y].insert(*it);
		start = (*it)+Vector(label_width,0);
		it = lines[grid_y].lower_bound(start);
	}
}

void  Line::continue_MIS_line(const Point_2& p, size_t grid_y) {
	Point_2 start = p + Vector(label_width, 0);
	auto it = lines[grid_y].lower_bound(start);
	auto candidateT = solution[grid_y].lower_bound(start);
	while (it != lines[grid_y].end()) {
		if (candidateT != solution[grid_y].end() && *it == *candidateT) {
			return;
		}
		start = (*it) + Vector(label_width, 0);
		solution[grid_y].insert(*it);
		add_greedy(*it);
		if (candidateT != solution[grid_y].end()) {
			solution[grid_y].erase(candidateT);
		}
		it = lines[grid_y].lower_bound(start);
		candidateT = solution[grid_y].lower_bound(start);;
	}
}

void Line::init(){
	for(int i = 0; i < numberH; i++){
		get_MIS_line(i);
	}
	solver_line<Point_2, std::set<Point_2>>::init();
};



bool Line::add_point(const Point_2& p) {
	Solver::add_point(p);
	bool updated = false;
	int y = ceil((p.y() - h_label_height) / label_height);
	lines[y].insert(p);
	unsigned int oldSize = solution[y].size();
	if (oldSize == 0) {
		solution[y].insert(p);
		add_greedy(p);
		if ((y % 2) == 0) sumE++;
		else sumO++;
		markE = (sumE > sumO);
		return true;
	}
	//std:: cout<< "point A"<< std::endl;
	auto next = solution[y].lower_bound(p);
	if (next != solution[y].begin()) {
		auto prev = std::prev(next);
		if (p.x() < (*prev).x() + label_width) return false;
	}
	if (next == solution[y].end() || p.x() <= (*next).x() - label_width) {
		solution[y].insert(p);
		add_greedy(p);
		updated = true;
	}
	else {
		solution[y].erase(next);
		solution[y].insert(p);
		updated = true;
		add_greedy(p);
		continue_MIS_line(p, y);
	}
	//assert(solution[y].size() >= oldSize);
	if (solution[y].size() > oldSize) {
		//assert(solution[y].size()  == oldSize+1);
		if ((y % 2) == 0) sumE++;
		else sumO++;
		markE = (sumE > sumO);
	}
	return updated;
};




bool Line::delete_point(size_t index) {
	Point_2 p = problem_ref->pVector[index];
	Solver::delete_point(index);
	int y = ceil((p.y() - h_label_height) / label_height);
	Point_2 temp;
	unsigned int oldSize = solution[y].size();
	lines[y].erase(p);


	std::set<Point_2>::iterator it;
	it = solution[y].find(p);


	// if p is in the solution
	if (it != solution[y].end()) {
		if (it != solution[y].begin()) {
			auto prev = std::prev(it);
			temp = *prev;
			solution[y].erase(p);
			continue_MIS_line(temp, y);
		}
		//if p is the start
		else {
			solution[y].erase(p);
			continue_MIS_line(Point_2(-DBL_MAX, p.y()), y);
		}
		//assert(solution[y].size() <= oldSize);
		if (solution[y].size() < oldSize) {
			//std::cout<< "size reduced"<<std::endl;
			//assert(solution[y].size()  == oldSize-1);
			if ((y % 2) == 0) sumE--;
			else sumO--;
			markE = (sumE > sumO);
		}

		return true;
	}
	return false;
}

void Line ::set_solution() {
	std::fill(mVector.begin(), mVector.end(), 0);
	int c = (markE == true) ? 0 : 1;
	// should in the line
	for (int i = c; i < numberH; i+=2) {
		for (const auto& s : solution[i]) {
			int index = problem_ref->look_up(s);
			mVector[index] = 1;
		}
	}
}
//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
void Line::print() {
	set_solution();
	solver_line<Point_2, std::set<Point_2>>::print();


};
void Line::draw(cairo_t* cr) {
	set_solution();
	solver_line<Point_2, std::set<Point_2>>::draw(cr);


};
#endif
//xPRINT---------------PRINT-------------------------

//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
void Line::debug() {
	// == ones in pVector
	int count = problem_ref->pVector.size();
	int i = 0;
	int y;
	for (const auto& l : lines) {
		for (const auto& e : l) {
			vectorContain(problem_ref->pVector, e);
			// each line contains the right ones
			y = ceil((e.y() - h_label_height) / label_height);
			assert(y == i);
			count--;
		}
		i++;
	}
	assert(count == 0);


	int c = (markE == true) ? 0 : 1;
	// should in the line
	for (int i = c; i < numberH; i++) {
		for (auto s : solution[i]) {
			assert(lines[i].find(s) != lines[i].end());
		}
	}
	// every line real MIS
	std::set<Point_2> MIS;
	i = 0;
	for (auto line : lines) {
		Point_2 start = Point_2(-1, -1);
		MIS.clear();
		auto it = lines[i].lower_bound(start);
		while (it != lines[i].end()) {
			MIS.insert(*it);
			start = (*it) + Vector(label_width, 0);
			it = lines[i].lower_bound(start);
		}
		assert(independency(solution[i]));
		assert(independency(MIS));

		assert(MIS.size() == solution[i].size());
		i++;
	}
	solver_line<Point_2, std::set<Point_2>>::debug();


};
#endif
//!DEBUG---------------debug------------------------



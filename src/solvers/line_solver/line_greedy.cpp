/*
 * Line_greedy::.cpp
 *
 *  Created on: Nov 23, 2019
 *      Author: ping
 */
#include "line_greedy.h"
void Line_greedy::set(const shared_ptr<Problem>& P)
{
	Line::set(P);
	numberV = ceil(width / label_width);
	greedy_solution = vector<std::set<Point_2>>(numberH, std::set<Point_2>());
};





void Line_greedy::init() {
	Line::init();
	greedy();
};


void Line_greedy::recompute()
{
	greedy_solution = vector<std::set<Point_2>>(numberH, std::set<Point_2>());
	Line::recompute();
}

bool Line_greedy::add_point_recompute(const Point_2& p)
{
	int y = ceil((p.y() - h_label_height) / label_height);
	lines[y].insert(p);
	Solver::add_point(p);
	recompute();
	return true;
}

bool Line_greedy::delete_point_recompute(size_t index)
{
	Point_2 p = problem_ref->pVector[index];
	int y = ceil((p.y() - h_label_height) / label_height);
	greedy_solution[y].erase(p);
	lines[y].erase(p);
	Solver::delete_point(index);
	recompute();
	return true;
}

void Line_greedy::greedy() {
	for (int i = 0; i < numberH; i++) {
		get_greedy_line(i);
	}
}







void  Line_greedy::get_greedy_line(size_t i) {
	std::set<Point_2>* solution_below = NULL;
	std::set<Point_2>* solution_above = NULL;
	double last = -DBL_MAX;

	if (i > 0) {
		solution_below = &solution[i - 1];
	}
	if (i < numberH - 1) {
		solution_above = &solution[i + 1];
	}
	for (auto e : lines[i]) {
		if (e.x() < last) continue;
		if (solution_below != NULL && !extendible_OrderedSET((*solution_below), e)) {
			continue;
		}
		if (solution_above != NULL && !extendible_OrderedSET((*solution_above), e)) {
			continue;
		}
		//std::cout<< "Point $$"<< std::endl;
		greedy_solution[i].insert(e);
		last = e.x() + label_width;
	}



}

void Line_greedy::update_greedy_line(size_t l, const Point_2& i, double last_t) {
	double start = last_t;
	auto candidateT = greedy_solution[l].end();
	auto candidateT_i = greedy_solution[l].find(i);
	if (candidateT_i != greedy_solution[l].end()) {
		candidateT = next(candidateT_i);
	}

	auto it = lines[l].lower_bound(Point_2(last_t, 0));

	std::set < Point_2>* solution_below = NULL;
	std::set<Point_2>* solution_above = NULL;
	if (l > 0) solution_below = &solution[l - 1];
	if (l < numberH - 1) solution_above = &solution[l + 1];
	auto e = it;
	while (e != lines[l].end()) {
		if (solution_below != NULL && !extendible_OrderedSET((*solution_below), *e)) {
			e = next(e);
			continue;
		}
		if (solution_above != NULL && !extendible_OrderedSET((*solution_above), *e)) {
			e = next(e);
			continue;
		}

		if (candidateT != greedy_solution[l].end() && *e == *candidateT) {
			return;
		}

		while (candidateT != greedy_solution[l].end() && candidateT->x() < e->x() + label_width) {
			candidateT_i = candidateT;
			candidateT = std::next(candidateT);
			greedy_solution[l].erase(candidateT_i);
		}
		greedy_solution[l].insert(*e);

		assert(independency(greedy_solution[l]));

		e = lines[l].lower_bound((*e) + Vector(label_width, 0));
	}
}

void Line_greedy::set_solution() {
	int chosen = (markE == true) ? 0 : 1;
	std::fill(mVector.begin(), mVector.end(), 0);
	size_t sum_original = (markE == true) ? sumE : sumO;
	size_t sum_greedy = 0;
	for (int i = 0; i < numberH; i++) {
		if (i % 2 == chosen) {
			for (const auto& p : solution[i]) {
				int index = problem_ref->look_up(p);
				mVector[index] = 1;
			};
		};
		if (i % 2 != chosen) {
			for (const auto& p : greedy_solution[i]) {
				int index = problem_ref->look_up(p);
				mVector[index] = 1;

			}
		};
	}
};



void Line_greedy::add_greedy(const Point_2& p) {
	int y = ceil((p.y() - h_label_height) / label_height);

	// it has lower one
	if (y > 0) {
		add_greedy_line(p, y - 1);
	}
	//it has above one
	if (y < numberH - 1) {
		add_greedy_line(p, y + 1);
	}
}

//check the greedy parts around a new point in the solution
void Line_greedy::add_greedy_line(const Point_2& p, size_t l){

	bool update = false;
	auto it = greedy_solution[l].lower_bound(p);
	if (it != greedy_solution[l].end()) {
		update = isConflict(p, *it);
	}
	// test previous one
	if (it != greedy_solution[l].begin()) {
		auto it_prev = std::prev(it);
		bool inConflict = isConflict(p, *it_prev);
		if (inConflict) { it = it_prev; update = true; }
	}

	if (update) {
		double last = -DBL_MAX;
		if (it != greedy_solution[l].begin()) {
			auto it_prev = std::prev(it);
			last = it_prev->x() + label_width;
			greedy_solution[l].erase(it);
			update_greedy_line(l, *it_prev, last);
		}
		else {
			greedy_solution[l].clear();
			get_greedy_line(l);

		}
	}
}


//TODO: not greedy
bool Line_greedy::delete_point(size_t index) {
	Point_2 p = problem_ref->pVector[index];
	int y = ceil((p.y() - h_label_height) / label_height);
	int x = ceil((p.x() - h_label_width) / label_width);
	int s = greedy_solution[y].erase(p);

	bool updated = Line::delete_point(index);

	if (s > 0) {
		greedy_solution[y].clear();
		get_greedy_line(y);

	}
	if (updated) {
		if (y > 1) {
			greedy_solution[y - 1].clear();
			get_greedy_line(y - 1);
		}
	
		if (y < numberH-1) {
			greedy_solution[y + 1].clear();
			get_greedy_line(y + 1);
		}
	} 
	return true;
}





//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
void Line_greedy::print() {
	set_solution();

	int count = 0;
	for (int i = 0; i < numberH; i++) {
		count = 0;
		for (auto e : greedy_solution[i]) {
			std::cout << "(" << e.x() << "," << e.y() << ") ";
			count++;
		}
		std::cout << std::endl;
	}

	Line::print();

};
void Line_greedy::draw(cairo_t* cr) {
	set_solution();
	Line::draw(cr);

};
#endif
//xPRINT---------------PRINT-------------------------

//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
void Line_greedy::debug() {
	Line::debug();

	for (int i = 0; i < numberH; i++) {
		int count = 0;
		for (auto e : greedy_solution[i]) {
			if (e != Point_2(-1, -1)) {
				count++;
			}
		}
		assert(count == greedy_solution[i].size());
	}


	vector<Point_2> solution_c;
	int count = 0;
	for (int i = 0; i < numberH; i++) {
		count = 0;
		solution_c.clear();
		if (i > 0) solution_c.insert(solution_c.end(), solution[i - 1].begin(), solution[i - 1].end());
		if (i < numberH - 1) solution_c.insert(solution_c.end(), solution[i + 1].begin(), solution[i + 1].end());
		assert(independency(solution_c));
		for (auto e : greedy_solution[i]) {
			if (e != Point_2(-1, -1)) {
				solution_c.push_back(e);
				assert(independency(solution_c));
				count++;
			}
		}
	}
};
#endif
//!DEBUG---------------debug------------------------



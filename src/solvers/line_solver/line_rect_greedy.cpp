/*
 * Line_greedy::.cpp
 *
 *  Created on: Nov 23, 2019
 *      Author: ping
 */
#include "line_rect_greedy.h"
void Line_rect_greedy::set(const shared_ptr<rect_problem>& P)
{
	Line_rect::set(P);
	greedy_solution = vector<std::set<interval, lexico_Interval>>(numberH, std::set<interval, lexico_Interval>());
};





void Line_rect_greedy::init() {
	Line_rect::init();
	greedy();
};


void Line_rect_greedy::recompute()
{
	greedy_solution = vector<std::set<interval, lexico_Interval>>(numberH, std::set<interval, lexico_Interval>());
	Line_rect::recompute();
	greedy();

}

bool Line_rect_greedy::add_point_rect_recompute(const label_point& p)
{
	Solver::add_point_rect(p);
	int y = ceil(p.y() / label_height);
	const interval I(p.x(), p.x() +p.width_single, p.multiplier, p.y());
	lines[y].insert(I);
	trees[y].add_element(I);
	recompute();
	return true;
}

bool Line_rect_greedy::delete_point_rect_recompute(size_t index)
{
	const label_point& p = rect_problem_ref->pVector[index];
	interval I = interval(p.x(), p.x()+ p.width_single,p.multiplier, p.y());
	size_t y = ceil(p.y() / label_height);
	if (mVector[index] == 1) {
		solution[y].erase(I);
	}
	greedy_solution[y].erase(I);
	lines[y].erase(I);
	trees[y].delete_element(I);

	Solver::delete_point_rect(index);
	recompute();
	return true;
}



void Line_rect_greedy::greedy() {
	//std::cout<< "in greedy"<< std::endl;
	for (int i = 0; i < numberH; i++) {
		get_greedy_line(i);
	}
}

void  Line_rect_greedy::get_greedy_line(size_t l) {
	std::set < interval, lexico_Interval >* solution_below = NULL;
	std::set<interval, lexico_Interval>* solution_above = NULL;
	solution_below = NULL;
	solution_above = NULL;
	double last = -DBL_MAX;
	// TODO: extend for ordered set (same issue as in the TODO of slk class)
	if (l > 0) {
		solution_below = &solution[l - 1];
	}
	if (l < numberH - 1) {
		solution_above = &solution[l + 1];
	}
	for (const auto& e : lines[l]) {
		if (e.first < last) continue;
		if (solution_below != NULL && !extendible_OrderedSET((*solution_below), e)) {
			continue;
		}
		if (solution_above != NULL && !extendible_OrderedSET((*solution_above), e)) {
			continue;
		}
		//std::cout<< "Point $$"<< std::endl;
		greedy_solution[l].insert(e);
		last = e.second;
	}




}

void  Line_rect_greedy::update_greedy_line(size_t l, const interval& i, double last_t) {
	double start = last_t;
	auto candidateT = greedy_solution[l].end();
	auto candidateT_i = greedy_solution[l].find(i);
	if (candidateT_i != greedy_solution[l].end()) {
		candidateT = next(candidateT_i);
	}
	auto it = lines[l].lower_bound(interval(last_t, last_t));

	std::set < interval, lexico_Interval >* solution_below = NULL;
	std::set<interval, lexico_Interval>* solution_above = NULL;
	if (l > 0) solution_below = &solution[l - 1];
	if (l < numberH - 1) solution_above = &solution[l + 1];
	auto e = it;
	while(e != lines[l].end()) {
		if (solution_below != NULL && !extendible_OrderedSET((*solution_below), *e)) {
			e = next(e);
			continue;
		}
		if (solution_above != NULL && !extendible_OrderedSET((*solution_above), *e)) {
			e = next(e);
			continue;
		}

		if (candidateT != greedy_solution[l].end() && *e == *candidateT) {
			if (!extendible_OrderedSET((*solution_below), *candidateT) ||
				extendible_OrderedSET((*solution_above), *candidateT)) 
			{
				candidateT_i = candidateT;
				candidateT = std::next(candidateT);
				greedy_solution[l].erase(candidateT_i);
			
			}
			else return;
		}

		while (candidateT != greedy_solution[l].end() && candidateT->first < e->second) {
			candidateT_i = candidateT;
			candidateT = std::next(candidateT);
			greedy_solution[l].erase(candidateT_i);
		}
		greedy_solution[l].insert(*e);

		//assert(independency(greedy_solution[l]));
		interval I(e->second, e->second,0);
		e = lines[l].lower_bound(I);
	}
}



void Line_rect_greedy::set_solution_size() {
	int chosen = (markE == true) ? 0 : 1;
	size_t sum_original = (markE == true) ? sumE : sumO;
	size_t sum_greedy = 0;
	for (int i = 0; i < numberH; i++) {
		if (i % 2 != chosen) {
			sum_greedy += greedy_solution[i].size();
		};
	}
	solution_size = sum_original + sum_greedy;
};






void Line_rect_greedy::add_greedy(const interval& p) {
	int y = ceil(p.height / label_height);

	// it has lower one
	if (y > 0) {
		add_greedy_line(p, y - 1);
	}
	//it has above one
	if (y < numberH - 1) {
		add_greedy_line(p, y + 1);
	}
}



void Line_rect_greedy::add_greedy_line(const interval& p, size_t l) {
	bool update = false;
	auto it = greedy_solution[l].upper_bound(p);
	if (it != greedy_solution[l].end()) {
		update = isConflict(p, *it);
	}
	// test previous one
	if (it != greedy_solution[l].begin()) {
		auto it_prev = std::prev(it);
		double inConflict = isConflict(p, *it_prev);
		if (inConflict) { it = it_prev; update = true; }
	}

	if (update) {
		double last = -DBL_MAX;
		if (it != greedy_solution[l].begin()) {
			auto it_prev = std::prev(it);
			last = it_prev->second;
			greedy_solution[l].erase(it);
			update_greedy_line(l, *it_prev, last);
		}
		else {
			greedy_solution[l].clear();
			get_greedy_line(l);

		}
	}
}



bool Line_rect_greedy::delete_point_rect(size_t index) {

	const label_point& p = rect_problem_ref->pVector[index];
	interval I = interval(p.x(), p.x() + p.width_single, p.multiplier, p.y());
	size_t y = ceil(p.y() / label_height);
	greedy_solution[y].erase(I);

	Line_rect::delete_point_rect(index);

	return true;
}




void Line_rect_greedy::set_solution() {
	Line_rect::set_solution();
	int chosen = (markE == true) ? 0 : 1;
	for (int i = 0; i < numberH; i++) {
		if (i % 2 != chosen) {
			for (const auto& s : greedy_solution[i]) {
				label_point p = label_point(s.first, s.height, s.multiplier);
				int index = rect_problem_ref->look_up(p);
				mVector[index] = 1;
			}
		};
	}
};




//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
void Line_rect_greedy::print() {
	set_solution();

	int count = 0;
	for (int i = 0; i < numberH; i++) {
		count = 0;
		for (const auto& e : greedy_solution[i]) {
			print_point(e);
			count++;
		}
		std::cout << std::endl;
	}

	Line_rect::print();

};
void Line_rect_greedy::draw(cairo_t* cr) {
	set_solution();
	Line_rect::draw(cr);

};
#endif
//xPRINT---------------PRINT-------------------------

//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
void Line_rect_greedy::debug_rect() {
	/*
	vector <interval > solution_c;
	int count = 0;
	for (int i = 0; i < numberH; i++) {
		count = 0;
		solution_c.clear();
		if (i > 0) solution_c.insert(solution_c.end(), solution[i - 1].begin(), solution[i - 1].end());
		if (i < numberH - 1) solution_c.insert(solution_c.end(), solution[i + 1].begin(), solution[i + 1].end());
		assert(independency(solution_c));
		for (const auto& e : greedy_solution[i]) {
				solution_c.push_back(e);
				count++;
		}
		assert(independency(solution_c));
	}
	*/
	Line_rect::debug_rect();
};
#endif
//!DEBUG---------------debug------------------------


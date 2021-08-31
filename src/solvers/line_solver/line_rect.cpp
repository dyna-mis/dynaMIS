#include "line_rect.h"

void Line_rect::set(const shared_ptr<rect_problem>& P) {
	solver_line<interval, std::set<interval, lexico_Interval>>::set(P);

	lines = vector<std::set<interval, lexico_Interval>>(numberH, std::set<interval, lexico_Interval>());
	solution = vector<std::set<interval, lexico_Interval>>(numberH, std::set<interval, lexico_Interval>());
	trees = vector<range_tree>(numberH, range_tree());

	int y;
	for (const auto& p : rect_problem_ref->pVector) {
		y = ceil(p.y()/ label_height);
		// interval
		lines[y].insert(interval(p.x(), p.x()+p.width_single, p.multiplier, p.y()));
	}
	for (size_t i = 0; i < numberH; i++) {
		if(lines[i].size()> 0){
			std::vector<interval> v(lines[i].begin(), lines[i].end());
			trees[i].set(v);
		}
	}
};





// get maximum independent set of one line
void Line_rect::get_MIS_line(size_t grid_y) {
	solution[grid_y].clear();
	double start = -1;
	interval it = trees[grid_y].search(start);
	while (it.second != DBL_MAX) {
		solution[grid_y].insert(it);
		start = it.second;
		it = trees[grid_y].search(start);
	}
}

void  Line_rect::continue_MIS_line(const interval& p, size_t grid_y) {
	double start = p.second;
	auto candidateT = solution[grid_y].upper_bound(p);
	interval it = trees[grid_y].search(start);
	while (it.second != DBL_MAX) {
		if (candidateT != solution[grid_y].end() && it == *candidateT) {
			return;
		}
		solution[grid_y].insert(it);
		if (candidateT != solution[grid_y].end()) {
			solution[grid_y].erase(candidateT);
		}

		start = it.second;
		candidateT = solution[grid_y].upper_bound(it);
		it = trees[grid_y].search(start);
	}
}

void Line_rect::init() {
	for (int i = 0; i < numberH; i++) {
		get_MIS_line(i);
	}
	solver_line<interval, std::set<interval, lexico_Interval>>::init();
};
//-------------------------debug----------------------------------


void Line_rect::recompute() {
	solution = vector<std::set<interval, lexico_Interval>>(numberH, std::set<interval, lexico_Interval>());
	int y;
	sumE = 0;
	sumO = 0;
	markE = false;
	init();

};
bool Line_rect::add_point_rect_recompute(const label_point& p) {
	Solver::add_point_rect(p);
	int y = ceil(p.y() / label_height);
	const interval I(p.x(), p.x() + p.width_single, p.multiplier, p.y());
	lines[y].insert(I);
	trees[y].add_element(I);
	recompute();
	return true;
};
bool Line_rect::delete_point_rect_recompute(size_t index) {
	label_point p = rect_problem_ref->pVector[index];
	const interval I(p.x(), p.x() + p.width_single, p.multiplier, p.y());
	int y = ceil(p.y() / label_height);
	if (mVector[index] == 1) {
		solution[y].erase(I);
	}
	Solver::delete_point_rect(index);

	lines[y].erase(I);
	trees[y].delete_element(I);

	recompute();
	return true;
};

bool Line_rect::add_point_rect(const label_point& p)
{
	bool updated = false;
	Solver::add_point_rect(p);
	int y = ceil(p.y() / label_height);
	const interval I(p.x(), p.x() + p.width_single, p.multiplier, p.y());
	lines[y].insert(I);
	trees[y].add_element(I);
	unsigned int oldSize = solution[y].size();
	if (oldSize == 0) {
		solution[y].insert(I);
		add_greedy(I);
		if ((y % 2) == 0) sumE++;
		else sumO++;
		markE = (sumE > sumO);
		return true;
	}
	//std:: cout<< "point A"<< std::endl;
	auto next = solution[y].lower_bound(I);
	if (next != solution[y].begin()) {
		auto prev_i = std::prev(next);
		if (I.second > (*prev_i).second) return false;
		else {
			solution[y].erase(*prev_i);
			solution[y].insert(I);
			add_greedy(I);
			updated = true;
			continue_MIS_line(I, y);
		}
	}
	else {
		if (I.second < (*next).second) {
			solution[y].erase(next);
			solution[y].insert(I);
			add_greedy(I);
			updated = true;
			continue_MIS_line(I, y);

		}
	}

	//assert(solution[y].size() >= oldSize);
	if (solution[y].size() > oldSize) {
		//assert(solution[y].size()  == oldSize+1);
		if ((y % 2) == 0) sumE++;
		else sumO++;
		markE = (sumE > sumO);
	}
	return updated;
}





bool Line_rect::delete_point_rect(size_t index)
{
	label_point p = rect_problem_ref->pVector[index];
	Solver::delete_point_rect(index);
	const interval I(p.x(), p.x() + p.width_single, p.multiplier, p.y());
	int y = ceil(p.y() / label_height);
	interval temp;
	unsigned int oldSize = solution[y].size();
	lines[y].erase(I);
	trees[y].delete_element(I);

	std::set<interval>::iterator it;
	it = solution[y].find(I);


	// if p is in the solution
	if (it != solution[y].end()) {
		if (it != solution[y].begin()) {
			auto prev = std::prev(it);
			temp = *prev;
			solution[y].erase(I);
			continue_MIS_line(temp, y);
		}
		//if p is the start
		else {
			solution[y].erase(I);
			if (lines[y].size() > 0) {
				const interval dymmy_start(-DBL_MAX, -DBL_MAX);
				continue_MIS_line(dymmy_start, y);
			}
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
};

void Line_rect::set_solution() {
	std::fill(mVector.begin(), mVector.end(), 0);
	int c = (markE == true) ? 0 : 1;
	// should in the line
	for (int i = c; i < numberH; i+=2) {
		for (const auto& s : solution[i]) {
			label_point p = label_point(s.first, s.height, s.multiplier);
			int index = rect_problem_ref->look_up(p);
			mVector[index] = 1;

		}
	}
};

//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
void Line_rect::print() {
	set_solution();
	solver_line<interval, std::set<interval, lexico_Interval>>::print();

};
void Line_rect::draw(cairo_t* cr) {
	set_solution();
	solver_line<interval, std::set<interval, lexico_Interval>>::draw(cr);




};
#endif
//xPRINT---------------PRINT-------------------------

//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
void Line_rect::debug_rect() {
	set_solution();
	int i = 0;
	for (auto& t : trees) {
		if (lines[i].size() > 0) {
			t.debug(lines[i]);
		}
		i++;
	}


	/*
	// every line real MIS
	i = 0;
	for (auto line : lines) {
		assert(independency(solution[i]));
		i++;
	}
	*/


	//solver_line<interval, std::set<interval, lexico_Interval>>::debug_rect();
};
#endif
//!DEBUG---------------debug------------------------
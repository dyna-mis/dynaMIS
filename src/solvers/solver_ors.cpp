#include "solver_ors.h"

void solver_ors::set(const shared_ptr<Problem>& P) {
	Solver::set(P);
	for (auto it = problem_ref->pVector.begin(); it != problem_ref->pVector.end(); ++it) {
		pSet.push_back(*it);
	}
};

void solver_ors::set(const shared_ptr<rect_problem>& P) {
	Solver::set(P);
	int i = 0;
	for (auto it = rect_problem_ref->pVector.begin(); it != rect_problem_ref->pVector.end(); ++it) {
		double c_x = it->x();
		double c_y = it->y();
		double c_y_upper = c_y + label_height;
		for (size_t j = 0; j <= it->multiplier; j++) {
			pSet.insert(Point_2(c_x, c_y));
			pSet.insert(Point_2(c_x, c_y_upper));
			corner_luTable[pair<double, double>(c_x, c_y)] = i;
			corner_luTable[pair<double, double>(c_x, c_y_upper)] = i;
			c_x += label_width;
		}
		i++;
	}
};

bool solver_ors::add_point(const Point_2& p) {
	Solver::add_point(p);
	pSet.insert(p);
	return true;
};

bool solver_ors::add_point_rect(const label_point& p) {
	size_t new_index = rect_problem_ref->point_size();
	Solver::add_point_rect(p);
	double c_x = p.x();
	double c_y = p.y();
	double c_y_upper = c_y + label_height;
	for (size_t i = 0; i <= p.multiplier; i++) {
		pSet.insert(Point_2(c_x, c_y));
		corner_luTable[pair<double, double>(c_x, c_y)] = new_index;


		pSet.insert(Point_2(c_x, c_y_upper));
		corner_luTable[pair<double, double>(c_x, c_y_upper)] = new_index;
		c_x += label_width;
	}
	return true;
};
bool solver_ors::delete_point(size_t index) {
	pSet.remove(pSet.lookup(problem_ref->pVector[index]));
	Solver::delete_point(index);
	return true;
};


bool solver_ors::delete_point_rect(size_t index) {
	label_point p_replace = rect_problem_ref->pVector[rect_problem_ref->pVector.size() - 1];
	label_point p_to_remove = rect_problem_ref->pVector[index];
	Solver::delete_point_rect(index);

	double c_x = p_replace.x();
	double c_y = p_replace.y();
	double c_y_upper = c_y + label_height;
	for (size_t i = 0; i <= p_replace.multiplier; i++) {
		auto it = corner_luTable.find(pair<double, double>(c_x, c_y));
		if (it != corner_luTable.end()) {
			it->second = index;
		}
		it = corner_luTable.find(pair<double, double>(c_x, c_y_upper));
		if (it != corner_luTable.end()) {
			it->second = index;
		}
		c_x += label_width;
	}


	c_x = p_to_remove.x();
	c_y = p_to_remove.y();
	c_y_upper = c_y + label_height;

	for (size_t i = 0; i <= p_to_remove.multiplier; i++) {
		pSet.remove(pSet.lookup(Point_2(c_x, c_y)));
		pSet.remove(pSet.lookup(Point_2(c_x, c_y_upper)));
		corner_luTable.erase(pair<double, double>(c_x, c_y));
		corner_luTable.erase(pair<double, double>(c_x, c_y_upper));
		c_x += label_width;
	}
	return true;
}




int solver_ors::look_up(const Point_2 & Point) const
{
	doublePair p(Point.x(), Point.y());
	std::unordered_map<doublePair, int, boost::hash<doublePair>>::const_iterator got = corner_luTable.find(p);
	if (got != corner_luTable.end()) {
		return got->second;
	}
	return -1;
};



//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
void solver_ors::print() {
	Solver::print();
	std::cout << "pSet" << std::endl;
	vector<Point_2> points;
	getPointSet(pSet, points, width, height);
	for (auto e : points) {
		std::cout << "(" << e.x() << ", " << e.y() << ") ";
	}


}
void solver_ors::print_rect() {
	Solver::print_rect();
	std::cout << "pSet" << std::endl;
	vector<Point_2> points;
	getPointSet(pSet, points, width, height);
	for (auto e : points) {
		std::cout << "(" << e.x() << ", " << e.y() << ") ";
	}

	std::cout << "luTable: " << corner_luTable.size() << std::endl;
	int i = 0;
	for (const auto& e : corner_luTable) {
		std::cout << i << " key:(" << e.first.first << "," << e.first.second << ") index:" << e.second << std::endl;
		i++;
	}


}
#endif
//xPRINT---------------PRINT-------------------------

//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
void solver_ors::debug() {
	Solver::debug();
	for (unsigned int i = 0; i < problem_ref->pVector.size(); i++) {
		pSet.lookup(problem_ref->pVector[i]) != NULL;
	}
	// how to get all points in a set?
	vector<Point_2> points;
	getPointSet(pSet, points, width, height);
	assert(points.size() == problem_ref->pVector.size());
	for (const auto& e : points) {
		assert(std::find(problem_ref->pVector.begin(), problem_ref->pVector.end(), e) != problem_ref->pVector.end());
	}


}
void solver_ors::debug_rect() {
	Solver::debug_rect();
	for (unsigned int i = 0; i < rect_problem_ref->pVector.size(); i++) {
		double c_x = rect_problem_ref->pVector[i].x();
		double c_y = rect_problem_ref->pVector[i].y();
		double c_y_upper = c_y + label_height;
		for (size_t j = 0; j <= rect_problem_ref->pVector[i].multiplier; j++) {
			pSet.lookup(Point_2(c_x, c_y)) != NULL;
			pSet.lookup(Point_2(c_x, c_y_upper)) != NULL;
			c_x += label_width;
		}

	}
	// how to get all points in a set?
	vector<Point_2> points;
	getPointSet(pSet, points, width, height);
	for (auto e : points) {
		int i = look_up(e);
		assert(i > -1);
		label_point ori = rect_problem_ref->pVector[i];
		double x_factor = (e.x() - ori.x()) / label_width;
		double intpart;
		double fractpart = modf(x_factor, &intpart);
		assert( fractpart < 0.01 || fractpart > 0.99);
		assert (intpart >= 0 && intpart <= ori.multiplier);
		assert(ori.y() == e.y() ||e.y() == ori.y() + label_height);
	}
}
#endif
//!DEBUG---------------debug------------------------
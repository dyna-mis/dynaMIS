/*
 * problem_rs.cpp
 *
 *  Created on: Nov 13, 2019
 *      Author: guangping
 */
#include "mis_ors.h"


void MIS_ors::set(const shared_ptr<Problem>& P) {
	solver_ors::set(P);
}

void MIS_ors::init() {
	size_t size = problem_ref->pVector.size();
	std::list<Vertex_handle> LV;
	std::list<Vertex_handle>::const_iterator itv;
	unsigned int  v_index;
	for (unsigned u_index = 0; u_index < size; u_index++) {
		if (mVector[u_index] == 0) {
			mVector[u_index] = 1;
			mSet.push_back(problem_ref->pVector[u_index]);
			LV.clear();
			rangeSearch_neighbor(pSet, problem_ref->pVector[u_index], LV);
			for (itv = LV.begin(); itv != LV.end(); itv++) {
				v_index = problem_ref->look_up((*itv)->point());
				if (v_index <= u_index) continue;
				mVector[v_index] = -1;
			}
		}
	}
}

void MIS_ors::set_solution() {};

void MIS_ors::recompute() {
	size_t size = problem_ref->pVector.size();
	mVector = vector<short>(2*size, 0);
	mSet.clear();
	std::list<Vertex_handle> LV;
	std::list<Vertex_handle>::const_iterator itv;
	unsigned int  v_index;
	for (unsigned u_index = 0; u_index < size; u_index++) {
		if (mVector[u_index] == 0) {
			mVector[u_index] = 1;
			mSet.push_back(problem_ref->pVector[u_index]);
			LV.clear();
			rangeSearch_neighbor(pSet, problem_ref->pVector[u_index], LV);
			for (itv = LV.begin(); itv != LV.end(); itv++) {
				v_index = problem_ref->look_up((*itv)->point());
				if (v_index <= u_index) continue;
				mVector[v_index] = -1;
			}
		}
	}
}


bool MIS_ors::add_point_recompute(const Point_2& p) {
	solver_ors::add_point(p);
	recompute();
	return true;

}

bool MIS_ors::delete_point_recompute(size_t index) {
	if (mVector[index] == 1) {
		mSet.remove(mSet.lookup(problem_ref->pVector[index]));
	}
	solver_ors::delete_point(index);
	recompute();
	return true;
}



bool MIS_ors::add_point(const Point_2& p){
	solver_ors::add_point(p);
	std::list<Vertex_handle> I;
	size_t index = problem_ref->point_size() - 1;
	I.clear();
	rangeSearch_neighbor(mSet, p, I);
	if (I.size() == 0) {
		mSet.insert(p);
		mVector[index] = 1;
	}
	else  mVector[index] = -1;

	return true;
};
bool MIS_ors::delete_point(size_t index) {

	if (mVector[index] == 1) {
		mSet.remove(mSet.lookup(problem_ref->pVector[index]));

		isoPoly R_free = isoPoly(1000);
		double x = problem_ref->pVector[index].x();
		double y = problem_ref->pVector[index].y();
		isoRect R_1 = isoRect(x - label_height, y - label_height, x + label_height, y + label_height);
		R_free.add(R_1);
		std::list<Vertex_handle> I_2;
		I_2.clear();
		rangeSearch(mSet, problem_ref->pVector[index], 2 * label_height, I_2);
		for (std::list<Vertex_handle>::const_iterator it = I_2.begin(); it != I_2.end(); it++) {
			isoRect cutter = isoRect((*it)->point().x() - label_height, (*it)->point().y() - label_height, (*it)->point().x() + label_height, (*it)->point().y() + label_height);
			R_free.cut(cutter);
		}
		std::list<Vertex_handle> w;
		while (true) {
			w.clear();
			R_free.rangeSearch(pSet, w);
			if (w.size() == 0) {
				break;
			}
			if (w.size() == 1) {
				if (problem_ref->look_up((*(w.begin()))->point()) == index) {
					//std::cout<< "w contains only the deleted one"<< std::endl;
					break;
				}
			}

			for (std::list<Vertex_handle>::const_iterator it = w.begin(); it != w.end(); it++) {

				if ((*it)->point() == problem_ref->pVector[index]) continue;
				size_t to_add_index = problem_ref->look_up((*it)->point());

				mVector[to_add_index] = 1;
				mSet.push_back(problem_ref->pVector[to_add_index]);

				isoRect cutter = isoRect((*it)->point().x() - label_height, (*it)->point().y() - label_height, (*it)->point().x() + label_height, (*it)->point().y() + label_height);

				R_free.cut(cutter);
				break;
			}
		}
	}
	solver_ors::delete_point(index);
	return true;
};


//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
void MIS_ors::print() {
	solver_ors::print();
	std::cout << "mSet" << std::endl;
	vector<Point_2> points;
	getPointSet(mSet, points, width, height);
	for (auto e : points) {
		std::cout << "(" << e.x() << ", " << e.y() << ") ";
	}

};
void MIS_ors::print_rect() {
	solver_ors::print_rect();
	std::cout << "mSet" << std::endl;
	vector<Point_2> points;
	getPointSet(mSet, points, width, height);
	for (const auto& e : points) {
		std::cout << "(" << e.x() << ", " << e.y() << ") ";
		int i = look_up(e);
		label_point ori = rect_problem_ref->pVector[i];
		print_point(ori);
	}
};
#endif
//xPRINT---------------PRINT-------------------------

//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
void MIS_ors::debug() {
	solver_ors::debug();
	vector<Point_2> points;
	getPointSet(mSet, points, width, height);
	// no conflict
	assert(independency(points));
	// all in pSet
	for (auto e : points) {
		assert(std::find(problem_ref->pVector.begin(), problem_ref->pVector.end(), e) != problem_ref->pVector.end());
	}
	//exact mVector "1"s
	unsigned int index;
	for (auto e : points) {
		index = problem_ref->look_up(e);
		assert(index >= 0);
		assert(index < problem_ref->pVector.size());
		assert(mVector[index] == 1);
	}
	for (unsigned int i = 0; i < problem_ref->pVector.size(); i++) {
		if (mVector[i] == 1) {
			assert(std::find(points.begin(), points.end(), problem_ref->pVector[i]) != points.end());
		}
	}


};
void MIS_ors::debug_rect() {
	solver_ors::debug_rect();
	vector<Point_2> points;
	getPointSet(mSet, points, width, height);
	//exact mVector "1"s
	unsigned int index;
	for (auto e : points) {
		index = look_up(e);
		assert(index >= 0);
		assert(index < problem_ref->pVector.size());
		assert(mVector[index] == 1);
	}
	for (unsigned int i = 0; i < problem_ref->pVector.size(); i++) {
		if (mVector[i] == 1) {
			double c_x = rect_problem_ref->pVector[i].x();
			double c_y = rect_problem_ref->pVector[i].y();
			double c_y_upper = c_y + label_height;
			for (size_t i = 0; i <= rect_problem_ref->pVector[i].multiplier; i++) {
				mSet.lookup(Point_2(c_x, c_y)) != NULL;
				mSet.lookup(Point_2(c_x, c_y_upper)) != NULL;
				c_x += rect_problem_ref->pVector[i].width_single;
			}
		}
	}





};
#endif
//!DEBUG---------------debug------------------------


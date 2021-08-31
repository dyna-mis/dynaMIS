/*
 * problem_cg.cpp
 *
 *  Created on: Nov 13, 2019
 *      Author: guangping
 */
#include "mis_cg.h"


void MIS_cg::set(const shared_ptr<Problem>& P) {
	solver_graph::set(P);
	cVector = vector<unsigned int>(problem_ref->point_size()* 2, 0);
};
void MIS_cg::set(const shared_ptr<rect_problem>& P) {
	solver_graph::set(P);
	cVector = vector<unsigned int>(rect_problem_ref->point_size() * 2, 0);
};

void MIS_cg::init(){
	size_t size = conflict_graph.get_vertex_size();
	for (unsigned u_index = 0; u_index < size; u_index++){
		//assert(mVector[u_index] == -1||mVector[u_index]== 0 );
		if(mVector[u_index] == 0){
			mVector[u_index] = 1;
			for (auto v_index: conflict_graph.adList[u_index]){
				cVector[v_index]++;
				mVector[v_index] = -1;
			}
		}
	}
}

void MIS_cg::recompute() {
	size_t size = conflict_graph.get_vertex_size();
	mVector = vector<short>(2*size, 0);
	cVector = vector<unsigned int>(size*2, 0);
	for (unsigned u_index = 0; u_index < size; u_index++) {
		//assert(mVector[u_index] == -1||mVector[u_index]== 0 );
		if (mVector[u_index] == 0) {
			mVector[u_index] = 1;
			for (auto v_index : conflict_graph.adList[u_index]) {
				cVector[v_index]++;
				mVector[v_index] = -1;
			}
		}
	}
}


bool MIS_cg::delete_point(size_t index){
	if (mVector[index] == 1) {
		mVector[index] = -1;
		for (auto n_index : conflict_graph.adList[index]) {
			cVector[n_index]--;
			if (cVector[n_index] == 0) {
				for (auto n_n_index : conflict_graph.adList[n_index]) {
					cVector[n_n_index]++;
				}
				mVector[n_index] = 1;
			}
		}
	}
	cVector[index] = cVector[problem_ref->point_size()-1];
	solver_graph::delete_point(index);
	return true;
}

bool MIS_cg::delete_point_recompute(size_t index) {
	solver_graph::delete_point(index);
	recompute();
	return true;
}





bool MIS_cg::add_point_rect(const label_point& p) {
	solver_graph::add_point_rect(p);

	size_t count = 0;
	int index = rect_problem_ref->point_size() - 1;
	const auto& neighbors = conflict_graph.adList[index];
	for (const auto& e : neighbors) {
		if (mVector[e] == 1) {
			count++;
		}
	}
	cVector[index] = count;
	//solution part
	if (count > 0) {
		mVector[index] = -1;
		return false;
	}
	else {
		mVector[index] = 1;
		for (auto e : neighbors) {
			cVector[e]++;
		}
	}


	return true;

};


bool MIS_cg::add_point_rect_recompute(const label_point& p) {
	solver_graph::add_point_rect(p);
	recompute();
	return true;

};


bool MIS_cg::delete_point_rect(size_t index) {
	if (mVector[index] == 1) {
		mVector[index] = -1;
		for (auto n_index : conflict_graph.adList[index]) {
			cVector[n_index]--;
			if (cVector[n_index] == 0) {
				for (auto n_n_index : conflict_graph.adList[n_index]) {
					cVector[n_n_index]++;
				}
				mVector[n_index] = 1;
			}
		}
	}
	cVector[index] = cVector[rect_problem_ref->point_size() - 1];
	solver_graph::delete_point_rect(index);
	return true;
};

bool MIS_cg::delete_point_rect_recompute(size_t index) {
	solver_graph::delete_point_rect(index);
	recompute();
	return true;
}

bool MIS_cg::add_point(const Point_2& p) {
	solver_graph::add_point(p);
	size_t count = 0;
	int index = problem_ref->point_size() - 1;
	const auto& neighbors = conflict_graph.adList[index];
	for (const auto& e : neighbors) {
		if (mVector[e] == 1) {
			count++;
		}
	}
	cVector[index] = count;
	//solution part
	if (count > 0) {
		mVector[index] = -1;
		return false;
	}
	else {
		mVector[index] = 1;
		for (auto e : neighbors) {
			cVector[e]++;
		}
	}
	return true;
};


bool MIS_cg::add_point_recompute(const Point_2& p) {
	solver_graph::add_point(p);
	recompute();
	return true;
}
void MIS_cg::set_solution() {};

//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
void MIS_cg::print() {
	solver_graph::print();
	std::cout << "counterVector" << std::endl;
	for (unsigned int i = 0; i < problem_ref->pVector.size(); i++) {
		std::cout << i << ": " << cVector[i] << std::endl;
	}

};
void MIS_cg::print_rect(){
	solver_graph::print_rect();
	std::cout << "counterVector" << std::endl;
	for (unsigned int i = 0; i < rect_problem_ref->pVector.size(); i++) {
		std::cout << i << ": " << cVector[i] << std::endl;
	}

};
#endif
//xPRINT---------------PRINT-------------------------

//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
void MIS_cg::debug(){
	solver_graph::debug();
	unsigned int c = 0;
	for (unsigned int i = 0; i < problem_ref->pVector.size(); i++) {
		for (unsigned int j = 0; j < problem_ref->pVector.size(); j++) {
			if (i == j) continue;
			if (mVector[j] == 1) {
				if (isConflict(problem_ref->pVector[i], problem_ref->pVector[j])) {
					assert(conflict_graph.containEdge(i, j));
					c++;
				}
			}
		}
		if (mVector[i] == 1) {
			assert(c == 0);
		}
		assert(c == cVector[i]);
		c = 0;
	}

};
void MIS_cg::debug_rect(){
	solver_graph::debug_rect();
	unsigned int c = 0;
	for (unsigned int i = 0; i < rect_problem_ref->pVector.size(); i++) {
		for (unsigned int j = 0; j < rect_problem_ref->pVector.size(); j++) {
			if (i == j) continue;
			if (mVector[j] == 1) {
				if (isConflict(rect_problem_ref->pVector[i], rect_problem_ref->pVector[j])) {
					c++;
				}
			}
		}
		if (mVector[i] == 1) {
			assert(c == 0);
		}
		assert(c == cVector[i]);
		c = 0;
	}

};
#endif
//!DEBUG---------------debug------------------------

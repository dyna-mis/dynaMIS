/*
 * Grid_k.cpp
 *
 *  Created on: Nov 13, 2019
 *      Author: guangping
 */
#include "grid_k.h"
void Grid_k::set(const shared_ptr<Problem>& P){
	solver_grid<std::set<Point_2>>::set(P);
	gridS = vector<vector<vector<Point_2>>>(numberH,vector<vector<Point_2>>(numberV,vector<Point_2>(param_k))) ;
	preGridS = vector<vector<vector<Point_2>>>(numberH,vector<vector<Point_2>>(param_k -1,vector<Point_2>(param_k)));

};
void Grid_k::init(){
	for(int i = 0; i < numberH; i++){
		for(int j = 0; j < numberV; j++){
			get_MIS_k(j,i);
			counter[i][j%(param_k +1)]+= gridS[i][j].size();
		}
		for(int j = 0; j < param_k -1; j++){
			get_MIS_pre(j,i);
			counter[i][(j+2)%(param_k +1)]+= preGridS[i][j].size();
		}
	}
	solver_grid<std::set<Point_2>>::init();
}
void Grid_k::recompute()
{
	gridS = vector<vector<vector<Point_2>>>(numberH, vector<vector<Point_2>>(numberV, vector<Point_2>(param_k)));
	preGridS = vector<vector<vector<Point_2>>>(numberH, vector<vector<Point_2>>(param_k - 1, vector<Point_2>(param_k)));
	solver_grid<std::set<Point_2>>::recompute();
	init();

}



bool Grid_k::add_point_recompute(const Point_2& p) {
	solver_grid<std::set<Point_2>>::add_point(p);
	recompute();
	return true;
};
bool Grid_k::delete_point_recompute(size_t index) {
	solver_grid<std::set<Point_2>>::delete_point(index);
	recompute();
	return true;
};


// get maximum independent set of k lines
void Grid_k::get_MIS_k(int grid_x, int grid_y) {
	Point_2 start = Point_2(-1, -1);
	int x = grid_x;
	vector<Point_2> solution(param_k);
	solution.clear();
	for (int i = 0; i < param_k; i++) {
		auto it = gridP[grid_y][x].lower_bound(start);
		if (it != gridP[grid_y][x].end()) {
			solution.push_back(*it);
			start = (*it) + Vector(label_width, 0);
		}
		x++;
		if (x == numberV) break;
	}
	gridS[grid_y][grid_x] = solution;
}
//stored in x, recored [0...x]
void Grid_k::get_MIS_pre(int grid_x, int grid_y){
	Point_2 start = Point_2(-1,-1);
	vector<Point_2> solution(param_k);
	solution.clear();
	for(int i =0; i <= grid_x; i++){
		auto it = gridP[grid_y][i].lower_bound(start);
		if(it != gridP[grid_y][i].end()){
			solution.push_back(*it);
			start = (*it)+Vector(label_width,0);
		}
	}
	preGridS[grid_y][grid_x]= solution;
	//std::cout<< "out getMIS_pre "<< sum << std::endl;
}


// if one point added
//(it can change k gridS containing its grid)
//(it can change until k-1 preGridS containing its grid)
bool  Grid_k::add_point(const Point_2& p) {
	solver_grid<std::set<Point_2>>::add_point(p);
	int x = ceil((p.x() - h_label_width) / label_width);
	int y = ceil((p.y() - h_label_height) / label_height);
	int x_index;
	// update grid
	for (int i = 0; i < param_k; i++) {
		x_index = x - i;
		if (x_index >= 0) add_point_recompute_update(x_index, y);
	}
	//update preGrid
	if (x < param_k - 1) {
		for (int i = x; i < param_k - 1; i++) {
			add_point_recompute_pre_update(i, y);
		}
	}
	return true;
};

// for gridS, gridC
void Grid_k::add_point_recompute_update(int x, int y) {
	int& sum = ((y % 2) == 0) ? sumE : sumO;
	int oldCount = gridS[y][x].size();
	int oldMAX = counter[y][mark[y]];
	get_MIS_k(x, y);
	int index;
	int newCount = gridS[y][x].size();
	if (newCount > oldCount) {
		index = x % (param_k + 1);
		//assert(newCount == oldCount+1);
		counter[y][index]++;
		if (counter[y][index] > oldMAX) {
			mark[y] = index;
			sum++;
			markE = (sumE > sumO);
		}
	}
}

//assert(x < k-1)
// for preGridS
void Grid_k::add_point_recompute_pre_update(int x, int y) {
	int& sum = ((y % 2) == 0) ? sumE : sumO;
	int oldCount = preGridS[y][x].size();
	int index = (x + 2) % (param_k + 1);
	int oldMAX = counter[y][mark[y]];
	get_MIS_pre(x, y);
	int newCount = preGridS[y][x].size();
	if (newCount > oldCount) {
		counter[y][index]++;
		if (counter[y][index] > oldMAX) {
			mark[y] = index;
			sum++;
			markE = (sumE > sumO);
		}
	}
}



bool Grid_k::delete_point(size_t index) {
	Point_2 p = problem_ref->pVector[index];
	int x = ceil((p.x() - h_label_width) / label_width);
	int y = ceil((p.y() - h_label_height) / label_height);
	solver_grid<std::set<Point_2>>::delete_point(index);

	int x_index;
	// update grid
	for(int i = 0; i < param_k;i++){
		x_index = x-i;
		if(x_index >= 0) delete_point_recompute_update(x_index,y);
	}
	//update preGrid
	if(x < param_k -1){
		for(int i = x; i < param_k -1; i++){
			delete_point_recompute_pre_update(i,y);
		}
	}
	return true;
};
// for gridS, gridC
void Grid_k::delete_point_recompute_update(int x, int y){
	int oldCount = gridS[y][x].size();
	int& sum = (y%2==0)? sumE:sumO;
	get_MIS_k(x, y);
	int index;
	int max_oldValue = -1;
	int max_value = -1;
	int index_candidate= -1;
	int max_index = -1;
	int newCount = gridS[y][x].size();
	if(newCount < oldCount){
		//assert(newCount == oldCount-1);
		index = x%(param_k +1);
		counter[y][index]--;
		if(mark[y]== index){
			max_oldValue = counter[y][index]+1;
			for(auto e:counter[y]){
				index_candidate++;
				if(e > max_value) {
					max_index = index_candidate;
					max_value = e;
				}
			}
			mark[y] =max_index;

			sum -= max_oldValue;
			sum+= max_value;
			markE = (sumE > sumO);
		}
	}
}
// for preGridS, preGridC
void Grid_k::delete_point_recompute_pre_update(int x, int y){
	//assert(x < k-1);
	int index_candidate= -1;
	int max_value = -1;
	int max_index = -1;
	int max_oldValue = -1;
	int& sum = ((y%2)==0)? sumE:sumO;
	int oldCount = preGridS[y][x].size();
	int index = (x+2)%(param_k +1);
	get_MIS_pre(x, y);
	int newCount = preGridS[y][x].size();
	if(newCount < oldCount){
		counter[y][index]--;

		if(mark[y]== index){
			max_oldValue = counter[y][index]+1;

			for(auto e:counter[y]){
				index_candidate++;
				if(e > max_value) {
					max_index = index_candidate;
					max_value = e;
				}
			}
			mark[y] =max_index;
			sum -= max_oldValue;
			sum+= max_value;
			markE = (sumE > sumO);
		}
	}
}


void Grid_k::set_solution() {
	std::fill(mVector.begin(), mVector.end(), 0);
	int c = (markE == true) ? 0 : 1;
	for (int line = 0; line < numberH; line++) {
		if (line % 2 != c)continue;
		for (int i = 0; i < numberV; i++) {
			if (i % (param_k + 1) == mark[line]) {
				for (auto e : gridS[line][i]) {
					size_t index = problem_ref->look_up(e);
					mVector[index] = 1;
				}
			}
		}
		if (mark[line] - 2 > -1) {
			for (auto e : preGridS[line][mark[line] - 2]) {
				size_t index = problem_ref->look_up(e);
				mVector[index] = 1;
			}
		}
	}
};


//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
void Grid_k::print() {
	set_solution();
	solver_grid<std::set<Point_2>>::print();

	std::cout << "The grid solution:" << std::endl;
	for (int i = 0; i < numberH; i++) {
		for (int j = 0; j < numberV; j++) {
			for (auto v : gridS[i][j]) {
				std::cout << "[" << v.x() << " , " << v.y() << "]  ";
			}
		}
		std::cout << std::endl;
	}
	std::cout << "The preGrid:" << std::endl;
	int line = 0;
	int grid = 0;
	for (auto l : preGridS) {
		grid = 0;
		for (auto g : l) {
			for (auto e : g) {
				std::cout << "(" << e.x() << " , " << e.y() << ")  ";
			}
			std::cout << "***" << preGridS[line][grid].size() << "***";
			grid++;

		}
		std::cout << endl;
		line++;
	}


};
void Grid_k::draw(cairo_t* cr) {
	set_solution();
	solver_grid<std::set<Point_2>>::draw(cr);


};
#endif
//xPRINT---------------PRINT-------------------------

//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
void Grid_k::debug() {
	set_solution();
	solver_grid<std::set<Point_2>>::debug();
};
#endif
//!DEBUG---------------debug------------------------
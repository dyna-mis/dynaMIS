/*
 * problem_slk_greedy.cpp
 *
 *  Created on: Nov 13, 2019
 *      Author: guangping
 */

#include "grid_k_greedy.h"
void Grid_k_greedy::set(const shared_ptr<Problem>& P){
	Grid_k::set(P);
	//greedy solution
	greedy_gridP = vector<vector<Point_2>>(numberH,vector<Point_2>(numberV,Point_2(-1,-1))) ;
	greedy_counter = vector<vector<int>> (numberH,vector<int>((param_k +1),0));

	//greedy between
	greedyKK = vector<vector<vector<vector<Point_2>>>>(numberH,vector<vector<vector<Point_2>>>(param_k +1,vector<vector<Point_2>>(param_k +1,vector<Point_2>(numberV,Point_2(-1,-1)))));
	greedyKKSize = vector<vector<vector<int>>>(numberH,vector<vector<int>>(param_k +1,vector<int>(param_k +1,0)));
};

void Grid_k_greedy::init() {
	Grid_k::init();
	greedy_Line();
	greedy_Between();
}

void Grid_k_greedy::recompute(){
	Grid_k::recompute();
	greedy_gridP = vector<vector<Point_2>>(numberH, vector<Point_2>(numberV, Point_2(-1, -1)));
	greedy_counter = vector<vector<int>>(numberH, vector<int>((param_k + 1), 0));

	greedyKK = vector<vector<vector<vector<Point_2>>>>(numberH, vector<vector<vector<Point_2>>>(param_k + 1, vector<vector<Point_2>>(param_k + 1, vector<Point_2>(numberV, Point_2(-1, -1)))));
	greedyKKSize = vector<vector<vector<int>>>(numberH, vector<vector<int>>(param_k + 1, vector<int>(param_k + 1, 0)));
	vector<Point_2> solution(numberV, Point_2(-1, -1));
	Point_2 prev;
	Point_2 back;
	size_t s;
	size_t chosen = (markE == true) ? 0 : 1;
	for (size_t i = chosen; i < numberH; i+=2) { 
		size_t j = mark[i];
			solution = vector<Point_2>(numberV, Point_2(-1, -1));
			get_group_index(solution, i, j);
			s = (j + param_k) % (param_k + 1);
			for (size_t q = s; q < numberV;) {
				if (q > 0 && solution[q - 1] != Point_2(-1, -1)) {
					prev = solution[q - 1] + Vector(label_width, 0);
				}
				else prev = Point_2(-1, -1);
				if (q < numberV - 1 && solution[q + 1] != Point_2(-1, -1)) {
					back = solution[q + 1] + Vector(-label_width, 0);
				}
				else back = Point_2(DBL_MAX, -1);
				auto it = gridP[i][q].lower_bound(prev);
				if (it != gridP[i][q].end() && (*it).x() <= back.x()) {
					//std::cout<< "q:-----------2  must"<< q<< std::endl;
					greedy_gridP[i][q] = *it;
					greedy_counter[i][j]++;
					q += param_k + 1;
					continue;
				}
				q += param_k + 1;
			}
	}



	vector<Point_2> solution_below;
	vector<Point_2> solution_above;
	double last = -1;
	bool extendible_upper;
	bool extendible_below;
	size_t not_chosen = (markE == true) ? 1 : 0;
	for (size_t i = not_chosen; i < numberH; i+=2) {
				solution_below.clear();
				solution_above.clear();
				last = -1;
				// it has below
				if (i > 0) {
					get_group(solution_below, i - 1, mark[i-1]);
					get_group_greedy(solution_below, i - 1, mark[i - 1]);
				}
				//it has upper
				if (i < numberH - 1) {
					get_group(solution_above, i + 1, mark[i+1]);
					get_group_greedy(solution_above, i + 1, mark[i + 1]);
				}
				for (int j = 0; j < numberV; j++) {
					for (auto e : gridP[i][j]) {
						if (e.x() < last) continue;
						extendible_below = extendible(solution_below, e, label_width, label_height);
						extendible_upper = extendible(solution_above, e, label_width, label_height);
						if (extendible_below && extendible_upper) {
							size_t below = 0;
							size_t above = 0;
							if (i > 0) {
								below = mark[i - 1];
							}
							if (i < numberH - 1) {
								above = mark[i + 1];
							}

							greedyKK[i][above][below][j] = e;
							greedyKKSize[i][above][below]++;
							last = e.x() + label_width;
							break;
						}
					}
				}
	}

}


bool Grid_k_greedy::add_point_recompute(const Point_2& p) {
	solver_grid<std::set<Point_2>>::add_point(p);
	recompute();
	return true;
};

bool Grid_k_greedy::delete_point_recompute(size_t index) {
	solver_grid<std::set<Point_2>>::delete_point(index);
	recompute();
	return true;
};

void Grid_k_greedy::greedy_Line(){
	//std::cout<< "in greedy_LINE"<< std::endl;
	vector<Point_2> solution(numberV,Point_2(-1,-1));
	Point_2 prev;
	Point_2 back;
	int s;
	for(int i = 0; i < numberH; i++){
		for(int j = 0;  j< param_k +1; j++){
			solution=vector<Point_2>(numberV,Point_2(-1,-1));
			get_group_index(solution,i,j);

			if (j > 0) s = j - 1;
			else s = param_k;
			
			for(size_t q = s; q < numberV;){
				if(q >0 && solution[q-1] != Point_2(-1,-1)){
					prev = solution[q-1]+Vector(label_width,0);
				}
				else prev = Point_2(-1,-1);
				if(q < numberV-1 && solution[q+1] != Point_2(-1,-1)){
					back = solution[q+1]+Vector(-label_width,0);
				}
				else back = Point_2(DBL_MAX,-1);
				auto it = gridP[i][q].lower_bound(prev);

				if(it != gridP[i][q].end()&& (*it).x()<= back.x()){
					greedy_gridP[i][q] = *it;
					greedy_counter[i][j]++;
					q+= param_k +1;
					continue;
				}
				q+= param_k +1;
			}
		}
	}
}

// get exact solution from one group
// no-index, no dummy
void Grid_k_greedy::get_group_index(vector<Point_2>& solution, int line, int group) {
	if (line < 0 || line >= numberH) return;
	int grid;
	for (int i = group; i < numberV; i+= param_k +1) {
		for (auto e : gridS[line][i]) {
			grid = ceil((e.x() - h_label_width) / label_width);
			solution[grid] = e;
		}
	}
	if (group > 1) {
		for (auto e : preGridS[line][group - 2]) {
			grid = ceil((e.x() - h_label_width) / label_width);
			solution[grid] = e;
		}
	}
}

void Grid_k_greedy::get_group(vector<Point_2>& solution, int line, int group) {
	if (line < 0 || line >= numberH) return;
	for (int i = group; i < numberV; i += param_k + 1) {
		for (auto e : gridS[line][i]) {
			solution.push_back(e);
		}
	}
	if (group > 1) {
		for (auto e : preGridS[line][group - 2]) {
			solution.push_back(e);
		}
	}
}






// get the greedy one of one group (not include the exact ones)
void Grid_k_greedy::get_group_greedy(vector<Point_2>& solution, int line, int group) {
	if (line < 0 || line >= numberH) return;
	int count = 0;
	int s = param_k;
	if (group > 0) s = group - 1;

	for (int j = s; j < numberV; ) {
		if (greedy_gridP[line][j] != Point_2(-1, -1)) {
			solution.push_back(greedy_gridP[line][j]);
			count++;
		}
		j += param_k + 1;
	}
	//assert(count == greedy_counter[line][group]);
}


void Grid_k_greedy::greedy_Between(){
	vector<Point_2> solution_below;
	vector<Point_2> solution_above;
	double last= -1;
	bool extendible_upper;
	bool extendible_below;
	for(int i = 0; i < numberH; i++){
		for(int upper =0; upper < param_k +1; upper++){
			for(int below = 0; below < param_k +1; below ++){
				solution_below.clear();
				solution_above.clear();
				last = -1;
				// it has below
				if(i > 0){
					get_group(solution_below,i-1,below);
					get_group_greedy(solution_below,i-1,below);
				}
				//it has upper
				if(i < numberH-1){
					get_group(solution_above,i+1,upper);
					get_group_greedy(solution_above,i+1,upper);
				}
				for(int j = 0; j < numberV; j++){
					for(auto e:gridP[i][j]){
						if(e.x() < last) continue;
						extendible_below = extendible(solution_below, e,label_width,label_height);
						extendible_upper = extendible(solution_above, e, label_width, label_height);
						if(extendible_below && extendible_upper){
							greedyKK[i][upper][below][j] = e;
							greedyKKSize[i][upper][below]++;
							last = e.x()+ label_width;
							break;
						}
					}
				}
			}
		}
	}
}

// if one point added
//(it can change k gridS containing its grid)
//(it can change until k-1 preGridS containing its grid)
bool Grid_k_greedy::add_point(const Point_2& p){
	Grid_k::add_point(p);
	//std:: cout<< "in addPoint"<< std::endl;
	int x = ceil((p.x() - h_label_width) / label_width);
	int y = ceil((p.y() - h_label_height) / label_height);
	// update grid
	int x_index;

	for(int i = 0; i < param_k;i++){
		x_index = x-i;
		if(x_index >= 0) {
			add_greedy_gridP(x_index,y);
			add_greedy_between(x_index,y);
		}

	}
	//update preGrid
	if(x < param_k -1){
		for(int i = x; i < param_k -1; i++){
			add_greedy_gridP_pre(i,y);
			add_greedy_between_pre(i,y);
		}
	}
	return true;
};
//update the greedy line because the group starting with x is changed
void Grid_k_greedy::add_greedy_gridP(int x,int y){
	size_t group = x%(param_k +1);
	if(gridS[y][x].size() > 0){
		Point_2 start = gridS[y][x].front();
		Point_2 end = gridS[y][x].back();
		if(x > 0){
			if(greedy_gridP[y][x-1] != Point_2(-1,-1)&&greedy_gridP[y][x-1].x()+label_width > start.x()){
				greedy_gridP[y][x-1] = Point_2(-1,-1);
				greedy_counter[y][group]--;
			}
		}
		if(x <numberV- param_k){
			if(greedy_gridP[y][x+ param_k] != Point_2(-1,-1)&&greedy_gridP[y][x+ param_k].x() > end.x()- label_width){
				greedy_gridP[y][x+ param_k] = Point_2(-1,-1);
				greedy_counter[y][group]--;
			}
		}
	}
}
//update the greedy line because the group starting with x is changed
void Grid_k_greedy::add_greedy_gridP_pre(int x, int y) {
	size_t group = (x + 2) % (param_k + 1);
	if (preGridS[y][x].size() > 0) {
		Point_2 start = preGridS[y][x].back();
		// TODO: in the preGridS start.x maximal!
		if (greedy_gridP[y][x + 1] != Point_2(-1, -1) && greedy_gridP[y][x + 1].x() < label_width + start.x()) {
			//std:: cout<< "inner if"<< std::endl;
			greedy_gridP[y][x + 1] = Point_2(-1, -1);
			greedy_counter[y][group]--;
		}
	}
}


void Grid_k_greedy::add_greedy_between(int x,int y){
	size_t group = x%(param_k +1);
	// between y and y-2
	if(y > 0){
		for(int i = 0; i < param_k +1; i++){
			// update greedyBetween[y-1][group][i]
			for(int j = x-1; j < x+ param_k +1; j++){
				if(j < 0) continue;
				if(j> numberV-1) break;
				// if now
				if(greedyKK[y-1][group][i][j] != Point_2(-1,-1)){
					// TODO: write extendible function for a sorted solution
					if(!extendible(gridS[y][x],greedyKK[y-1][group][i][j], label_width, label_height)){
						greedyKK[y-1][group][i][j]= Point_2(-1,-1);
						greedyKKSize[y-1][group][i]--;
						//assert(greedyKKSize[y-1][group][i] >= 0);
					}
				}
			}
		}
	}
	//between y and y+2
	if(y < numberH-1){
		for(int i = 0; i < param_k +1; i++){
			//update greedyBetween[y+1][i][group]
			for(int j = x-1; j < x+ param_k +1; j++){
				if(j < 0) continue;
				if(j> numberV-1) break;
				// if now
				if(greedyKK[y+1][i][group][j] != Point_2(-1,-1)){
					// TODO: write extendible function for a sorted solution
					if(!extendible(gridS[y][x],greedyKK[y+1][i][group][j], label_width, label_height)){
						greedyKK[y+1][i][group][j]= Point_2(-1,-1);
						greedyKKSize[y+1][i][group]--;
					}
				}
			}
		}
	}
}



void Grid_k_greedy::add_greedy_between_pre(int x,int y){
	//std:: cout<< "in Grid_k_greedy::addPoint_updateGreedyBetween_pre"<< std::endl;
	int group = (x+2)%(param_k +1);
	// between y and y-2
	if(y > 0){
		for(int i = 0; i < param_k +1; i++){
			// update greedyBetween[y-1][group][i]
			for(int j = 0; j <= x+1; j++){
				// if now
				if(greedyKK[y-1][group][i][j] != Point_2(-1,-1)){
					// TODO: write extendible function for a sorted solution
					if(!extendible(preGridS[y][x],greedyKK[y-1][group][i][j], label_width, label_height)){
						greedyKK[y-1][group][i][j]= Point_2(-1,-1);
						greedyKKSize[y-1][group][i]--;
					}
				}
			}
		}
	}
	//between y and y+2
	if(y < numberH-1){
		for(int i = 0; i < param_k +1; i++){
			//update greedyBetween[y+1][i][group]
			for(int j = 0; j <= x+1; j++){
				// if now
				if(greedyKK[y+1][i][group][j] != Point_2(-1,-1)){
					// TODO: write extendible function for a sorted solution
					if(!extendible(preGridS[y][x],greedyKK[y+1][i][group][j], label_width, label_height)){
						greedyKK[y+1][i][group][j]= Point_2(-1,-1);
						greedyKKSize[y+1][i][group]--;
					}
				}
			}
		}
	}
}
bool Grid_k_greedy::delete_point(size_t index) {
	Point_2 p = problem_ref->pVector[index];
	int x = ceil((p.x() - h_label_width) / label_width);
	int y = ceil((p.y() - h_label_height) / label_height);


	int x_index;
	//assert(gridC[y][x] > 0);
	gridP[y][x].erase(p);
	// update grid
	for (int i = 0; i < param_k; i++) {
		x_index = x - i;
		if (x_index >= 0) subPoint_recompute(x_index, y);
	}
	//update preGrid
	if (x < param_k - 1) {
		for (int i = x; i < param_k - 1; i++) {
			subPoint_recompute_pre(i, y);
		}
	}
	delete_greedy(x, y, p);

	solver_grid<std::set<Point_2>>::delete_point(index);
	return true;
};

// for gridS, gridC
// recompute group strating with grid_x and grid_y
void Grid_k_greedy::subPoint_recompute(int grid_x, int grid_y) {
	int oldCount = gridS[grid_y][grid_x].size();
	int& sum = (grid_y % 2 == 0) ? sumE : sumO;
	get_MIS_k(grid_x, grid_y);
	int index;
	int max_oldValue = -1;
	int max_value = -1;
	int index_candidate = -1;
	int max_index = -1;
	int newCount = gridS[grid_y][grid_x].size();
	if (newCount < oldCount) {
		//assert(newCount == oldCount-1);
		index = grid_x % (param_k + 1);
		counter[grid_y][index]--;
		if (mark[grid_y] == index) {
			max_oldValue = counter[grid_y][index] + 1;
			for (auto e : counter[grid_y]) {
				index_candidate++;
				if (e > max_value) {
					max_index = index_candidate;
					max_value = e;
				}
			}
			mark[grid_y] = max_index;

			sum -= max_oldValue;
			sum += max_value;
			if (sumE > sumO) {
				markE = true;
			}
			else markE = false;
		}
	}

	add_greedy_gridP(grid_x, grid_y);
	add_greedy_between(grid_x, grid_y);
	//TODO: because the group is recomputed, then all greedy parts(above and below) (2*k*k) needs to be checked
}

// for preGridS, preGridC
// recompute pre group strating with grid_x and grid_y(reversed to zero)
void Grid_k_greedy::subPoint_recompute_pre(int grid_x, int grid_y) {
	//assert(grid_x < k-1);
	int index_candidate = -1;
	int max_value = -1;
	int max_index = -1;
	int max_oldValue = -1;
	int& sum = ((grid_y % 2) == 0) ? sumE : sumO;
	int oldCount = preGridS[grid_y][grid_x].size();
	int index = (grid_x + 2) % (param_k + 1);
	get_MIS_pre(grid_x, grid_y);
	int newCount = preGridS[grid_y][grid_x].size();

	if (newCount < oldCount) {
		counter[grid_y][index]--;
		if (mark[grid_y] == index) {
			max_oldValue = counter[grid_y][index] + 1;
			for (auto e : counter[grid_y]) {
				index_candidate++;
				if (e > max_value) {
					max_index = index_candidate;
					max_value = e;
				}
			}
			mark[grid_y] = max_index;
			sum -= max_oldValue;
			sum += max_value;
			if (sumE > sumO) {
				markE = true;
			}
			else markE = false;
		}
	}
	//TODO: because the group is recomputed, then all greedy parts(above and below) (2*k*k) needs to be checked
	add_greedy_gridP_pre(grid_x, grid_y);
	add_greedy_between_pre(grid_x, grid_y);
}

void Grid_k_greedy::delete_greedy(int x, int y, Point_2& p){
	if(greedy_gridP[y][x]== p){
		greedy_gridP[y][x]= Point_2(-1,-1);
		greedy_counter[y][(x+1)%(param_k +1)]--;
	}
	//greedyKK
	for(int upper = 0; upper < param_k +1; upper++){
		for(int lower = 0; lower < param_k +1; lower++){
			if(greedyKK[y][upper][lower][x] == p){
				greedyKK[y][upper][lower][x]= Point_2(-1,-1);
				greedyKKSize[y][upper][lower]--;
			}
		}
	}
}

void Grid_k_greedy::set_solution() {
	Grid_k::set_solution();
	int c = (markE == true) ? 0 : 1;
	for (int i = 0; i < numberH; i++) {
		if (i % 2 != c) {
			int below = 0;
			int upper = 0;

			if (i > 0) {
				below = mark[i - 1];
			}
			if (i < numberH - 1) {
				upper = mark[i + 1];
			}
			for (int j = 0; j < numberV; j++) {
				if (greedyKK[i][upper][below][j] != Point_2(-1, -1)) {
					size_t index = problem_ref->look_up(greedyKK[i][upper][below][j]);
					mVector[index] = 1;
				}
			}
		}
		else {
			int s = (mark[i] + param_k) % (param_k + 1);
			for (int j = s; j < numberV; ) {
				if (greedy_gridP[i][j] != Point_2(-1, -1)) {
					size_t index = problem_ref->look_up(greedy_gridP[i][j]);
					mVector[index] = 1;
				}
				j += param_k + 1;
			}
		}
	}
}

//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
void Grid_k_greedy::print() {
	set_solution();
	Grid_k::print();

	for (int i = 0; i < numberH; i++) {
		vector<int> count((param_k + 1), 0);
		for (int j = 0; j < numberV; j++) {
			if (greedy_gridP[i][j] != Point_2(-1, -1)) {
				count[(j + 1) % (param_k + 1)]++;
				std::cout << "(" << greedy_gridP[i][j].x() << "," << greedy_gridP[i][j].y() << ") ";
			}
		}
		int j = 0;
		for (auto e : count) {
			assert(e == greedy_counter[i][j]);
			j++;
		}
		std::cout << endl;
	}

	int count;
	for (int i = 0; i < numberH; i++) {
		for (int upper = 0; upper < param_k + 1; upper++) {
			for (int below = 0; below < param_k + 1; below++) {
				std::cout << "i: " << i << " upper:" << upper <<" below:" << below << " size:" << greedyKKSize[i][upper][below] << std::endl;
				count = 0;
				for (auto e : greedyKK[i][upper][below]) {
					if (e != Point_2(-1, -1)) {
						std::cout << "(" << e.x() << "," << e.y() << ") ";
						count++;
					}
				}
				assert(count == greedyKKSize[i][upper][below]);
				std::cout << std::endl;
			}
		}
	}


};
void Grid_k_greedy::draw(cairo_t* cr) {
	set_solution();
	Grid_k::draw(cr);


};
#endif
//xPRINT---------------PRINT-------------------------

//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG



void Grid_k_greedy::getCombineSolution_GreedyBetween(vector<Point_2>& solution, int line_below, int group_below, int line_upper, int group_upper) {
	int line = line_below + 1;
	assert(line_below + 2 == line_upper);
	int count = 0;
	getCombineSolution(solution, line_below, group_below, line_upper, group_upper);
	assert(independency(solution));
	for (auto e : greedyKK[line][group_upper][group_below]) {
		if (e != Point_2(-1, -1)) {
			solution.push_back(e);
			count++;
		}
	}
	assert(independency(solution));
	assert(count == greedyKKSize[line][group_upper][group_below]);
}
void Grid_k_greedy::getCombineSolution(vector<Point_2>& solution, int line_below, int group_below, int line_upper, int group_upper) {
	assert(line_below + 2 == line_upper);
	if (line_below >= 0){
		get_group(solution, line_below, group_below);
		get_group_greedy(solution, line_below, group_below);
	}
	if(line_upper < numberH){
		get_group(solution, line_upper, group_upper);
		get_group_greedy(solution, line_upper, group_upper);
	}
}

void Grid_k_greedy::debugGreedyBetween(bool full) {
	vector<Point_2> solution;
	vector<Point_2> fullSolution;
	for (int i = 0; i < numberH; i++) {
		for (int upper = 0; upper < param_k + 1; upper++) {
			for (int below = 0; below < param_k + 1; below++) {
				solution.clear();
				getCombineSolution_GreedyBetween(solution, i - 1, below, i + 1, upper);
				// test independendy
				assert(independency(solution));
				if (full) {
					//check full
					fullSolution.clear();
					getCombineSolution_GreedyBetween(fullSolution, i - 1, below, i + 1, upper);
					// get solution of one lines (fullSolution)
					assert(fullSolution.size() == solution.size());
				}
			}
		}
	}

}



void Grid_k_greedy::debug() {
	set_solution();
	//Grid_k::debug();
	vector<Point_2> solution;
	vector<Point_2> fullSolution;
	for (int i = 0; i < numberH; i++) {
		for (int upper = 0; upper < param_k + 1; upper++) {
			for (int below = 0; below < param_k + 1; below++) {
				solution.clear();
				getCombineSolution_GreedyBetween(solution, i - 1, below, i + 1, upper);
				// test independendy
				assert(independency(solution));
			}
		}
	}


	// each line
	for (int i = 0; i < numberH; i++) {
		// each group
		for (int j = 0; j < (param_k + 1); j++) {
			// test independendy
			solution.clear();
			get_group(solution, i, j);
			get_group_greedy(solution, i, j);
			assert(independency(solution));

		}
	}
};
#endif
//!DEBUG---------------debug------------------------
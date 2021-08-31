/*
 * grid.cpp
 *
 *  Created on: Oct 17, 2019
 *      Author: guangping
 */
#include "grid_greedy.h"
void Grid_greedy::set(const shared_ptr<Problem>& P) {
	Grid::set(P);
	// greedy solution
	greedy_gridP = vector<vector<Point_2>>(numberH, vector<Point_2>(numberV, Point_2(-1, -1)));
	greedy_counter = vector<vector<unsigned int>>(numberH, vector<unsigned int>{0, 0});
	greedyEE = vector<vector<Point_2>>(numberH, vector<Point_2>(numberV, Point_2(-1, -1)));//00
	greedyEO = vector<vector<Point_2>>(numberH, vector<Point_2>(numberV, Point_2(-1, -1)));//01
	greedyOE = vector<vector<Point_2>>(numberH, vector<Point_2>(numberV, Point_2(-1, -1)));//10
	greedyOO = vector<vector<Point_2>>(numberH, vector<Point_2>(numberV, Point_2(-1, -1)));//11
	greedyEE_size = vector<unsigned int>(numberH, 0);
	greedyEO_size = vector<unsigned int>(numberH, 0);
	greedyOE_size = vector<unsigned int>(numberH, 0);
	greedyOO_size = vector<unsigned int>(numberH, 0);
};

void Grid_greedy::init(){
	Grid::init();
	
	greedy_Line();
	greedy_Between();
}

void Grid_greedy::recompute()
{
	Grid::recompute();


	size_t c = markE ? 0 : 1;
	greedy_gridP = vector<vector<Point_2>>(numberH, vector<Point_2>(numberV, Point_2(-1, -1)));
	greedy_counter = vector<vector<unsigned int>>(numberH, vector<unsigned int>{0, 0});
	greedyEE = vector<vector<Point_2>>(numberH, vector<Point_2>(numberV, Point_2(-1, -1)));//00
	greedyEO = vector<vector<Point_2>>(numberH, vector<Point_2>(numberV, Point_2(-1, -1)));//01
	greedyOE = vector<vector<Point_2>>(numberH, vector<Point_2>(numberV, Point_2(-1, -1)));//10
	greedyOO = vector<vector<Point_2>>(numberH, vector<Point_2>(numberV, Point_2(-1, -1)));//11
	greedyEE_size = vector<unsigned int>(numberH, 0);
	greedyEO_size = vector<unsigned int>(numberH, 0);
	greedyOE_size = vector<unsigned int>(numberH, 0);
	greedyOO_size = vector<unsigned int>(numberH, 0);
	for (size_t i = c; i < numberH; i += 2) {
		size_t cc = 0;
		Point_2 back = Point_2(-1, -1);
		Point_2 prev = Point_2(-1, -1);
		if (mark[i] == 0) {
			cc = 1;
		}
		for (size_t j = cc; j < numberV; j += 2) {
			if (j < numberV-1 && gridP[i][j + 1].size() > 0) {
				back = gridP[i][j + 1].back();
			}
			else back = Point_2(DBL_MAX, 0);
			if (j >= 1 && gridP[i][j - 1].size() > 0) {
				prev = gridP[i][j - 1].back();
			}
			for (auto e : gridP[i][j]) {
				// if it can be greedily extended, then add it in the greedy, geedyLineSize[i]++;
				if (e.x() > prev.x() + label_width && e.x() < back.x() - label_width) {
					greedy_gridP[i][j] = e;
					greedy_counter[i][j % 2]++;
					break;
				}
			}
		}
	}


	vector<vector<Point_2>>* greedyBetween = NULL;//11
	vector<unsigned int>* greedyBetween_size = NULL;
	
	size_t b = markE? 1 : 0;
	for (; b < numberH; b+=2) {
		size_t lower = 0;
		size_t upper = 0;
		bool nextE = false;
		bool prevE = false;
		Point_2 prev = Point_2(-1, -1);

		if (b > 0) { lower = b - 1; };
		if (b < numberH - 1) { upper = b + 1; };
		if (mark[upper] == 0) nextE = true;
		if (mark[lower] == 0) prevE = true;

		if (nextE && prevE) {
			greedyBetween = &greedyEE;
		}
		if (!nextE && prevE) {
			greedyBetween = &greedyOE;
		}
		if (nextE && !prevE) {
			greedyBetween = &greedyEO;
		}
		if (!nextE && !prevE) {
			greedyBetween = &greedyOO;
		}

		if (nextE && prevE) greedyBetween_size = &greedyEE_size;
		if (!nextE && prevE) greedyBetween_size = &greedyOE_size;
		if (nextE && !prevE) greedyBetween_size = &greedyEO_size;
		if (!nextE && !prevE) greedyBetween_size = &greedyOO_size;


		for (int j = 0; j < numberV; j++) {
			for (auto e : gridP[b][j]) {
				if (j > 0) {
					prev = (*greedyBetween)[b][j - 1];
					if ((e.x() - label_width) < prev.x()) continue;
				}
				bool checked = check_greedy(nextE, prevE, b, j, e);
				if (checked) {
					(*greedyBetween)[b][j] = e;
					(*greedyBetween_size)[b]++;
					break;
				}
			}
		}
	}
}

bool Grid_greedy::add_point_recompute(const Point_2& p) {
	solver_grid<std::list<Point_2>>::add_point(p);
	recompute();
	return true;
};
bool Grid_greedy::delete_point_recompute(size_t index) {
	solver_grid<std::list<Point_2>>::delete_point(index);
	recompute();
	return true;
};


void  Grid_greedy::greedy_Line() {
	Point_2 prev = Point_2(-1, -1);
	Point_2 back = Point_2(-1, -1);
	int i; int j;
	for (i = 0; i < numberH; i++) {
		for (j = 0; j < numberV - 1;) {
			if (gridP[i][j + 1].size() > 0) {
				back = gridP[i][j + 1].back();
			}
			else back = Point_2(DBL_MAX, 0);
			for (auto e : gridP[i][j]) {
				// if it can be greedily extended, then add it in the greedy, geedyLineSize[i]++;
				if (e.x() > prev.x() + label_width && e.x() < back.x() - label_width) {
					greedy_gridP[i][j] = e;
					greedy_counter[i][j % 2]++;
					break;
				}
			}
			j++;
			if (gridP[i][j - 1].size() > 0) {
				prev = gridP[i][j - 1].back();
			}
			else prev = Point_2(-DBL_MAX, 0);
		}
		// last one need to check the front one also
		for (auto e : gridP[i][j]) {
			if (e.x() > prev.x() + label_width) {
				greedy_gridP[i][j] = e;
				greedy_counter[i][j % 2]++;
				break;
			}
		}
	}



};
void  Grid_greedy::greedy_Between() {
	Point_2 prev = Point_2(-1, -1);
	Point_2 back = Point_2(-1, -1);
	for (int i = 0; i < numberH; i++) {
		//greedyEE = vector<vector<Point_2>>(numberH,vector<Point_2>(numberV,Point_2(-1,-1)));//00
		for (int j = 0; j < numberV; j++) {
			for (auto e : gridP[i][j]) {
				if (j > 0) {
					prev = greedyEE[i][j - 1];
					if ((e.x() - label_width) < prev.x()) continue;
				}
				if (check_greedy(true, true, i, j, e)) {
					greedyEE[i][j] = e;
					greedyEE_size[i]++;
					break;
				}
			}
		}
		//vector<vector<Point_2>> greedyEO = vector<vector<Point_2>>(numberH,vector<Point_2>(numberV,Point_2(-1,-1)));//01
		for (int j = 0; j < numberV; j++) {
			for (auto e : gridP[i][j]) {
				if (j > 0) {
					prev = greedyEO[i][j - 1];
					if ((e.x() - label_width) < prev.x()) continue;
				}
				if (check_greedy(true, false, i, j, e)) {
					greedyEO[i][j] = e;
					greedyEO_size[i]++;
					break;
				}
			}
		}
		//vector<vector<Point_2>> greedyOE = vector<vector<Point_2>>(numberH,vector<Point_2>(numberV,Point_2(-1,-1)));//10
		for (int j = 0; j < numberV; j++) {
			for (auto e : gridP[i][j]) {
				if (j > 0) {
					prev = greedyOE[i][j - 1];
					if ((e.x() - label_width) < prev.x()) continue;
				}
				if (check_greedy(false, true, i, j, e)) {
					greedyOE[i][j] = e;
					greedyOE_size[i]++;
					break;
				}
			}
		}
		//vector<vector<Point_2>> greedyOO = vector<vector<Point_2>>(numberH,vector<Point_2>(numberV,Point_2(-1,-1)));//11
		for (int j = 0; j < numberV; j++) {
			for (auto e : gridP[i][j]) {
				if (j > 0) {
					prev = greedyOO[i][j - 1];
					if ((e.x() - label_width) < prev.x()) continue;
				}
				if (check_greedy(false, false, i, j, e)) {
					greedyOO[i][j] = e;
					greedyOO_size[i]++;
					break;
				}
			}
		}
	}
};


// during adding the greedy points from left to right
// checked: the left neighbor is not in conflict with the current one
bool Grid_greedy::check_greedy(bool nextE, bool prevE, int i, int j, Point_2 current) {
	bool even = ((j % 2) == 0);
	// check at most four points
	Point_2 temp;
	if (i < numberH - 1) {
		if (even == nextE) {
			// one point above
			if (gridP[i + 1][j].size() > 0) {
				temp = gridP[i + 1][j].back();
				if (temp.y() < label_height + current.y()) {
					return false;
				}
			}
			// two greedy point above
			if (j > 0) {
				temp = greedy_gridP[i + 1][j - 1];
				if (temp != Point_2(-1, -1) && temp.y() < label_height + current.y() && temp.x() > current.x() - label_width) {
					return false;
				}
			}
			if (j < numberV - 1) {
				temp = greedy_gridP[i + 1][j + 1];
				if (temp != Point_2(-1, -1) && temp.y() < label_height + current.y() && temp.x() < current.x() + label_width) {
					return false;
				}
			}
		}
		else {
			//check if j is 0 or numV-1
			// two point above
			if (j > 0) {
				if (gridP[i + 1][j - 1].size() > 0) {
					temp = gridP[i + 1][j - 1].back();
					if (temp.y() < label_height + current.y() && temp.x() > current.x() - label_width) {
						return false;
					}
				}
			}
			if (j < numberV - 1) {
				if (gridP[i + 1][j + 1].size() > 0) {
					temp = gridP[i + 1][j + 1].back();
					if (temp.y() < label_height + current.y() && temp.x() < current.x() + label_width) {
						return false;
					}
				}
			}
			// one greedy point above
			temp = greedy_gridP[i + 1][j];
			if (temp != Point_2(-1, -1) && temp.y() < label_height + current.y()) {
				return false;
			}
		}
	}



	if (i > 0) {
		if (even == prevE) {
			// one point below
			if (gridP[i - 1][j].size() > 0) {
				temp = gridP[i - 1][j].back();
				if (temp.y() > -label_height + current.y()) {
					return false;
				}
			}
			// two greedy points below
			if (j > 0) {
				temp = greedy_gridP[i - 1][j - 1];
				if (temp != Point_2(-1, -1) && temp.y() > -label_height + current.y() && temp.x() > current.x() - label_width) {
					return false;
				}
			}
			if (j < numberV - 1) {
				temp = greedy_gridP[i - 1][j + 1];
				if (temp != Point_2(-1, -1) && temp.y() > -label_height + current.y() && temp.x() < current.x() + label_width) {
					return false;
				}
			}
		}
		else {
			//check if j is 0 or numV-1
			// two point below
			if (j > 0) {
				if (gridP[i - 1][j - 1].size() > 0) {
					temp = gridP[i - 1][j - 1].back();
					if (temp.y() > -label_height + current.y() && temp.x() > current.x() - label_width) {
						return false;
					}
				}
			}
			if (j < numberV - 1) {
				if (gridP[i - 1][j + 1].size() > 0) {
					temp = gridP[i - 1][j + 1].back();
					if (temp.y() > -label_height + current.y() && temp.x() < current.x() + label_width) {
						return false;
					}
				}
			}
			//one greedy point below
			temp = greedy_gridP[i - 1][j];
			if (temp != Point_2(-1, -1) && temp.y() > -label_width + current.y()) {
				return false;
			}
		}
	}
	return true;
};




bool Grid_greedy::add_point(const Point_2& p){
	Grid::add_point(p);
	//std:: cout<< "in addPoint"<< std::endl;
	int x = ceil((p.x() - h_label_width) / label_width);
	int y = ceil((p.y() - h_label_height) / label_height);
	//std:: cout<< "original points:"<< p.x() << ","<< p.y()<< std::endl;
	//std:: cout<< "lines:"<< x << ","<< y<< std::endl;
	int& sum = ((y % 2) == 0) ? sumE : sumO;
	if (gridP[y][x].size() == 1) {
		add_greedy(x, y, p);
		return true;
	}
	return true;
};

// 0->1 at the ceil
void Grid_greedy::add_greedy(int x, int y, const Point_2& p) {
	//std::cout<<"in addGreedy"<< std::endl;
	//print_Greedy();
	Point_2 prev = Point_2(-1, -1);
	Point_2 back = Point_2(-1, -1);
	Point_2 greedyBack = Point_2(-1, -1);
	Point_2 greedyPrev = Point_2(-1, -1);

	//std::cout<< "for back"<< std::endl;
	if (x < numberV - 1) {
		if (gridP[y][x + 1].size() > 0) {
			back = gridP[y][x + 1].back();
			greedyBack = greedy_gridP[y][x + 1];
			if (greedy_gridP[y][x + 1] != Point_2(-1, -1) && p.x() > greedyBack.x() - label_width) {
				//std::cout<<greedy_gridP[y][x+1].x()<<" "<<  greedy_gridP[y][x+1].y()<< std::endl;
				greedy_gridP[y][x + 1] = Point_2(-1, -1);
				greedy_counter[y][(x + 1) % 2]--;
			}
		}
	}
	//std::cout<< "　for prev"<< std::endl;
	if (x > 0) {
		if (gridP[y][x - 1].size() > 0) {
			prev = gridP[y][x - 1].back();
			greedyPrev = greedy_gridP[y][x - 1];
			if (greedyPrev != Point_2(-1, -1) && p.x() < greedyPrev.x() + label_width) {
				//std::cout<<greedy_gridP[y][x-1].x()<<" "<<  greedy_gridP[y][x-1].y()<< std::endl;
				greedy_gridP[y][x - 1] = Point_2(-1, -1);
				greedy_counter[y][(x - 1) % 2]--;
			}
		}
	}



	//std::cout<< "  affect 4 lines according to the x"<< std::endl;
		//	std::cout<< "next"<< std::endl;
	if (x % 2 == 0) {
		//std::cout<< "see upper part"<< std::endl;
		if (y < numberH - 1) {
			//std::cout<< "above"<< std::endl;
			if (greedyEE[y + 1][x] != Point_2(-1, -1) && greedyEE[y + 1][x].y() < label_height + p.y()) {
				//std::cout<<greedyEE[y+1][x].x()<<" "<< greedyEE[y+1][x].y()<< std::endl;
				greedyEE[y + 1][x] = Point_2(-1, -1);
				greedyEE_size[y + 1]--;
			}
			if (greedyOE[y + 1][x] != Point_2(-1, -1) && greedyOE[y + 1][x].y() < label_height + p.y()) {
				//std::cout<<greedyOE[y+1][x] .x()<<" "<<  greedyOE[y+1][x].y()<< std::endl;
				greedyOE[y + 1][x] = Point_2(-1, -1);
				greedyOE_size[y + 1]--;
			}
			//std::cout<< "above prev"<< std::endl;
			if (x > 0) {
				if (greedyEE[y + 1][x - 1] != Point_2(-1, -1) && greedyEE[y + 1][x - 1].y() < p.y() + label_height && greedyEE[y + 1][x - 1].x() > p.x() - label_width) {
					//std::cout<<greedyEE[y+1][x-1] .x()<<" "<<  greedyEE[y+1][x-1].y()<< std::endl;
					greedyEE[y + 1][x - 1] = Point_2(-1, -1);
					greedyEE_size[y + 1]--;
				}
				if (greedyOE[y + 1][x - 1] != Point_2(-1, -1) && greedyOE[y + 1][x - 1].y() < p.y() + label_height && greedyOE[y + 1][x - 1].x() > p.x() - label_width) {
					//std::cout<<greedyOE[y+1][x-1].x()<<" "<<  greedyOE[y+1][x-1].y()<< std::endl;
					greedyOE[y + 1][x - 1] = Point_2(-1, -1);
					greedyOE_size[y + 1]--;
				}
			}
			//std::cout<< " above back"<< std::endl;
			if (x < numberV - 1) {
				if (greedyEE[y + 1][x + 1] != Point_2(-1, -1) && greedyEE[y + 1][x + 1].y() < p.y() + label_height && greedyEE[y + 1][x + 1].x() < p.x() + label_width) {
					//std::cout<<greedyEE[y+1][x+1].x()<<" "<<  greedyEE[y+1][x+1].y()<< std::endl;
					greedyEE[y + 1][x + 1] = Point_2(-1, -1);
					greedyEE_size[y + 1]--;
				}
				if (greedyOE[y + 1][x + 1] != Point_2(-1, -1) && greedyOE[y + 1][x + 1].y() < p.y() + label_height && greedyOE[y + 1][x + 1].x() < p.x() + label_width) {
					//std::cout<<greedyOE[y+1][x+1].x()<<" "<<  greedyOE[y+1][x+1].y()<< std::endl;
					greedyOE[y + 1][x + 1] = Point_2(-1, -1);
					greedyOE_size[y + 1]--;
				}
			}
		}
		//std::cout<< "see below part"<< std::endl;
		if (y > 0) {
			//std::cout<< "below"<< std::endl;
			if (greedyEE[y - 1][x] != Point_2(-1, -1) && greedyEE[y - 1][x].y() > -label_height + p.y()) {
				//std::cout<<greedyEE[y-1][x] .x()<<" "<<  greedyEE[y-1][x] .y()<< std::endl;
				greedyEE[y - 1][x] = Point_2(-1, -1);
				greedyEE_size[y - 1]--;
			}
			if (greedyEO[y - 1][x] != Point_2(-1, -1) && greedyEO[y - 1][x].y() > -label_height + p.y()) {
				//std::cout<<greedyEO[y-1][x] .x()<<" "<<  greedyEO[y-1][x] .y()<< std::endl;
				greedyEO[y - 1][x] = Point_2(-1, -1);
				greedyEO_size[y - 1]--;
			}
			//std::cout<< " below prev"<< std::endl;
			if (x > 0) {
				if (greedyEE[y - 1][x - 1] != Point_2(-1, -1) && greedyEE[y - 1][x - 1].y() > p.y() - label_height && greedyEE[y - 1][x - 1].x() > p.x() - label_width) {
					//std::cout<<greedyEE[y-1][x-1].x()<<" "<<  greedyEE[y-1][x-1].y()<< std::endl;
					greedyEE[y - 1][x - 1] = Point_2(-1, -1);
					greedyEE_size[y - 1]--;
				}
				if (greedyEO[y - 1][x - 1] != Point_2(-1, -1) && greedyEO[y - 1][x - 1].y() > p.y() - label_height && greedyEO[y - 1][x - 1].x() > p.x() - label_width) {
					//std::cout<<greedyEO[y-1][x-1].x()<<" "<< greedyEO[y-1][x-1].y()<< std::endl;
					greedyEO[y - 1][x - 1] = Point_2(-1, -1);
					greedyEO_size[y - 1]--;
				}
			}
			//std::cout<< " below back"<< std::endl;
			if (x < numberV - 1) {
				if (greedyEE[y - 1][x + 1] != Point_2(-1, -1) && greedyEE[y - 1][x + 1].y() > p.y() - label_height && greedyEE[y - 1][x + 1].x() < p.x() + label_width) {
					//std::cout<<greedyEE[y-1][x+1].x()<<" "<<  greedyEE[y-1][x+1].y()<< std::endl;
					greedyEE[y - 1][x + 1] = Point_2(-1, -1);
					greedyEE_size[y - 1]--;
				}
				if (greedyEO[y - 1][x + 1] != Point_2(-1, -1) && greedyEO[y - 1][x + 1].y() > p.y() - label_height && greedyEO[y - 1][x + 1].x() < p.x() + label_width) {
					//std::cout<<greedyEO[y-1][x+1].x()<<" "<<  greedyEO[y-1][x+1].y()<< std::endl;
					greedyEO[y - 1][x + 1] = Point_2(-1, -1);
					greedyEO_size[y - 1]--;
				}
			}
		}
	}
	else {
		//std::cout<< "see upper part"<< std::endl;
		if (y < numberH - 1) {
			//std::cout<< "above"<< std::endl;
			if (greedyEO[y + 1][x] != Point_2(-1, -1) && greedyEO[y + 1][x].y() < label_height + p.y()) {
				//std::cout<<greedyEO[y+1][x].x()<<" "<<  greedyEO[y+1][x].y()<< std::endl;
				greedyEO[y + 1][x] = Point_2(-1, -1);
				greedyEO_size[y + 1]--;
			}
			if (greedyOO[y + 1][x] != Point_2(-1, -1) && greedyOO[y + 1][x].y() < label_height + p.y()) {
				//std::cout<<greedyOO[y+1][x].x()<<" "<<  greedyOO[y+1][x].y()<< std::endl;
				greedyOO[y + 1][x] = Point_2(-1, -1);
				greedyOO_size[y + 1]--;
			}
			//std::cout<< " above prev"<< std::endl;
			if (x > 0) {
				if (greedyEO[y + 1][x - 1] != Point_2(-1, -1) && greedyEO[y + 1][x - 1].y() < p.y() + label_height && greedyEO[y + 1][x - 1].x() > p.x() - label_width) {
					//std::cout<<greedyEO[y+1][x-1].x()<<" "<<  greedyEO[y+1][x-1].y()<< std::endl;
					greedyEO[y + 1][x - 1] = Point_2(-1, -1);
					greedyEO_size[y + 1]--;
				}
				if (greedyOO[y + 1][x - 1] != Point_2(-1, -1) && greedyOO[y + 1][x - 1].y() < p.y() + label_height && greedyOO[y + 1][x - 1].x() > p.x() - label_width) {
					//std::cout<<greedyOO[y+1][x-1].x()<<" "<< greedyOO[y+1][x-1].y()<< std::endl;
					greedyOO[y + 1][x - 1] = Point_2(-1, -1);
					greedyOO_size[y + 1]--;
				}
			}
			//std::cout<< " above back"<< std::endl;
			if (x < numberV - 1) {
				if (greedyEO[y + 1][x + 1] != Point_2(-1, -1) && greedyEO[y + 1][x + 1].y() < p.y() + label_height && greedyEO[y + 1][x + 1].x() < p.x() + label_width) {
					//std::cout<<greedyEO[y+1][x+1].x()<<" "<<  greedyEO[y+1][x+1].y()<< std::endl;
					greedyEO[y + 1][x + 1] = Point_2(-1, -1);
					greedyEO_size[y + 1]--;
				}
				if (greedyOO[y + 1][x + 1] != Point_2(-1, -1) && greedyOO[y + 1][x + 1].y() < p.y() + label_height && greedyOO[y + 1][x + 1].x() < p.x() + label_width) {
					//std::cout<<greedyOO[y+1][x+1].x()<<" "<<  greedyOO[y+1][x+1].y()<< std::endl;
					greedyOO[y + 1][x + 1] = Point_2(-1, -1);
					greedyOO_size[y + 1]--;
				}
			}
		}
		//std::cout<< "see below part"<< std::endl;
		if (y > 0) {
			//std::cout<< "below"<< std::endl;
			if (greedyOE[y - 1][x] != Point_2(-1, -1) && greedyOE[y - 1][x].y() > -label_height + p.y()) {
				//std::cout<<	greedyOE[y-1][x].x()<<" "<<  	greedyOE[y-1][x].y()<< std::endl;
				greedyOE[y - 1][x] = Point_2(-1, -1);
				greedyOE_size[y - 1]--;
			}
			if (greedyOO[y - 1][x] != Point_2(-1, -1) && greedyOO[y - 1][x].y() > -label_height + p.y()) {
				//std::cout<<greedyOO[y-1][x].x()<<" "<< greedyOO[y-1][x].y()<< std::endl;
				greedyOO[y - 1][x] = Point_2(-1, -1);
				greedyOO_size[y - 1]--;
			}
			//std::cout<< " below prev"<< std::endl;
			if (x > 0) {
				if (greedyOE[y - 1][x - 1] != Point_2(-1, -1) && greedyOE[y - 1][x - 1].y() > p.y() - label_height && greedyOE[y - 1][x - 1].x() > p.x() - label_width) {
					//std::cout<<greedyOE[y-1][x-1].x()<<" "<<  greedyOE[y-1][x-1].y()<< std::endl;
					greedyOE[y - 1][x - 1] = Point_2(-1, -1);
					greedyOE_size[y - 1]--;
				}
				if (greedyOO[y - 1][x - 1] != Point_2(-1, -1) && greedyOO[y - 1][x - 1].y() > p.y() - label_height && greedyOO[y - 1][x - 1].x() > p.x() - label_width) {
					//std::cout<<greedyOO[y-1][x-1].x()<<" "<<  greedyOO[y-1][x-1].y()<< std::endl;
					greedyOO[y - 1][x - 1] = Point_2(-1, -1);
					greedyOO_size[y - 1]--;
				}
			}
			//std::cout<< " below back"<< std::endl;
			if (x < numberV - 1) {
				if (greedyOE[y - 1][x + 1] != Point_2(-1, -1) && greedyOE[y - 1][x + 1].y() > p.y() - label_height && greedyOE[y - 1][x + 1].x() < p.x() + label_width) {
					//std::cout<<greedyOE[y-1][x+1].x()<<" "<< greedyOE[y-1][x+1].y()<< std::endl;
					greedyOE[y - 1][x + 1] = Point_2(-1, -1);
					greedyOE_size[y - 1]--;
				}
				if (greedyOO[y - 1][x + 1] != Point_2(-1, -1) && greedyOO[y - 1][x + 1].y() > p.y() - label_height && greedyOO[y - 1][x + 1].x() < p.x() + label_width) {
					//std::cout<<greedyOO[y-1][x+1].x()<<" "<<  greedyOO[y-1][x+1].y()<< std::endl;
					greedyOO[y - 1][x + 1] = Point_2(-1, -1);
					greedyOO_size[y - 1]--;
				}
			}
		}
	}
}
void Grid_greedy::delete_greedy(int x, int y, Point_2& p, bool inSolution) {
	//std::cout<< "in subGreedy"<< std::endl;
	if (greedy_gridP[y][x] == p) {
		greedy_gridP[y][x] = Point_2(-1, -1);
		greedy_counter[y][x % 2]--;
	}
	//EE
	if (greedyEE[y][x] == p) {
		greedyEE[y][x] = Point_2(-1, -1);
		greedyEE_size[y]--;
	}
	//EO
	if (greedyEO[y][x] == p) {
		greedyEO[y][x] = Point_2(-1, -1);
		greedyEO_size[y]--;
	}
	//OE
	if (greedyOE[y][x] == p) {
		greedyOE[y][x] = Point_2(-1, -1);
		greedyOE_size[y]--;
	}
	//OO
	if (greedyOO[y][x] == p) {
		greedyOO[y][x] = Point_2(-1, -1);
		greedyOO_size[y]--;
	}
	if (inSolution) {
		if (x > 0 && greedy_gridP[y][x - 1] != Point_2(-1, -1)) {
			greedy_gridP[y][x - 1] = Point_2(-1, -1);
			greedy_counter[y][(x - 1) % 2]--;
		}
		if (x < numberV - 1 && greedy_gridP[y][x + 1] != Point_2(-1, -1)) {
			greedy_gridP[y][x + 1] = Point_2(-1, -1);
			greedy_counter[y][(x + 1) % 2]--;
		}
		bool even = ((x % 2) == 0);
		vector<vector<Point_2>>* greedyAboveE = NULL;
		vector<vector<Point_2>>* greedyAboveO = NULL;
		vector<vector<Point_2>>* greedyBelowE = NULL;
		vector<vector<Point_2>>* greedyBelowO = NULL;
		vector<unsigned int>* greedyAboveESize = NULL;
		vector<unsigned int>* greedyAboveOSize = NULL;
		vector<unsigned int>* greedyBelowESize = NULL;
		vector<unsigned int>* greedyBelowOSize = NULL;

		if (even) {
			greedyAboveE = &greedyEE;
			greedyAboveO = &greedyOE;
			greedyBelowE = &greedyEE;
			greedyBelowO = &greedyEO;
			greedyAboveESize = &greedyEE_size;
			greedyAboveOSize = &greedyOE_size;
			greedyBelowESize = &greedyEE_size;
			greedyBelowOSize = &greedyEO_size;
		}
		else {
			greedyAboveE = &greedyEO;
			greedyAboveO = &greedyOO;
			greedyBelowE = &greedyOE;
			greedyBelowO = &greedyOO;
			greedyAboveESize = &greedyEO_size;
			greedyAboveOSize = &greedyOO_size;
			greedyBelowESize = &greedyOE_size;
			greedyBelowOSize = &greedyOO_size;
		}
		if (y < numberH - 1) {
			// (y+1) EE OE
			//x
			if ((*greedyAboveE)[y + 1][x] != Point_2(-1, -1)) {
				(*greedyAboveE)[y + 1][x] = Point_2(-1, -1);
				(*greedyAboveESize)[y + 1]--;
			}
			if ((*greedyAboveO)[y + 1][x] != Point_2(-1, -1)) {
				(*greedyAboveO)[y + 1][x] = Point_2(-1, -1);
				(*greedyAboveOSize)[y + 1]--;
			}
			//x-1
			if (x > 0) {
				if ((*greedyAboveE)[y + 1][x - 1] != Point_2(-1, -1)) {
					(*greedyAboveE)[y + 1][x - 1] = Point_2(-1, -1);
					(*greedyAboveESize)[y + 1]--;
				}
				if ((*greedyAboveO)[y + 1][x - 1] != Point_2(-1, -1)) {
					(*greedyAboveO)[y + 1][x - 1] = Point_2(-1, -1);
					(*greedyAboveOSize)[y + 1]--;
				}
			}
			//x+1
			if (x < numberV - 1) {
				if ((*greedyAboveE)[y + 1][x + 1] != Point_2(-1, -1)) {
					(*greedyAboveE)[y + 1][x + 1] = Point_2(-1, -1);
					(*greedyAboveESize)[y + 1]--;
				}
				if ((*greedyAboveO)[y + 1][x + 1] != Point_2(-1, -1)) {
					(*greedyAboveO)[y + 1][x + 1] = Point_2(-1, -1);
					(*greedyAboveOSize)[y + 1]--;
				}
			}
		}
		if (y > 0) {
			//(y-1）(EE,EO)
			//x
			if ((*greedyBelowE)[y - 1][x] != Point_2(-1, -1)) {
				(*greedyBelowE)[y - 1][x] = Point_2(-1, -1);
				(*greedyBelowESize)[y - 1]--;
			}
			if ((*greedyBelowO)[y - 1][x] != Point_2(-1, -1)) {
				(*greedyBelowO)[y - 1][x] = Point_2(-1, -1);
				(*greedyBelowOSize)[y - 1]--;
			}
			//x-1
			if (x > 0) {
				if ((*greedyBelowE)[y - 1][x - 1] != Point_2(-1, -1)) {
					(*greedyBelowE)[y - 1][x - 1] = Point_2(-1, -1);
					(*greedyBelowESize)[y - 1]--;
				}
				if ((*greedyBelowO)[y - 1][x - 1] != Point_2(-1, -1)) {
					(*greedyBelowO)[y - 1][x - 1] = Point_2(-1, -1);
					(*greedyBelowOSize)[y - 1]--;
				}
			}
			//x+1
			if (x < numberV - 1) {
				if ((*greedyBelowE)[y - 1][x + 1] != Point_2(-1, -1)) {
					(*greedyBelowE)[y - 1][x + 1] = Point_2(-1, -1);
					(*greedyBelowESize)[y - 1]--;
				}
				if ((*greedyBelowO)[y - 1][x + 1] != Point_2(-1, -1)) {
					(*greedyBelowO)[y - 1][x + 1] = Point_2(-1, -1);
					(*greedyBelowOSize)[y - 1]--;
				}
			}
		}
	}
	//std::cout<< "out subGreedy"<< std::endl;
}


bool Grid_greedy::delete_point(size_t index) {
	//std:: cout<< "in subPoint"<< std::endl;
	Point_2 p = problem_ref->pVector[index];
	int x = ceil((p.x() - h_label_width) / label_width);
	int y = ceil((p.y() - h_label_height) / label_height);
	bool inSolution = (gridP[y][x].back() == p);
	Grid::delete_point(index);
	delete_greedy(x, y, p, inSolution);
	return true;
	//std:: cout<< "out subPoint"<< std::endl;
};
void Grid_greedy::set_solution() {
	
	vector<vector<Point_2>> const* greedyBetween = NULL;//11
	
	
	Grid::set_solution();

	int c = markE? 0 : 1;
	for (int i = 0; i < numberH; i++) {
		if (i % 2 == c){
			for (int j = 0; j < numberV; j++) {
				if (j % 2 != mark[i]) {
					if (greedy_gridP[i][j] != Point_2(-1, -1)) {
						size_t index = problem_ref->look_up(greedy_gridP[i][j]);
						mVector[index] = 1;
					}

				}
			}
		}
		// greedy solution in the middle lines
		else {
			// the greedy part in between
			if (i == 0 || mark[i - 1] == 0) {
				if (i == numberH - 1 || mark[i + 1] == 0) {
					greedyBetween = &greedyEE;
				}
				else {
					greedyBetween = &greedyOE;
				}
			}
			else {
				if (i == numberH - 1 || mark[i + 1] == 0) {
					greedyBetween = &greedyEO;
				}
				else {
					greedyBetween = &greedyOO;
				}
			}
			for (auto e : (*greedyBetween)[i]) {
				if (e != Point_2(-1, -1)) {
					size_t index = problem_ref->look_up(e);
					mVector[index] = 1;
				
				};
			}
		}
	}
};
//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
void Grid_greedy::print() {
	set_solution();
	Grid::print();


};
void Grid_greedy::draw(cairo_t* cr) {
	set_solution();
	Grid::draw(cr);
};
#endif
//xPRINT---------------PRINT-------------------------

//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
void Grid_greedy::debug() {
	set_solution();
	Grid::debug();

	vector<Point_2> solution;
	solution.clear();
	int j;
	vector<vector<Point_2>> const* greedyBetween = NULL;//11
	vector<unsigned int>const* greedyBetweenSize = NULL;
	unsigned int upperGreedy = 0;
	unsigned int upperCount = 0;
	unsigned int lowerGreedy = 0;
	unsigned int lowerCount = 0;
	int greedyBetweenCount = 0;
	//std::cout<< " first line in the pair"<< std::endl;
	for (int i = 0; i < numberH - 2; i++) {
		j = i + 2;
		for (int first = 0; first < 2; first++) {
			for (int second = 0; second < 2; second++) {
				//std::cout<< "get solution"<< std::endl;
					//std::cout<< "upper line"<< std::endl;
				for (int index = 0; index < numberV; index++) {
					if (index % 2 == first) {
						if (gridP[i][index].size() > 0) {
							solution.push_back(gridP[i][index].back());
							upperCount++;
						}
					}
					else {
						if (greedy_gridP[i][index] != Point_2(-1, -1)) {
							solution.push_back(greedy_gridP[i][index]);
							upperGreedy++;
						}
					}
				}
				//std::cout<< " lower line"<< std::endl;
				for (int index = 0; index < numberV; index++) {
					if (index % 2 == second) {
						if (gridP[j][index].size() > 0) {
							solution.push_back(gridP[j][index].back());
							lowerCount++;
						}
					}
					else {
						if (greedy_gridP[j][index] != Point_2(-1, -1)) {
							solution.push_back(greedy_gridP[j][index]);
							lowerGreedy++;
						}
					}
				}
				assert(independency(solution));
				//std::cout<< "the greedy part in between"<< std::endl;
				if (first % 2 == 0) {
					if (second % 2 == 0) {
						greedyBetween = &greedyEE;
						greedyBetweenSize = &greedyEE_size;
					}
					else {
						greedyBetween = &greedyOE;
						greedyBetweenSize = &greedyOE_size;
					}
				}
				else {
					if (second % 2 == 0) {
						greedyBetween = &greedyEO;
						greedyBetweenSize = &greedyEO_size;
					}
					else {
						greedyBetween = &greedyOO;
						greedyBetweenSize = &greedyOO_size;
					}
				}
				for (auto q : (*greedyBetween)[i + 1]) {
					if (q != Point_2(-1, -1)) {
						solution.push_back(q);
						greedyBetweenCount++;
					}
				}
				assert(upperGreedy == greedy_counter[i][(first + 1) % 2]);
				if ((first % 2) == 0) {
					assert(upperCount == counter[i][0]);
				}
				else assert(upperCount == counter[i][1]);
				if ((second % 2) == 0) {
					assert(lowerCount == counter[j][0]);
				}
				else 		assert(lowerCount == counter[j][1]);
				assert(greedyBetweenCount == (*greedyBetweenSize)[i + 1]);
				assert(lowerGreedy == greedy_counter[j][(second + 1) % 2]);
				assert(solution.size() == (upperGreedy + upperCount + lowerCount + greedyBetweenCount + lowerGreedy));
				assert(independency(solution));
				//std::cout<< "clear solution"<< std::endl;
				upperGreedy = 0;
				lowerGreedy = 0;
				upperCount = 0;
				lowerCount = 0;
				greedyBetweenCount = 0;
				solution.clear();
			}
		}
	}

};
#endif
//!DEBUG---------------debug------------------------
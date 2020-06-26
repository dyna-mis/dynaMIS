/*
 * grid.cpp
 *
 *  Created on: Oct 17, 2019
 *      Author: guangping
 */
#include "problem_sl_greedy.h"
Problem_sl_greedy::Problem_sl_greedy(double w, const char* fileName):Problem(w,fileName){
	numberV = ceil(width/sigma);
	numberH=ceil(height/sigma);
	gridP=vector<vector<list<Point_2>>>(numberH,vector<list<Point_2>>(numberV, list<Point_2>{}));
	gridC=vector<vector<int>>(numberH,vector<int>(numberV, 0));
	counter=vector<intPair>(numberH,intPair(0,0));
	mark=vector<bool>(numberH,false);
	sumE= 0;
	sumO = 0;
	markE = false;

	// greedy solution
	greedyLine = vector<vector<Point_2>>(numberH,vector<Point_2>(numberV,Point_2(-1,-1)));
	greedyLineSize =vector<vector<unsigned int>>(numberH, vector<unsigned int>{0,0});
	greedyEE = vector<vector<Point_2>>(numberH,vector<Point_2>(numberV,Point_2(-1,-1)));//00
	greedyEO = vector<vector<Point_2>>(numberH,vector<Point_2>(numberV,Point_2(-1,-1)));//01
	greedyOE = vector<vector<Point_2>>(numberH,vector<Point_2>(numberV,Point_2(-1,-1)));//10
	greedyOO = vector<vector<Point_2>>(numberH,vector<Point_2>(numberV,Point_2(-1,-1)));//11
	greedyEESize = vector<unsigned int> (numberH,0);
	greedyEOSize = vector<unsigned int>(numberH, 0);
	greedyOESize = vector<unsigned int>(numberH, 0);
	greedyOOSize = vector<unsigned int>(numberH, 0);
};
void Problem_sl_greedy::setPoint(){
	int x, y;
	for(auto p: pVector){
		x=ceil((p.x()-h_sigma)/sigma);
		y=ceil((p.y()-h_sigma)/sigma);
		if(!(x < numberV)){
			std::cout<< "p_x: "<<p.x()<< std::endl;
			std::cout<< "before ceil: "<<(p.x()-h_sigma)/sigma<< std::endl;
			std::cout<< "x: "<<x<< std::endl;
			std::cout<< "numberV: "<<numberV<< std::endl;
		}
		//assert(x < numberV);
		if(!(y < numberH)){
			std::cout<< "p_y: "<<p.y()<< std::endl;
			std::cout<< "y: "<<y<< std::endl;
			std::cout<< "numberH: "<<numberH<< std::endl;
		}
		//assert(y < numberH);
		gridC[y][x]++;
		gridP[y][x].push_back(p);
		if(gridC[y][x] == 1){
			if(x%2 ==0) counter[y].first++;
			else counter[y].second++;
		}
	}
};
void Problem_sl_greedy::simple(){
	auto t0 = std::chrono::high_resolution_clock::now();
	setPoint();
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds d = std::chrono::duration_cast< std::chrono::nanoseconds >(fs);
	t0 = std::chrono::high_resolution_clock::now();
	int sum = 0;
	for(int i = 0; i < numberH; i++){
	   // std::cout<< "&&&&&&&&&&&&& "<<counter[i].first<< " "<<counter[i].second<< std::endl;
		sum = 0;
		if(counter[i].first > counter[i].second){
			sum = counter[i].first;
		 //   std::cout<< "*******************"<< std::endl;
			mark[i]= true;
		}
		else{
			//assert(!(counter[i].first > counter[i].second));
			sum = counter[i].second;
		}
		if(i%2==0) sumE += sum;
		else sumO += sum;
	}
	if(sumE > sumO){
		markE = true;
	}
	else markE = false;
	greedy();
}
void Problem_sl_greedy::greedy(){
	Point_2 prev = Point_2(-1,-1) ;
	Point_2 back = Point_2(-1,-1);
	int i; int j;
	for(i = 0; i < numberH; i++){
		for(j = 0; j < numberV-1;){
			if(gridC[i][j+1] > 0){
				back = gridP[i][j+1].back();
			}
			else back = Point_2(DBL_MAX,0);
			for(auto e: gridP[i][j]){
				// if it can be greedily extended, then add it in the greedy, geedyLineSize[i]++;
				if(e.x() > prev.x()+sigma && e.x() < back.x()-sigma){
					greedyLine[i][j] = e;
					greedyLineSize[i][j%2]++;
					break;
				}
			}
			j++;
			if(gridC[i][j-1] > 0){
				prev = gridP[i][j-1].back();
			}
			else prev = Point_2(-DBL_MAX,0);
		}
		// last one need to check the front one also
		for(auto e: gridP[i][j]){
			if(e.x() > prev.x()+sigma){
				greedyLine[i][j] = e;
				greedyLineSize[i][j%2]++;
				break;
			}
		}
	}
	for(int i = 0; i < numberH; i++){
		//greedyEE = vector<vector<Point_2>>(numberH,vector<Point_2>(numberV,Point_2(-1,-1)));//00
		for(int j = 0; j < numberV;j++){
			for(auto e: gridP[i][j]){
				if(j > 0){
					prev =greedyEE[i][j-1];
					if((e.x()-sigma) <prev.x()) continue;
				}
				if(checkGreedy(true, true, i,j,e)){
					greedyEE[i][j]=e;
					greedyEESize[i]++;
					break;
				}
			}
		}
		//vector<vector<Point_2>> greedyEO = vector<vector<Point_2>>(numberH,vector<Point_2>(numberV,Point_2(-1,-1)));//01
		for(int j = 0; j < numberV;j++){
			for(auto e: gridP[i][j]){
				if(j > 0){
					prev =greedyEO[i][j-1];
					if((e.x()-sigma) <prev.x()) continue;
				}
				if(checkGreedy(true, false, i,j,e)){
					greedyEO[i][j]=e;
					greedyEOSize[i]++;
					break;
				}
			}
		}
		//vector<vector<Point_2>> greedyOE = vector<vector<Point_2>>(numberH,vector<Point_2>(numberV,Point_2(-1,-1)));//10
		for(int j = 0; j < numberV;j++){
			for(auto e: gridP[i][j]){
				if(j > 0){
					prev =greedyOE[i][j-1];
					if((e.x()-sigma) <prev.x()) continue;
				}
				if(checkGreedy(false, true, i,j,e)){
					greedyOE[i][j]=e;
					greedyOESize[i]++;
					break;
				}
			}
		}
		//vector<vector<Point_2>> greedyOO = vector<vector<Point_2>>(numberH,vector<Point_2>(numberV,Point_2(-1,-1)));//11
		for(int j = 0; j < numberV;j++){
			for(auto e: gridP[i][j]){
				if(j > 0){
					prev =greedyOO[i][j-1];
					if((e.x()-sigma) <prev.x()) continue;
				}
				if(checkGreedy(false, false, i,j,e)){
					greedyOO[i][j]=e;
					greedyOOSize[i]++;
					break;
				}
			}
		}
	}

};
// during adding the greedy points from left to right
// check already the left neighbor is not in conflict with the current one
bool Problem_sl_greedy::checkGreedy(bool nextE, bool prevE, int i, int j,Point_2 current){
	bool even = ((j%2)==0);
	// check at most four points
	Point_2 temp;
	if(i< numberH-1){
		if(even == nextE){
			// one point above
			if(gridC[i+1][j]>0){
				temp = gridP[i+1][j].back();
				if(temp.y() < sigma +current.y() ){
					return false;
				}
			}
			// two greedy point above
			if(j > 0){
				temp = greedyLine[i+1][j-1];
				if(temp != Point_2(-1,-1)&& temp.y() < sigma +current.y()&& temp.x() > current.x()-sigma ){
					return false;
				}
			}
			if(j < numberV-1){
					temp = greedyLine[i+1][j+1];
					if(temp != Point_2(-1,-1)&& temp.y() < sigma +current.y() && temp.x() < current.x()+sigma ){
						return false;
					}
			}
		}
		else{
			//check if j is 0 or numV-1
			// two point above
			if(j > 0){
				if(gridC[i+1][j-1]>0){
					temp = gridP[i+1][j-1].back();
					if(temp.y() < sigma +current.y()&& temp.x() > current.x()-sigma ){
						return false;
					}
				}
			}
			if(j < numberV-1){
				if(gridC[i+1][j+1]>0){
					temp = gridP[i+1][j+1].back();
					if(temp.y() < sigma +current.y() && temp.x() < current.x()+sigma ){
						return false;
					}
				}
			}
			// one greedy point above
				temp = greedyLine[i+1][j];
				if(temp != Point_2(-1,-1)&& temp.y() < sigma +current.y() ){
					return false;
				}
		}
	}
	if(i> 0){
		if(even == prevE){
			// one point below
			if(gridC[i-1][j] > 0){
				temp = gridP[i-1][j].back();
				if(temp.y() > -sigma +current.y() ){
					return false;
				}
			}
			// two greedy points below
			if(j > 0){
					temp = greedyLine[i-1][j-1];
					if(temp != Point_2(-1,-1)&&temp.y() > -sigma +current.y()&& temp.x() > current.x()-sigma ){
						return false;
					}
			}
			if(j < numberV-1){
					temp = greedyLine[i-1][j+1];
					if(temp != Point_2(-1,-1)&&temp.y() > -sigma +current.y()&& temp.x() < current.x()+sigma  ){
						return false;
					}
			}
		}
		else{
			//check if j is 0 or numV-1
			// two point below
			if(j > 0){
				if(gridC[i-1][j-1] > 0){
					temp = gridP[i-1][j-1].back();
					if(temp.y() > -sigma +current.y()&& temp.x() > current.x()-sigma ){
						return false;
					}
				}
			}
			if(j < numberV-1){
				if(gridC[i-1][j+1] > 0){
					temp = gridP[i-1][j+1].back();
					if(temp.y() > -sigma +current.y()&& temp.x() < current.x()+sigma  ){
						return false;
					}
				}
			}
			//one greedy point below
				temp = greedyLine[i-1][j];
				if(temp != Point_2(-1,-1)&&temp.y() > -sigma +current.y() ){
					return false;
				}
		}
	}
	return true;
};

void Problem_sl_greedy::getSolution(vector<Point_2>& solution){
	solution.clear();
	int c =(markE == true)?0:1;
	int l;
	vector<vector<Point_2>> const* greedyBetween = NULL;//11
	for(int i = 0; i < numberH; i++){
		// exact solution and greedy solution on the chosen horizontal lines
		if(i%2 ==c){
			l = (mark[i] == true)?0:1;
			for(int j = 0; j < numberV; j++){
				if(j%2 == l){
					if(gridC[i][j] > 0){
						solution.push_back(gridP[i][j].back());

					}
				}
				else{
					if(greedyLine[i][j] != Point_2(-1,-1)) solution.push_back(greedyLine[i][j] );
				}
			}
		}
		// greedy solution in the middle lines
		else{
			// the greedy part in between
			if(i== 0 ||mark[i-1]== true){
				if(i == numberH-1 || mark[i+1]== true){
					greedyBetween = &greedyEE;
				}
				else{
					greedyBetween = &greedyOE;
				}
			}
			else{
				if(i == numberH-1 || mark[i+1]== true){
					greedyBetween = &greedyEO;
				}
				else{
					greedyBetween = &greedyOO;
				}
			}
			for(auto e: (*greedyBetween)[i]){
				if(e != Point_2(-1,-1)) solution.push_back(e);
			}
		}
	}
}

void Problem_sl_greedy::addPoint(Point_2 p){
	Problem::addThePoint(p);
	int x=ceil((p.x()-h_sigma)/sigma);
	int y=ceil((p.y()-h_sigma)/sigma);
	int& sum = ((y%2)==0)? sumE:sumO;
	gridC[y][x]++;
	gridP[y][x].push_front(p);
	if(gridC[y][x] == 1){
		if((x%2) == 0){
			counter[y].first++;
			if(mark[y]== false){
				 if(counter[y].first > counter[y].second){
					 mark[y]=true;
					 sum++;
				 }
			 }
			else 		sum++;
		}
		else{
			counter[y].second++;
			if(mark[y]== true){
				 if(counter[y].first < counter[y].second){
					 mark[y]=false;
					 sum++;
				 }
			}
			else 		sum++;
		}
		if(sumE > sumO){
			markE = true;
		}
		else markE = false;
		addGreedy(x,y,p);
	}
};
void Problem_sl_greedy::subPoint(Point_2 p){
	int x=ceil((p.x()-h_sigma)/sigma);
	int y=ceil((p.y()-h_sigma)/sigma);
	int& sum = (y%2==0)? sumE:sumO;
	if(gridC[y][x] <=0){

	}
	gridC[y][x]--;
	bool inSolution = (gridP[y][x].back() == p);
	gridP[y][x].remove(p);
	if(gridC[y][x] == 0){
		if(x%2 == 0){
			counter[y].first--;
			if(mark[y]== true){
				 if(counter[y].first < counter[y].second){
					 mark[y]=false;
				 }
				 else 		sum--;
			 }
		}
		else{
			counter[y].second--;
			if(mark[y]== false){
				 if(counter[y].first > counter[y].second){
					 mark[y]=true;
				 }
				 else 		sum--;
			}
		}
	if(sumE > sumO){
		markE = true;
	}
	else markE = false;
	}
	subGreedy(x, y, p,inSolution);
	Problem::subThePoint(p);
};
void Problem_sl_greedy::subPoint_index(unsigned int index){
Point_2 p = pVector[index];
subPoint(p);
};
// 0->1 at the ceil
void Problem_sl_greedy::addGreedy(int x, int y, Point_2& p){
	//std::cout<<"in addGreedy"<< std::endl;
	//print_Greedy();
	Point_2 prev = Point_2(-1,-1) ;
	Point_2 back = Point_2(-1,-1);
	Point_2 greedyBack = Point_2(-1,-1);
	Point_2 greedyPrev = Point_2(-1,-1);

	//std::cout<< "for back"<< std::endl;
	if(x < numberV-1){
		if(gridC[y][x+1] > 0){
			back = gridP[y][x+1].back();
			greedyBack = greedyLine[y][x+1];
			if(greedyLine[y][x+1]!=Point_2(-1,-1) &&p.x() > greedyBack.x()-sigma){
				//std::cout<<greedyLine[y][x+1].x()<<" "<<  greedyLine[y][x+1].y()<< std::endl;
				greedyLine[y][x+1] = Point_2(-1,-1);
				greedyLineSize[y][(x+1)%2]--;
			}
		}
	}
	//std::cout<< "　for prev"<< std::endl;
	if(x > 0){
		if(gridC[y][x-1] > 0){
			prev = gridP[y][x-1].back();
			greedyPrev = greedyLine[y][x-1];
			if(greedyPrev !=Point_2(-1,-1) &&p.x() < greedyPrev.x()+sigma){
				//std::cout<<greedyLine[y][x-1].x()<<" "<<  greedyLine[y][x-1].y()<< std::endl;
				greedyLine[y][x-1] = Point_2(-1,-1);
				greedyLineSize[y][(x-1)%2]--;
			}
		}
	}
	//std::cout<< "  affect 4 lines according to the x"<< std::endl;
		//	std::cout<< "next"<< std::endl;
	if(x%2 == 0){
		//std::cout<< "see upper part"<< std::endl;
		if(y < numberH-1){
			//std::cout<< "above"<< std::endl;
			if( greedyEE[y+1][x]!=Point_2(-1,-1) &&greedyEE[y+1][x].y() < sigma+ p.y()) {
				//std::cout<<greedyEE[y+1][x].x()<<" "<< greedyEE[y+1][x].y()<< std::endl;
				greedyEE[y+1][x] = Point_2(-1,-1);
				greedyEESize[y+1]--;
			}
			if(greedyOE[y+1][x]!=Point_2(-1,-1) &&greedyOE[y+1][x].y() < sigma+ p.y()){
				//std::cout<<greedyOE[y+1][x] .x()<<" "<<  greedyOE[y+1][x].y()<< std::endl;
				greedyOE[y+1][x] = Point_2(-1,-1);
				greedyOESize[y+1]--;
			}
			//std::cout<< "above prev"<< std::endl;
			if(x > 0){
				if(greedyEE[y+1][x-1]!=Point_2(-1,-1) &&greedyEE[y+1][x-1].y() < p.y()+sigma && greedyEE[y+1][x-1].x() > p.x()-sigma){
					//std::cout<<greedyEE[y+1][x-1] .x()<<" "<<  greedyEE[y+1][x-1].y()<< std::endl;
					greedyEE[y+1][x-1]= Point_2(-1,-1);
					greedyEESize[y+1]--;
				}
				if(greedyOE[y+1][x-1]!=Point_2(-1,-1)&&greedyOE[y+1][x-1].y() < p.y()+sigma && greedyOE[y+1][x-1].x() > p.x()-sigma){
					//std::cout<<greedyOE[y+1][x-1].x()<<" "<<  greedyOE[y+1][x-1].y()<< std::endl;
					greedyOE[y+1][x-1]= Point_2(-1,-1);
					greedyOESize[y+1]--;
				}
			}
			//std::cout<< " above back"<< std::endl;
			if(x < numberV-1){
				if(greedyEE[y+1][x+1]!=Point_2(-1,-1)&&greedyEE[y+1][x+1].y() < p.y()+sigma && greedyEE[y+1][x+1].x() < p.x()+sigma){
					//std::cout<<greedyEE[y+1][x+1].x()<<" "<<  greedyEE[y+1][x+1].y()<< std::endl;
					greedyEE[y+1][x+1]= Point_2(-1,-1);
					greedyEESize[y+1]--;
				}
				if(greedyOE[y+1][x+1]!=Point_2(-1,-1)&&greedyOE[y+1][x+1].y() < p.y()+sigma && greedyOE[y+1][x+1].x() < p.x()+sigma){
					//std::cout<<greedyOE[y+1][x+1].x()<<" "<<  greedyOE[y+1][x+1].y()<< std::endl;
					greedyOE[y+1][x+1]= Point_2(-1,-1);
					greedyOESize[y+1]--;
				}
			}
		}
		//std::cout<< "see below part"<< std::endl;
		if(y > 0){
			//std::cout<< "below"<< std::endl;
			if(greedyEE[y-1][x]!=Point_2(-1,-1)&&greedyEE[y-1][x].y() > -sigma+ p.y()){
				//std::cout<<greedyEE[y-1][x] .x()<<" "<<  greedyEE[y-1][x] .y()<< std::endl;
				greedyEE[y-1][x] = Point_2(-1,-1);
				greedyEESize[y-1]--;
			}
			if(greedyEO[y-1][x] !=Point_2(-1,-1)&&greedyEO[y-1][x].y() > -sigma+ p.y()){
				//std::cout<<greedyEO[y-1][x] .x()<<" "<<  greedyEO[y-1][x] .y()<< std::endl;
				greedyEO[y-1][x] = Point_2(-1,-1);
				greedyEOSize[y-1]--;
			}
			//std::cout<< " below prev"<< std::endl;
			if(x > 0){
				if(greedyEE[y-1][x-1]!=Point_2(-1,-1)&&greedyEE[y-1][x-1].y() > p.y()-sigma && greedyEE[y-1][x-1].x() > p.x()-sigma) {
					//std::cout<<greedyEE[y-1][x-1].x()<<" "<<  greedyEE[y-1][x-1].y()<< std::endl;
					greedyEE[y-1][x-1]= Point_2(-1,-1);
					greedyEESize[y-1]--;
				}
				if(greedyEO[y-1][x-1]!=Point_2(-1,-1)&&greedyEO[y-1][x-1].y() > p.y()-sigma && greedyEO[y-1][x-1].x() > p.x()-sigma){
					//std::cout<<greedyEO[y-1][x-1].x()<<" "<< greedyEO[y-1][x-1].y()<< std::endl;
					greedyEO[y-1][x-1]= Point_2(-1,-1);
					greedyEOSize[y-1]--;
				}
			}
			//std::cout<< " below back"<< std::endl;
			if(x < numberV-1){
				if(greedyEE[y-1][x+1]!=Point_2(-1,-1)&&greedyEE[y-1][x+1].y() > p.y()-sigma && greedyEE[y-1][x+1].x() < p.x()+sigma) {
					//std::cout<<greedyEE[y-1][x+1].x()<<" "<<  greedyEE[y-1][x+1].y()<< std::endl;
					greedyEE[y-1][x+1]= Point_2(-1,-1);
					greedyEESize[y-1]--;
				}
				if(greedyEO[y-1][x+1]!=Point_2(-1,-1)&&greedyEO[y-1][x+1].y() > p.y()-sigma && greedyEO[y-1][x+1].x() < p.x()+sigma){
					//std::cout<<greedyEO[y-1][x+1].x()<<" "<<  greedyEO[y-1][x+1].y()<< std::endl;
					greedyEO[y-1][x+1]= Point_2(-1,-1);
					greedyEOSize[y-1]--;
				}
			}
		}
	}
	else{
		//std::cout<< "see upper part"<< std::endl;
		if(y < numberH-1){
			//std::cout<< "above"<< std::endl;
			if(greedyEO[y+1][x]!=Point_2(-1,-1)&&greedyEO[y+1][x].y() < sigma+ p.y()){
				//std::cout<<greedyEO[y+1][x].x()<<" "<<  greedyEO[y+1][x].y()<< std::endl;
				greedyEO[y+1][x] = Point_2(-1,-1);
				greedyEOSize[y+1]--;
			}
			if(greedyOO[y+1][x]!=Point_2(-1,-1)&&greedyOO[y+1][x].y() < sigma+ p.y()) {
				//std::cout<<greedyOO[y+1][x].x()<<" "<<  greedyOO[y+1][x].y()<< std::endl;
				greedyOO[y+1][x] = Point_2(-1,-1);
				greedyOOSize[y+1]--;
			}
			//std::cout<< " above prev"<< std::endl;
			if(x > 0){
				if(greedyEO[y+1][x-1]!=Point_2(-1,-1)&&greedyEO[y+1][x-1].y() < p.y()+sigma && greedyEO[y+1][x-1].x() > p.x()-sigma){
					//std::cout<<greedyEO[y+1][x-1].x()<<" "<<  greedyEO[y+1][x-1].y()<< std::endl;
					greedyEO[y+1][x-1]= Point_2(-1,-1);
					greedyEOSize[y+1]--;
				}
				if(greedyOO[y+1][x-1]!=Point_2(-1,-1)&&greedyOO[y+1][x-1].y() < p.y()+sigma && greedyOO[y+1][x-1].x() > p.x()-sigma){
					//std::cout<<greedyOO[y+1][x-1].x()<<" "<< greedyOO[y+1][x-1].y()<< std::endl;
					greedyOO[y+1][x-1]= Point_2(-1,-1);
					greedyOOSize[y+1]--;
				}
			}
			//std::cout<< " above back"<< std::endl;
			if(x < numberV-1){
				if(greedyEO[y+1][x+1]!=Point_2(-1,-1)&&greedyEO[y+1][x+1].y() < p.y()+sigma && greedyEO[y+1][x+1].x() < p.x()+sigma) {
					//std::cout<<greedyEO[y+1][x+1].x()<<" "<<  greedyEO[y+1][x+1].y()<< std::endl;
					greedyEO[y+1][x+1]= Point_2(-1,-1);
					greedyEOSize[y+1]--;
				}
				if(greedyOO[y+1][x+1]!=Point_2(-1,-1)&&greedyOO[y+1][x+1].y() < p.y()+sigma && greedyOO[y+1][x+1].x() < p.x()+sigma){
					//std::cout<<greedyOO[y+1][x+1].x()<<" "<<  greedyOO[y+1][x+1].y()<< std::endl;
					greedyOO[y+1][x+1]= Point_2(-1,-1);
					greedyOOSize[y+1]--;
				}
			}
		}
		//std::cout<< "see below part"<< std::endl;
		if(y > 0){
			//std::cout<< "below"<< std::endl;
			if(greedyOE[y-1][x]!=Point_2(-1,-1)&&greedyOE[y-1][x].y() > -sigma+ p.y()){
				//std::cout<<	greedyOE[y-1][x].x()<<" "<<  	greedyOE[y-1][x].y()<< std::endl;
				greedyOE[y-1][x] = Point_2(-1,-1);
				greedyOESize[y-1]--;
			}
			if(greedyOO[y-1][x]!=Point_2(-1,-1)&&greedyOO[y-1][x].y() > -sigma+ p.y()) {
				//std::cout<<greedyOO[y-1][x].x()<<" "<< greedyOO[y-1][x].y()<< std::endl;
				greedyOO[y-1][x] = Point_2(-1,-1);
				greedyOOSize[y-1]--;
			}
			//std::cout<< " below prev"<< std::endl;
			if(x > 0){
				if(greedyOE[y-1][x-1]!=Point_2(-1,-1)&&greedyOE[y-1][x-1].y() > p.y()-sigma && greedyOE[y-1][x-1].x() > p.x()-sigma) {
					//std::cout<<greedyOE[y-1][x-1].x()<<" "<<  greedyOE[y-1][x-1].y()<< std::endl;
					greedyOE[y-1][x-1]= Point_2(-1,-1);
					greedyOESize[y-1]--;
				}
				if(greedyOO[y-1][x-1]!=Point_2(-1,-1)&&greedyOO[y-1][x-1].y() > p.y()-sigma && greedyOO[y-1][x-1].x() > p.x()-sigma){
					//std::cout<<greedyOO[y-1][x-1].x()<<" "<<  greedyOO[y-1][x-1].y()<< std::endl;
					greedyOO[y-1][x-1]= Point_2(-1,-1);
					greedyOOSize[y-1]--;
				}
			}
			//std::cout<< " below back"<< std::endl;
			if(x < numberV-1){
				if(greedyOE[y-1][x+1]!=Point_2(-1,-1)&&greedyOE[y-1][x+1].y() > p.y()-sigma && greedyOE[y-1][x+1].x() < p.x()+sigma){
					//std::cout<<greedyOE[y-1][x+1].x()<<" "<< greedyOE[y-1][x+1].y()<< std::endl;
					greedyOE[y-1][x+1]= Point_2(-1,-1);
					greedyOESize[y-1]--;
				}
				if(greedyOO[y-1][x+1]!=Point_2(-1,-1)&&greedyOO[y-1][x+1].y() > p.y()-sigma && greedyOO[y-1][x+1].x() < p.x()+sigma) {
					//std::cout<<greedyOO[y-1][x+1].x()<<" "<<  greedyOO[y-1][x+1].y()<< std::endl;
					greedyOO[y-1][x+1]= Point_2(-1,-1);
					greedyOOSize[y-1]--;
				}
			}
		}
	}
}
void Problem_sl_greedy::subGreedy(int x, int y, Point_2& p,bool inSolution){
	//std::cout<< "in subGreedy"<< std::endl;
	if(greedyLine[y][x]== p){
		greedyLine[y][x]= Point_2(-1,-1);
		greedyLineSize[y][x%2]--;
	}
	//EE
	if(greedyEE[y][x]== p){
		greedyEE[y][x]= Point_2(-1,-1);
		greedyEESize[y]--;
	}
	//EO
	if(greedyEO[y][x]== p){
		greedyEO[y][x]= Point_2(-1,-1);
		greedyEOSize[y]--;
	}
	//OE
	if(greedyOE[y][x]== p){
		greedyOE[y][x]= Point_2(-1,-1);
		greedyOESize[y]--;
	}
	//OO
	if(greedyOO[y][x]== p){
		greedyOO[y][x]= Point_2(-1,-1);
		greedyOOSize[y]--;
	}
	if(inSolution){
		if(x > 0 && greedyLine[y][x-1] != Point_2(-1,-1)){
			greedyLine[y][x-1]=Point_2(-1,-1);
			greedyLineSize[y][(x-1)%2]--;
		}
		if(x < numberV-1 && greedyLine[y][x+1] != Point_2(-1,-1)){
			greedyLine[y][x+1]=Point_2(-1,-1);
			greedyLineSize[y][(x+1)%2]--;
		}
		bool even = ((x%2)==0);
		vector<vector<Point_2>>* greedyAboveE = NULL;
		vector<vector<Point_2>>* greedyAboveO = NULL;
		vector<vector<Point_2>>* greedyBelowE = NULL;
		vector<vector<Point_2>>* greedyBelowO = NULL;
		vector<unsigned int>*  greedyAboveESize = NULL;
		vector<unsigned int>*  greedyAboveOSize = NULL;
		vector<unsigned int>*  greedyBelowESize = NULL;
		vector<unsigned int>*  greedyBelowOSize = NULL;

		if(even){
			greedyAboveE = &greedyEE;
			greedyAboveO = &greedyOE;
			greedyBelowE = &greedyEE;
			greedyBelowO = &greedyEO;
			greedyAboveESize = &greedyEESize;
			greedyAboveOSize = &greedyOESize;
			greedyBelowESize = &greedyEESize;
			greedyBelowOSize = &greedyEOSize;
		}
		else{
			greedyAboveE = &greedyEO;
			greedyAboveO = &greedyOO;
			greedyBelowE = &greedyOE;
			greedyBelowO = &greedyOO;
			greedyAboveESize = &greedyEOSize;
			greedyAboveOSize = &greedyOOSize;
			greedyBelowESize = &greedyOESize;
			greedyBelowOSize = &greedyOOSize;
		}
		if(y < numberH-1){
			// (y+1) EE OE
			//x
			if((*greedyAboveE)[y+1][x] != Point_2(-1,-1)){
				(*greedyAboveE)[y+1][x] = Point_2(-1,-1);
				(*greedyAboveESize)[y+1]--;
			}
			if((*greedyAboveO)[y+1][x] != Point_2(-1,-1)){
				(*greedyAboveO)[y+1][x] = Point_2(-1,-1);
				(*greedyAboveOSize)[y+1]--;
			}
			//x-1
			if(x > 0){
				if((*greedyAboveE)[y+1][x-1] != Point_2(-1,-1)){
					(*greedyAboveE)[y+1][x-1]= Point_2(-1,-1);
					(*greedyAboveESize)[y+1]--;
				}
				if((*greedyAboveO)[y+1][x-1] != Point_2(-1,-1)){
					(*greedyAboveO)[y+1][x-1]= Point_2(-1,-1);
					(*greedyAboveOSize)[y+1]--;
				}
			}
			//x+1
			if(x < numberV-1){
				if((*greedyAboveE)[y+1][x+1] != Point_2(-1,-1)){
					(*greedyAboveE)[y+1][x+1]= Point_2(-1,-1);
					(*greedyAboveESize)[y+1]--;
				}
				if((*greedyAboveO)[y+1][x+1] != Point_2(-1,-1)){
					(*greedyAboveO)[y+1][x+1]= Point_2(-1,-1);
					(*greedyAboveOSize)[y+1]--;
				}
			}
		}
		if(y > 0){
			//(y-1）(EE,EO)
			//x
			if((*greedyBelowE)[y-1][x] != Point_2(-1,-1)){
				(*greedyBelowE)[y-1][x] = Point_2(-1,-1);
				(*greedyBelowESize)[y-1]--;
			}
			if((*greedyBelowO)[y-1][x] != Point_2(-1,-1)){
				(*greedyBelowO)[y-1][x] = Point_2(-1,-1);
				(*greedyBelowOSize)[y-1]--;
			}
			//x-1
			if(x > 0){
				if((*greedyBelowE)[y-1][x-1] != Point_2(-1,-1)){
					(*greedyBelowE)[y-1][x-1]= Point_2(-1,-1);
					(*greedyBelowESize)[y-1]--;
				}
				if((*greedyBelowO)[y-1][x-1] != Point_2(-1,-1)){
					(*greedyBelowO)[y-1][x-1]= Point_2(-1,-1);
					(*greedyBelowOSize)[y-1]--;
				}
			}
			//x+1
			if(x < numberV-1){
				if((*greedyBelowE)[y-1][x+1] != Point_2(-1,-1)){
					(*greedyBelowE)[y-1][x+1]= Point_2(-1,-1);
					(*greedyBelowESize)[y-1]--;
				}
				if((*greedyBelowO)[y-1][x+1] != Point_2(-1,-1)){
					(*greedyBelowO)[y-1][x+1]= Point_2(-1,-1);
					(*greedyBelowOSize)[y-1]--;
				}
			}
		}
	}
	//std::cout<< "out subGreedy"<< std::endl;
}
void Problem_sl_greedy::forcedDeletion(unsigned int  index){
	//std::cout<< "in forcedDeletion_sl"<<index<< std::endl;
    //debug();
   // double x,y;
	//x=ceil((pVector[index].x()-h_sigma)/sigma);
	//y=ceil((pVector[index].y()-h_sigma)/sigma);
	//assert(gridC[x][y] == 0);
	//assert(mVector[index] == 1);
    //debug();
	//std::cout<< "out forcedDeletion_cg"<<index<< std::endl;
}
void Problem_sl_greedy::forcedAddition(unsigned int  index){
	//assert(mVector[index] == 0);
}
void Problem_sl_greedy::freeDeletion(unsigned int  index){
	//assert(mVector[index] == 1);
}
void Problem_sl_greedy::freeAddition(unsigned int  index){
	//assert(mVector[index] == 0);
}
void Problem_sl_greedy::printSolution(){
	vector<Point_2> solution;
	getSolution(solution);
	std::cout<<"################################################################################"<<std::endl;
	for(auto e: solution){
		std::cout<< lookUp(e)<<" ";
	}
	std::cout<<std::endl<<"solution size: "<< solution.size()<< std::endl;
	std::cout<<"################################################################################"<<std::endl;
};


/*
 * problem_slk.cpp
 *
 *  Created on: Nov 13, 2019
 *      Author: guangping
 */
#include "problem_slk.h"
Problem_slk::Problem_slk(double w, const char* fileName,int k_i):Problem(w,fileName){
	k = k_i;
	numberV = ceil(width/sigma);
	//assert(k < numberV);
	numberH=ceil(height/sigma);
	gridP=vector<vector<set<Point_2>>>(numberH,vector<set<Point_2>>(numberV, set<Point_2>{}));
	gridC = vector<vector<int>>(numberH,vector<int>(numberV,0)) ;
	gridS = vector<vector<vector<Point_2>>>(numberH,vector<vector<Point_2>>(numberV,vector<Point_2>(k))) ;
	preGridC = vector<vector<int>>(numberH,vector<int>(k-1,0)) ;
	preGridS = vector<vector<vector<Point_2>>>(numberH,vector<vector<Point_2>>(k-1,vector<Point_2>(k)));
	counter=vector<vector<int>>(numberH,vector<int>(k+1,0));
	mark=vector<int>(numberH,-1);
	sumE= 0;
	sumO = 0;
	markE = false;
};
void Problem_slk::setPoint(){
	int x, y;
	for(auto p: pVector){
		x=ceil((p.x()-h_sigma)/sigma);
		y=ceil((p.y()-h_sigma)/sigma);
		gridP[y][x].insert(p);
	}
};
void Problem_slk::simple(){
	//std::cout<< "in simple"<< std::endl;
	setPoint();
	int max_index = -1;
	int max_value = -1;
	int index = -1;
	for(int i = 0; i < numberH; i++){
		for(int j = 0; j < numberV; j++){
			getMIS_k(j,i);
			counter[i][j%(k+1)]+= gridC[i][j];
		}
		for(int j = 0; j < k-1; j++){
			getMIS_pre(j,i);
			counter[i][(j+2)%(k+1)]+= preGridC[i][j];
		}
		for(auto e:counter[i]){
			index++;
			if(e > max_value) {
				max_index = index;
				max_value = e;
			}
		}
		/*if(max_value <= -1){
			printCounter();
			std::cout<<"max_value: "<<max_value<<std::endl;
		}*/
		//assert(max_value > -1);
		mark[i] = max_index;
		if(i%2 == 0) sumE+= max_value;
		else sumO+= max_value;
		max_index = -1;
		max_value = -1;
		index = -1;
	}
	//********************************
	if(sumE > sumO){
		markE = true;
	}
	else markE = false;
}
// get maximum independent set of k lines
void Problem_slk::getMIS_k(int grid_x, int grid_y){
	gridC[grid_y][grid_x]=0;
	Point_2 start = Point_2(-1,-1);
	int x = grid_x;
	int sum = 0;
	vector<Point_2> solution(k);
	solution.clear();
	for(int i =0; i < k; i++){
		auto it = gridP[grid_y][x].lower_bound(start);
		if(it != gridP[grid_y][x].end()){
			solution.push_back(*it);
			sum++;
			start = (*it)+Vector(sigma,0);
		}
		x++;
		if(x == numberV) break;
	}
	gridS[grid_y][grid_x]= solution;
	gridC[grid_y][grid_x] = sum;
}
void Problem_slk::getMIS_pre(int grid_x, int grid_y){
	//std::cout<< "in getMIS_pre "<< grid_x <<" "<< grid_y<< std::endl;
	//assert(grid_x < k-1);
	//assert(grid_y < numberH);
	preGridC[grid_y][grid_x]=0;
	Point_2 start = Point_2(-1,-1);
	int sum = 0;
	vector<Point_2> solution(k);
	solution.clear();
	for(int i =0; i <= grid_x; i++){
		auto it = gridP[grid_y][i].lower_bound(start);
		if(it != gridP[grid_y][i].end()){
			solution.push_back(*it);
			sum++;
			start = (*it)+Vector(sigma,0);
		}
	}
	preGridS[grid_y][grid_x]= solution;
	preGridC[grid_y][grid_x] = sum;
	//std::cout<< "out getMIS_pre "<< sum << std::endl;
}
void Problem_slk::forcedDeletion(unsigned int  index){};
void Problem_slk::forcedAddition(unsigned int  index){};
void Problem_slk::freeAddition(unsigned int  index){};
void Problem_slk::freeDeletion(unsigned int  index){};


void Problem_slk::getSolution(vector<Point_2>& solution){
	int c =(markE == true)?0:1;
	for(int i = 0; i < numberH; i++){
		if(i%2!=c)continue;
		getOneGroup(solution,i,mark[i]);
	}
}

// get solution from the
void Problem_slk::getOneGroup(vector<Point_2>& solution, int line,int group){
	//assert(line < numberH);
	//assert(group < k+1);
	for(int i = 0; i< numberV; i++){
		if(i %(k+1)== group){
			for(auto e: gridS[line][i]){
				solution.push_back(e);
			}
		}
	}
	if(group-2 > -1){
		for(auto e: preGridS[line][group-2]){
			solution.push_back(e);
		}
	}
	//assert(independency(solution,sigma, sigma));
}
void Problem_slk::getFullSolution(vector<Point_2>& solution, int line,int group){
	//assert(line < numberH);
	//assert(group < k+1);
	int s = (group+k)%(k+1);
	for(int i = 0; i < numberV; i++){
		if(i%(k+1)== s) continue;
		else{
			for(auto e: gridP[line][i]){
				if(extendible(solution, e, sigma,sigma)){
					solution.push_back(e);
				}
			}
		}
	}
	//assert(independency(solution,sigma, sigma));
}
//-------------------------debug----------------------------------

// if one point added
//(it can change k gridS containing its grid)
//(it can change until k-1 preGridS containing its grid)
void Problem_slk::addPoint(Point_2 p){
	auto t0 = std::chrono::high_resolution_clock::now();
	Problem::addThePoint(p);
	//std:: cout<< "in addPoint"<< std::endl;
	int x=ceil((p.x()-h_sigma)/sigma);
	int y=ceil((p.y()-h_sigma)/sigma);
	int x_index;
	gridP[y][x].insert(p);
	//std:: cout<< "add point:"<< p.x() << ","<< p.y()<< std::endl;
	//std:: cout<< "add point_grid:"<< x << ","<< y<< std::endl;
	// update grid
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds d = std::chrono::duration_cast< std::chrono::nanoseconds >(fs);
	t0 = std::chrono::high_resolution_clock::now();
	for(int i = 0; i <k;i++){
		x_index = x-i;
		if(x_index >= 0) addPoint_recompute(x_index,y);
	}
	//update preGrid
	if(x < k-1){
		for(int i = x; i < k-1; i++){
			addPoint_recompute_pre(i,y);
		}
	}
	t1 = std::chrono::high_resolution_clock::now();
	fs = t1 - t0;
	d = std::chrono::duration_cast< std::chrono::nanoseconds >(fs);
	//std:: cout<< "out addPoint"<< std::endl;
};
// for gridS, gridC
void Problem_slk::addPoint_recompute(int x, int y){
	int& sum = ((y%2)==0)? sumE:sumO;
	int oldCount = gridC[y][x];
	int oldMAX = counter[y][mark[y]];
	getMIS_k(x, y);
	int index;
	int newCount = gridC[y][x];
	if(newCount > oldCount){
		index = x%(k+1);
		//assert(newCount == oldCount+1);
		counter[y][index]++;
		if(counter[y][index] > oldMAX){
			mark[y]= index;
			sum++;
			if(sumE > sumO){
				markE = true;
			}
			else markE = false;
		}
	}
	else return;

}
// for preGridS, preGridC
void Problem_slk::addPoint_recompute_pre(int x, int y){
	//assert(x < k-1);
	int& sum = ((y%2)==0)? sumE:sumO;
	int oldCount = preGridC[y][x];
	int index = (x+2)%(k+1);
	int oldMAX = counter[y][mark[y]];
	getMIS_pre(x, y);
	int newCount = preGridC[y][x];
	if(newCount > oldCount){
		//assert(newCount == oldCount+1);
		counter[y][index]++;
		if(counter[y][index] > oldMAX){
			mark[y]= index;
			sum++;
			if(sumE > sumO){
				markE = true;
			}
			else markE = false;
		}
	}
	else return;

}
void Problem_slk::subPoint(Point_2 p){
	int x=ceil((p.x()-h_sigma)/sigma);
	int y=ceil((p.y()-h_sigma)/sigma);
	int x_index;
	gridP[y][x].erase(p);
	for(int i = 0; i <k;i++){
		x_index = x-i;
		if(x_index >= 0) subPoint_recompute(x_index,y);
	}
	//update preGrid
	if(x < k-1){
		for(int i = x; i < k-1; i++){
			subPoint_recompute_pre(i,y);
		}
	}
	Problem::subThePoint(p);
};
// for gridS, gridC
void Problem_slk::subPoint_index(unsigned int index){
	Point_2 p = pVector[index];
	subPoint(p);
};
void Problem_slk::subPoint_recompute(int x, int y){
	int oldCount = gridC[y][x];
	int& sum = (y%2==0)? sumE:sumO;
	getMIS_k(x, y);
	int index;
	int max_oldValue = -1;
	int max_value = -1;
	int index_candidate= -1;
	int max_index = -1;
	int newCount = gridC[y][x];
	if(newCount < oldCount){
		//assert(newCount == oldCount-1);
		index = x%(k+1);
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
			if(max_oldValue < max_value){
				std::cout<< "max_oldValue:"<< max_oldValue<< std::endl;
				std::cout<< "max_value:"<< max_value<< std::endl;
			}
			//assert(max_oldValue >= max_value);
			sum -= max_oldValue;
			sum+= max_value;
			if(sumE > sumO){
			markE = true;
			}
			else markE = false;
		}
	}
}
// for preGridS, preGridC
void Problem_slk::subPoint_recompute_pre(int x, int y){
	//assert(x < k-1);
	int index_candidate= -1;
	int max_value = -1;
	int max_index = -1;
	int max_oldValue = -1;
	int& sum = ((y%2)==0)? sumE:sumO;
	int oldCount = preGridC[y][x];
	int index = (x+2)%(k+1);
	getMIS_pre(x, y);
	int newCount = preGridC[y][x];
	if(newCount < oldCount){
		counter[y][index]--;
		if(mark[y]== index){
			max_oldValue = counter[y][index]+1;
			//std::cout<< "change sum:"<< y%2<< std::endl;
			for(auto e:counter[y]){
				index_candidate++;
				if(e > max_value) {
					max_index = index_candidate;
					max_value = e;
				}
			}
			mark[y] =max_index;
			//assert(max_oldValue >= max_value);
			sum -= max_oldValue;
			sum+= max_value;
			if(sumE > sumO){
				markE = true;
			}
			else markE = false;
		}
	}
	else return;
}


void Problem_slk::printSolution(){
	vector<Point_2> solution;
	getSolution(solution);
	std::cout<<"################################################################################"<<std::endl;
	for(auto e: solution){
		std::cout<< lookUp(e)<<" ";
	}
	std::cout<<std::endl<<"solution size: "<< solution.size()<< std::endl;
	std::cout<<"################################################################################"<<std::endl;
};

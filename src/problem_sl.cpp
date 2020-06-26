/*
 * grid.cpp
 *
 *  Created on: Oct 17, 2019
 *      Author: guangping
 */
#include "problem_sl.h"
Problem_sl::Problem_sl(double w, const char* fileName):Problem(w,fileName){
	numberV = ceil(width/sigma);
	numberH=ceil(height/sigma);
	gridP=vector<vector<list<Point_2>>>(numberH,vector<list<Point_2>>(numberV, list<Point_2>{}));
	gridC=vector<vector<int>>(numberH,vector<int>(numberV, 0));
	counter=vector<intPair>(numberH,intPair(0,0));
	mark=vector<bool>(numberH,false);
	sumE= 0;
	sumO = 0;
	markE = false;
};
void Problem_sl::setPoint(){
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
void Problem_sl::simple(){
	std::cout<<"enter simple"<< std::endl;
	setPoint();
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
}

void Problem_sl::addPoint(Point_2 p){
	auto t0 = std::chrono::high_resolution_clock::now();
	Problem::addThePoint(p);
	int x=ceil((p.x()-h_sigma)/sigma);
	int y=ceil((p.y()-h_sigma)/sigma);
	int& sum = ((y%2)==0)? sumE:sumO;
	gridC[y][x]++;
	gridP[y][x].push_front(p);
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds d = std::chrono::duration_cast< std::chrono::nanoseconds >(fs);
	t0 = std::chrono::high_resolution_clock::now();
	if(gridC[y][x] == 1){
		if((x%2) == 0){
			counter[y].first++;
			if(mark[y]== false){
				 if(counter[y].first > counter[y].second){
					 //TODO: the solution need to updated
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
		//TODO: the solution need to updated
		if(sumE > sumO){
			markE = true;
		}
		else markE = false;
	}
	t1 = std::chrono::high_resolution_clock::now();
	fs = t1 - t0;
	d = std::chrono::duration_cast< std::chrono::nanoseconds >(fs);
};
void Problem_sl::subPoint(Point_2 p){
	int x=ceil((p.x()-h_sigma)/sigma);
	int y=ceil((p.y()-h_sigma)/sigma);
	int& sum = (y%2==0)? sumE:sumO;
	//assert(gridC[y][x] > 0);
	gridC[y][x]--;
	gridP[y][x].remove(p);
	if(gridC[y][x] == 0){
		if(x%2 == 0){
			counter[y].first--;
			if(mark[y]== true){
				 if(counter[y].first < counter[y].second){
					 //TODO: the solution need to updated
					 mark[y]=false;
				 }
				 else sum--;
			 }
		}
		else{
			//std::cout<<std::endl<< "in subPoint_sl　*"<< std::endl;
			counter[y].second--;
			if(mark[y]== false){
				//std::cout<<std::endl<< "in subPoint_sl　**"<< std::endl;
				 if(counter[y].first > counter[y].second){
					 mark[y]=true;
				 }
				 else{
					 sum--;
					// std::cout<<std::endl<< "in subPoint_sl　***"<< std::endl;
				 }
			}
		}
	//TODO: the solution need to updated
	if(sumE > sumO){
		markE = true;
	}
	else markE = false;
	}
	Problem::subThePoint(p);
};
void Problem_sl::subPoint_index(unsigned int index){
Point_2 p = pVector[index];
subPoint(p);
};
void Problem_sl::freeDeletion(unsigned int  index){
	//assert(mVector[index] == 1);
}
void Problem_sl::freeAddition(unsigned int  index){
	//assert(mVector[index] == 0);
}
void Problem_sl::printSolution(){
	std::cout<<"################################################################################"<<std::endl;
	std::cout<<"solution: ";
	int c =(markE == true)?0:1;
	int l;
	for(int i = c; i < numberH; i+=2){
		l = (mark[i] == true)?0:1;
		for(int j = l; j < numberV; j+=2){
			if(gridC[i][j] > 0){
				std::cout<<  lookUp(gridP[i][j].back())<<" ";
			}
		}
	}
	if(markE) std::cout<<std::endl<<"solution size: "<< sumE<< std::endl;
	else std::cout<<std::endl<<"solution size: "<< sumO<< std::endl;
	std::cout<<"################################################################################"<<std::endl;
};


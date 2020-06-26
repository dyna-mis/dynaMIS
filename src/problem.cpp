/*
 * problem.cpp
 *
 *  Created on: Sep 13, 2019
 *      Author: guangping
 */
#include "problem.h"
Problem::Problem(double sig, const char* fileName){
	sigma = sig;
	readFile(fileName);
	int i = 0;
	for (PointVector::iterator it = pVector.begin() ; it != pVector.end(); ++it){
		luTable[pair<double, double>((*it).x(),(*it).y())] =i;
		i++;
	}
	mVector= vector<short>(size*2,0);
}
void Problem::printSolution(){
	std::cout<<"################################################################################"<<std::endl;
	std::cout<<"solution: ";
	int i = 0;
	int s = 0;
	for(auto e: mVector){
		if (e== 1){
			std::cout<< i << " ";
			s++;
		}
		i++;
	}
	std::cout<<std::endl<<"solution size: "<< s<< std::endl;
	std::cout<<"################################################################################"<<std::endl;
}
double Problem::getSigma(){return sigma;};
// construct the Problem with fill information of the input file
void Problem::readFile(const char* fileName){
	//cout<< "in readFile"<<endl;
	ifstream fp;
	//fp.open("Debug/instance.cnf",std::ios::in);
	fp.open(fileName,std::ios::in);
	if(!fp.is_open()){
		std::cout<< fileName<<std::endl;
		perror("read file fails");
		exit(EXIT_FAILURE);
	}
	string buff;
	char head;
   	getline(fp,buff);
   	getline(fp,buff);
   	while(!fp.eof()){
   		if(buff.empty()) continue;
		head =buff.at(0);
		if(head == 'p'){
			break;
		}
	  getline(fp,buff);
	}
   	//std::cout<< buff<< std::endl;
   	// Parse the p line
   	char* str = strdup(buff.c_str());
    const char s[2] = " ";
    strtok(str, s);
    strtok(NULL, s);
    size = atoi(strtok(NULL, s));
    pVector.reserve(size*2);
    width = atof(strtok(NULL, s));
    height = atof(strtok(NULL, s));
    double x,y;
    h_sigma = sigma/2;
    double w_bound = width-sigma/2;
    double h_bound = height-sigma/2;
   	for(unsigned int i = 0; i < size; i++){
		getline(fp,buff);
		if(buff.empty()) continue;
		str = strdup(buff.c_str());
		strtok(str, s);
		x =atof(strtok(NULL, s));
		y =atof(strtok(NULL, s));
		// filter
		if(x< h_sigma || y < h_sigma) continue;
		if(x> w_bound || y > h_bound) continue;
		pVector.push_back(Point_2(x,y));
   	}
    size = pVector.size();
};
//+++++++++++++++++++++++++get MIS++++++++++++++++++++++++++++++++++
// get conflict graph of current problem
int Problem::lookUp(Point_2& Point){
	//std::cout <<"in Problem::lookUp"<<Point.x()<<" "<< Point.y()<<std::endl;
	doublePair p(Point.x(), Point.y());
	std::unordered_map<doublePair,int, boost::hash<doublePair>>::const_iterator got = luTable.find(p);
	 if (got != luTable.end()){
		 // std::cout <<"out Problem::lookUp"<< std::endl;
		 return got->second;
	 }
	 // std::cout <<"out Problem::lookUp"<< std::endl;
	return -1;
};
//-------------------------get MIS--------------------------------
double Problem::getWidth(){
	return width;
};
double  Problem::getHeight(){
	return height;
};
int Problem::getSize(){
	return size;
};
int Problem::counter(unsigned int i){
	unsigned int c = 0;
	for(unsigned int j = 0; j < size; j++){
		if(i == j) continue;
		if(mVector[j]==1){
			if(isConflict(pVector[i].x(), pVector[i].y(), pVector[j].x(), pVector[j].y(), sigma, sigma)){
				c++;
			}
		}
	}
	return c;
};
//+++++++++++++++++++++++++dynamic changing+++++++++++++++++++++++



void Problem::addThePoint(Point_2 p){
	pVector.push_back(p);
	luTable[pair<double, double>(p.x(),p.y())] =size;
	mVector[size] = 0;
	size++;

};
void Problem::subThePoint(Point_2 p){
	//debug();
	int index = lookUp(p);
	int replace = size-1;
	doublePair p_pair(p.x(),p.y());
	if(index != replace){
		pVector[index]= pVector[replace];
		//assert(pVector[replace].x() >= 0);
		//assert(pVector[replace].y() >= 0);
		doublePair rep_pair(pVector[replace].x(), pVector[replace].y());
		auto it = luTable.find(rep_pair);
		//assert(it != luTable.end());
		if(it != luTable.end()){
			it->second = index;
		}
		pVector[index]= pVector[replace];
		mVector[index]= mVector[replace];
		mVector[replace] = -1;
	}
	luTable.erase(p_pair);
	//assert(number == 1);
	pVector.pop_back();
	size--;
};
//-------------------------dynamic changing-----------------------

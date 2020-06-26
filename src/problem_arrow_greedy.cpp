/*
 * problem_arrow_greedy.cpp
 *
 *  Created on: Nov 23, 2019
 *      Author: ping
 */
#include "problem_arrow_greedy.h"
Problem_arrow_greedy::Problem_arrow_greedy(double w, const char* fileName):Problem(w,fileName){
	numberH=ceil(height/sigma);
	numberV = ceil(width/sigma);
	lines =vector<set<Point_2>>(numberH,set<Point_2>{});
	solution =vector<arrowLine>(numberH,arrowLine());
	sumE= 0;
	sumO = 0;
	markE = false;
	greedyLine = vector<vector<Point_2>>(numberH,vector<Point_2>(numberV,Point_2(-1,-1)));
	greedyLineSize =vector<int>(numberH,0);
};
void Problem_arrow_greedy::setPoint(){
    //std::cout<< "in setPoint"<< std::endl;
	int y;
	//std::cout<< "pVector has "<<pVector.size()<< std::endl;
	for(auto p: pVector){
	   // std::cout<< "in for"<< std::endl;
		y=ceil((p.y()-h_sigma)/sigma);
		//if(true){
		/*if(!(y < numberH)){
			std::cout<< "p_y: "<<p.y()<< std::endl;
			std::cout<< "y: "<<y<< std::endl;
			std::cout<< "numberH: "<<numberH<< std::endl;
		}*/
		//assert(y < numberH);
		//std::cout<< "in for 1"<< std::endl;
		//std::cout<< "size: "<<lines.size()<< std::endl;
		lines[y].insert(p);
	    //std::cout<< "next round"<< std::endl;
	}
    //std::cout<< "in setPoint A"<< std::endl;
    //std::cout<< "out setPoint"<< std::endl;
};
// get maximum independent set of one line
void Problem_arrow_greedy::setArrow(int grid_y){
	Point_2 start = Point_2(-1,-1);
	solution[grid_y].clear();
	auto it = lines[grid_y].lower_bound(start);
	while(it != lines[grid_y].end()){
		solution[grid_y].insert(*it);
		start = (*it)+Vector(sigma,0);
		it = lines[grid_y].lower_bound(start);
	}
}
// get maximum independent set of one line
/*void Problem_arrow_greedy::getMISOneLine(int grid_y, vector<Point_2>& MIS ){
	Point_2 start = Point_2(-1,-1);
	auto it = lines[grid_y].lower_bound(start);
	while(it != lines[grid_y].end()){
		MIS.push_back(*it);
		start = (*it)+Vector(sigma,0);
		it = lines[grid_y].lower_bound(start);
	}
}
// get maximum independent set of one line
void Problem_arrow_greedy::debugArrow(int grid_y, set<Point_2>& MIS ){
	Point_2 start = Point_2(-1,-1);
	MIS.clear();
	auto it = lines[grid_y].lower_bound(start);
	while(it != lines[grid_y].end()){
		MIS.insert(*it);
		start = (*it)+Vector(sigma,0);
		it = lines[grid_y].lower_bound(start);
	}
}*/
void Problem_arrow_greedy::simple(){
	auto t0 = std::chrono::high_resolution_clock::now();
    //std::cout<< "in simple"<< std::endl;
	setPoint();
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds d = std::chrono::duration_cast< std::chrono::nanoseconds >(fs);
	t0 = std::chrono::high_resolution_clock::now();
	for(int i = 0; i < numberH; i++){
		setArrow(i);
	}
	for(int i= 0; i < numberH; i+=2){
		sumE+= solution[i].size();
	}
	for(int i= 1; i < numberH; i+=2){
		sumO+= solution[i].size();
	}
	if(sumE > sumO){
		markE = true;
	}
	else markE = false;
	greedy();
	t1 = std::chrono::high_resolution_clock::now();
	fs = t1 - t0;
	d = std::chrono::duration_cast< std::chrono::nanoseconds >(fs);
	vector<Point_2> solution;
	solution.clear();
	getTheSolution(solution);
	getGreedySolution(solution);
	//debugGreedy(true);
    //std::cout<< "out simple"<< std::endl;
};
void Problem_arrow_greedy::greedy(){
    //std::cout<< "in greedy"<< std::endl;
	set<Point_2>* solution_below = NULL;
	set<Point_2>* solution_above = NULL;
	int x;
	double last = -DBL_MAX;
	for(int i = 0; i < numberH; i++){
		//std::cout<< "i:"<< i<< std::endl;
		solution_below = NULL;
		solution_above = NULL;
		last = -DBL_MAX;
		// TODO: extend for ordered set (same issue as in the TODO of slk class)
		if(i > 0){
			solution_below = &solution[i-1];
		}
		if(i < numberH-1){
			solution_above = &solution[i+1];
		}
		for(auto e: lines[i]){
			if(e.x()< last) continue;
			if(solution_below != NULL && !extendible_OrderedSET((*solution_below),e,sigma,sigma)){
				//std::cout<< "out extendible_OrderedSET"<< std::endl;
				continue;
			}
			if(solution_above != NULL && !extendible_OrderedSET((*solution_above),e,sigma,sigma)){
				//std::cout<< "out extendible_OrderedSET"<< std::endl;
				continue;
			}
			//std::cout<< "Point $"<< std::endl;
			x=ceil((e.x()-h_sigma)/sigma);
			//std::cout<< "Point $$"<< std::endl;
			if(greedyLine[i][x] == Point_2(-1,-1)){
				greedyLine[i][x] = e;
				last = e.x()+sigma;
				greedyLineSize[i]++;
			}
			//std::cout<< "Point $$$$$"<< std::endl;
		}
	}
    //std::cout<< "out greedy"<< std::endl;
}
// Greedy
/*void Problem_arrow_greedy::printGreedy(){
	std::cout<< "in Problem_arrow_greedy::printGreedy"<< std::endl;
	int count = 0;
	for(int i = 0; i < numberH; i++){
		count =0;
		for(auto e: greedyLine[i]){
			if(e != Point_2(-1,-1)){
				std::cout<<"("<< e.x()<<","<< e.y()<<") ";
				count++;
			}
		}
		assert(count == greedyLineSize[i]);
		std::cout<< std::endl;
	}
	std::cout<< "out Problem_arrow_greedy::printGreedy"<< std::endl;
};
// get the full solution in three continuous line , upper-line and lower line get maximum independent set and then extend the solution with nodes in the middle
void Problem_arrow_greedy::getGreeyFullLines(vector<Point_2>& fullSolution,int lower,int upper){
	assert(lower +2 == upper);
	if(lower >= 0) getMISOneLine(lower,fullSolution);
	if(upper < numberH) getMISOneLine(upper,fullSolution);
	for(auto e: lines[lower+1]){
		if(extendible(fullSolution,e,sigma,sigma)){
			fullSolution.push_back(e);
		}
	}
};
void Problem_arrow_greedy::debugGreedy(bool full){
	//std::cout<< "in Problem_arrow_greedy::debugGreedy"<< std::endl;
	vector<Point_2> solution_c;
	vector<Point_2> fullSolution;
	int count = 0;
	for(int i = 0; i < numberH; i++){
		count =0;
		solution_c.clear();
		if(i > 0) solution_c.insert(solution_c.end(), solution[i-1].begin(), solution[i-1].end());
		if(i < numberH-1) solution_c.insert(solution_c.end(), solution[i+1].begin(), solution[i+1].end());
		assert(independency(solution_c,sigma,sigma));
		for(auto e: greedyLine[i]){
			if(e != Point_2(-1,-1)){
				solution_c.push_back(e);
				count++;
			}
		}
		// test independency
		if(!independency(solution_c,sigma,sigma)){
			std::cout<< "i: "<< i<< std::endl;
			printSolution(solution_c,sigma,sigma);
		}
		//assert(independency(solution_c,sigma,sigma));
		// test full
		if(full){
			fullSolution.clear();
			getGreeyFullLines(fullSolution,i-1,i+1);
			if(fullSolution.size() != solution_c.size()){
				std::cout<<"i: "<<i<<std::endl;
				std::cout<<"numberH: "<<numberH<<std::endl;
				std::cout<< "fullSolution.size(): "<<fullSolution.size() <<std::endl;
				printSolution(fullSolution,sigma,sigma);
				std::cout<<"solution_c: "<<solution_c.size()<<std::endl;
				printSolution(solution_c,sigma,sigma);
			}
			assert(fullSolution.size() == solution_c.size());
		}
	}
	//std::cout<< "in Problem_arrow_greedy::debugGreedy"<< std::endl;

};
*/
void Problem_arrow_greedy::forcedDeletion(unsigned int  index){};
void Problem_arrow_greedy::forcedAddition(unsigned int  index){};
void Problem_arrow_greedy::freeAddition(unsigned int  index){};
void Problem_arrow_greedy::freeDeletion(unsigned int  index){};


void Problem_arrow_greedy::getTheSolution(vector<Point_2>& tSolution){
	int c =(markE == true)?0:1;
	for(int i = 0; i < numberH; i++){
		if(i%2!=c)continue;
		for(auto e: solution[i]){
			tSolution.push_back(e);
		}
	}
};
void Problem_arrow_greedy::getGreedySolution(vector<Point_2>& tSolution){
	int c =(markE == true)?1:0;
	for(int i = 0; i < numberH; i++){
		if(i%2!=c)continue;
		for(int j = 0; j < numberV; j++){
			if(greedyLine[i][j] != Point_2(-1,-1)){
				tSolution.push_back(greedyLine[i][j] );
			}
		}
	}

};

void Problem_arrow_greedy::addPoint(Point_2 p){
	auto t0 = std::chrono::high_resolution_clock::now();
	//std:: cout<< "in addPoint"<<p.x()<<" "<<p.y()<< std::endl;
	Problem::addThePoint(p);
	std::set<Point_2>::iterator it;
	std::pair<std::set<Point_2>::iterator,bool> ret;
	int y=ceil((p.y()-h_sigma)/sigma);
	ret= lines[y].insert(p);
	it=ret.first;
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds d = std::chrono::duration_cast< std::chrono::nanoseconds >(fs);
	t0 = std::chrono::high_resolution_clock::now();
	unsigned int oldSize = solution[y].size();
	if(oldSize == 0){
		solution[y].insert(p);
		greedyCheck(p);
	}
	//std:: cout<< "point A"<< std::endl;
	auto next = solution[y].lower_bound(p);
	if(next != solution[y].begin()){
		auto prev = std::prev(next);
		if(p.x() < (*prev).x()+sigma){
			t1 = std::chrono::high_resolution_clock::now();
			fs = t1 - t0;
			d = std::chrono::duration_cast< std::chrono::nanoseconds >(fs);
			return;
		}
	}
	if(next == solution[y].end() || p.x() <= (*next).x()-sigma){
		solution[y].insert(p);
		greedyCheck(p);
	}
	else{
		solution[y].erase(next);
		solution[y].insert(p);
		greedyCheck(p);
		continueLine_Greedy(p);
	}
	//assert(solution[y].size() >= oldSize);
	if(solution[y].size() > oldSize){
		//assert(solution[y].size()  == oldSize+1);
		if((y%2)==0) sumE++;
		else sumO++;
		if(sumE > sumO){
			markE = true;
		}
		else markE = false;

	}
	t1 = std::chrono::high_resolution_clock::now();
	fs = t1 - t0;
	d = std::chrono::duration_cast< std::chrono::nanoseconds >(fs);
		//TODO: the solution need to updated

	//std:: cout<< "out addPoint"<< std::endl;
};
//TODO: to check the greedy parts around a new point in the solution
void Problem_arrow_greedy::greedyCheck(Point_2 p){
	int x=ceil((p.x()-h_sigma)/sigma);
	int y=ceil((p.y()-h_sigma)/sigma);
	// it has lower one
	if(y > 0){
		if(x > 0){
			if(greedyLine[y-1][x-1]!= Point_2(-1,-1)&&isConflict(greedyLine[y-1][x-1],p,sigma,sigma)){
				greedyLine[y-1][x-1]= Point_2(-1,-1);
				greedyLineSize[y-1]--;
			}
		}
		if(greedyLine[y-1][x]!= Point_2(-1,-1)&&isConflict(greedyLine[y-1][x],p,sigma,sigma)){
			greedyLine[y-1][x]= Point_2(-1,-1);
			greedyLineSize[y-1]--;
		}
		if(x < numberV-1){
			if(greedyLine[y-1][x+1]!= Point_2(-1,-1)&&isConflict(greedyLine[y-1][x+1],p,sigma,sigma)){
				greedyLine[y-1][x+1]= Point_2(-1,-1);
				greedyLineSize[y-1]--;
			}
		}
	}
	//it has above one
	if(y < numberH-1){
		if(x > 0){
			if(greedyLine[y+1][x-1]!= Point_2(-1,-1)&&isConflict(greedyLine[y+1][x-1],p,sigma,sigma)){
				greedyLine[y+1][x-1]= Point_2(-1,-1);
				greedyLineSize[y+1]--;
			}
		}
		if(greedyLine[y+1][x]!= Point_2(-1,-1)&&isConflict(greedyLine[y+1][x],p,sigma,sigma)){
			greedyLine[y+1][x]= Point_2(-1,-1);
			greedyLineSize[y+1]--;
		}
		if(x < numberV-1){
			if(greedyLine[y+1][x+1]!= Point_2(-1,-1)&&isConflict(greedyLine[y+1][x+1],p,sigma,sigma)){
				greedyLine[y+1][x+1]= Point_2(-1,-1);
				greedyLineSize[y+1]--;
			}
		}
	}
}
void Problem_arrow_greedy::inGreedyCheck(Point_2 p){
	int y=ceil((p.y()-h_sigma)/sigma);
	int x=ceil((p.x()-h_sigma)/sigma);
	if(greedyLine[y][x] == p){
		greedyLine[y][x] = Point_2(-1,-1);
		greedyLineSize[y]--;
	}

}
//TODO: not greedy
void Problem_arrow_greedy::subPoint(Point_2 p){
	auto t0 = std::chrono::high_resolution_clock::now();
	//std:: cout<< "in subPoint "<<p.x()<<" "<< p.y()<< std::endl;
	int y=ceil((p.y()-h_sigma)/sigma);
	Point_2 temp;
	unsigned int oldSize = solution[y].size();
	lines[y].erase(p);
	inGreedyCheck(p);
	//TODO: number just for debugger
	//assert(number == 1);
	// for solution
	arrowLine::iterator it;
	it = solution[y].find(p);
	// if p is in the solution
	if(it != solution[y].end()){
		if(it != solution[y].begin()){
			auto prev = std::prev(it);
			temp = *prev;
			solution[y].erase(p);
			continueLine_Greedy(temp);
		}
		//if p is the start
		else{
			solution[y].erase(p);
			continueLine_Greedy(Point_2(-DBL_MAX,p.y()));
		}
	}
	//assert(solution[y].size() <= oldSize);
	if(solution[y].size() < oldSize){
		//assert(solution[y].size()  == oldSize-1);
		if((y%2)==0) sumE--;
		else sumO--;
		if(sumE > sumO){
			markE = true;
		}
		else markE = false;
	}
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds d = std::chrono::duration_cast< std::chrono::nanoseconds >(fs);
	t0 = std::chrono::high_resolution_clock::now();
	Problem::subThePoint(p);
	//std:: cout<< "out subPoint"<< std::endl;
	t1 = std::chrono::high_resolution_clock::now();
	fs = t1 - t0;
	d = std::chrono::duration_cast< std::chrono::nanoseconds >(fs);
};
void Problem_arrow_greedy::subPoint_index(unsigned int index){
	Point_2 p = pVector[index];
	subPoint(p);
};
void  Problem_arrow_greedy::continueLine(Point_2 p){
	Point_2 start  = p+Vector(sigma,0);
	double grid_y=ceil((p.y()-h_sigma)/sigma);
	auto it = lines[grid_y].lower_bound(start);
	auto candidateT = solution[grid_y].lower_bound(start);
	while(it != lines[grid_y].end()){
		if(candidateT != solution[grid_y].end()&& *it == *candidateT){
			return;
		}
		start = (*it)+Vector(sigma,0);
		solution[grid_y].insert(*it);
		if(candidateT != solution[grid_y].end()){
			solution[grid_y].erase(candidateT);
		}
		it = lines[grid_y].lower_bound(start);
		candidateT = solution[grid_y].lower_bound(start);;
	}
}
void  Problem_arrow_greedy::continueLine_Greedy(Point_2 p){
	Point_2 start  = p+Vector(sigma,0);
	double grid_y=ceil((p.y()-h_sigma)/sigma);
	auto it = lines[grid_y].lower_bound(start);
	auto candidateT = solution[grid_y].lower_bound(start);
	while(it != lines[grid_y].end()){
		if(candidateT != solution[grid_y].end()&& *it == *candidateT){
			return;
		}
		start = (*it)+Vector(sigma,0);
		solution[grid_y].insert(*it);
		greedyCheck(*it);
		if(candidateT != solution[grid_y].end()){
			solution[grid_y].erase(candidateT);
		}
		it = lines[grid_y].lower_bound(start);
		candidateT = solution[grid_y].lower_bound(start);;
	}
}

void Problem_arrow_greedy::setArrow_recompute(int grid_y,vector<arrowLine>& solution,vector<set<Point_2>>& lines){
	Point_2 start = Point_2(-1,-1);
	solution[grid_y].clear();
	auto it = lines[grid_y].lower_bound(start);
	while(it != lines[grid_y].end()){
		solution[grid_y].insert(*it);
		start = (*it)+Vector(sigma,0);
		it = lines[grid_y].lower_bound(start);
	}
}
void Problem_arrow_greedy::getTheSolution_recompute(vector<Point_2>& tSolution,vector<arrowLine>& solution,double markE){
	int c =(markE == true)?0:1;
	for(int i = 0; i < numberH; i++){
		if(i%2!=c)continue;
		for(auto e: solution[i]){
			tSolution.push_back(e);
		}
	}

}
void Problem_arrow_greedy::getGreedySolution_recompute(vector<Point_2>& tSolution,vector<vector<Point_2>>& greedyLine,double markE){
	int c =(markE == true)?1:0;
	for(int i = 0; i < numberH; i++){
		if(i%2!=c)continue;
		for(int j = 0; j < numberV; j++){
			if(greedyLine[i][j] != Point_2(-1,-1)){
				tSolution.push_back(greedyLine[i][j] );
			}
		}
	}
}
void Problem_arrow_greedy::printSolution(){
	vector<Point_2> solution;
	solution.clear();
	getTheSolution(solution);
	getGreedySolution(solution);
	for(auto e: solution){
			std::cout<< lookUp(e)<<" ";
	}
	std::cout<<std::endl<<"solution size: "<< solution.size()<< std::endl;
	std::cout<<"################################################################################"<<std::endl;
};


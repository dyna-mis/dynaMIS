/*
 * problem_slk_greedy.cpp
 *
 *  Created on: Nov 13, 2019
 *      Author: guangping
 */
#include "problem_slk_greedy.h"
Problem_slk_greedy::Problem_slk_greedy(double w, const char* fileName,int k_i):Problem(w,fileName){
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
	//greedy solution
	greedyLine = vector<vector<Point_2>>(numberH,vector<Point_2>(numberV,Point_2(-1,-1))) ;
	greedyLineSize = vector<vector<int>> (numberH,vector<int>((k+1),0));

	//greedy between
	greedyKK = vector<vector<vector<vector<Point_2>>>>(numberH,vector<vector<vector<Point_2>>>(k+1,vector<vector<Point_2>>(k+1,vector<Point_2>(numberV,Point_2(-1,-1)))));
	greedyKKSize = vector<vector<vector<int>>>(numberH,vector<vector<int>>(k+1,vector<int>(k+1,0)));
};
void Problem_slk_greedy::setPoint(){
	int x, y;
	for(auto p: pVector){
		x=ceil((p.x()-h_sigma)/sigma);
		y=ceil((p.y()-h_sigma)/sigma);
		gridP[y][x].insert(p);
	}
};
void Problem_slk_greedy::simple(){
	setPoint();
	auto t0 = std::chrono::high_resolution_clock::now();
	//std::cout<< "in simple"<< std::endl;
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
	greedy();
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds d = std::chrono::duration_cast< std::chrono::nanoseconds >(fs);
	vector<Point_2> solution;
	solution.clear();
	getSolution_Greedy(solution);
	//std::cout<< "out simple"<< std::endl;
}
void Problem_slk_greedy::greedy(){
	greedy_Line();
	greedy_Between();
}
void Problem_slk_greedy::greedy_Line(){
	//std::cout<< "in greedy_LINE"<< std::endl;
	vector<Point_2> solution(numberV,Point_2(-1,-1));
	Point_2 prev;
	Point_2 back;
	int s;
	for(int i = 0; i < numberH; i++){
		//std::cout<< "i:"<< i<< std::endl;
		for(int j = 0;  j< k+1; j++){
			//std::cout<< "j:"<< j<< std::endl;
			solution=vector<Point_2>(numberV,Point_2(-1,-1));
			getOneGroup_Grid(solution,i,j);
			s= (j+k)%(k+1);
			for(int q = s; q < numberV;){
				//std::cout<< "q:"<< q<< std::endl;
				if(q >0 && solution[q-1] != Point_2(-1,-1)){
					prev = solution[q-1]+Vector(sigma,0);
				}
				else prev = Point_2(-1,-1);
				//std::cout<< "prev: "<< prev.x()<<" "<< prev.y()<< std::endl;
				if(q < numberV-1 && solution[q+1] != Point_2(-1,-1)){
					back = solution[q+1]+Vector(-sigma,0);
				}
				else back = Point_2(DBL_MAX,-1);
				//std::cout<< "back: "<< back.x()<<" "<< back.y()<< std::endl;
				//std::cout<< "q:-----------2 "<< q<< std::endl;
				auto it = gridP[i][q].lower_bound(prev);
				//std::cout<< "q:-----------2  * "<< q<< std::endl;
				//std::cout<< "print solution"<<std::endl;
				//printSolution(solution,sigma,sigma);
				//std::cout<<"prev:"<< prev<<std::endl;
				//std::cout<<"back:"<< back<<std::endl;
				//std::cout<<"*it:"<< *it<<std::endl;
				if(it != gridP[i][q].end()&& (*it).x()<= back.x()){
					//std::cout<< "q:-----------2  must"<< q<< std::endl;
					greedyLine[i][q] = *it;
					greedyLineSize[i][j]++;
					q+=k+1;
					continue;
				}
				//std::cout<< "q:-----------2  ** "<< q<< std::endl;
				q+=k+1;
			}
			//std::cout<< "out for"<<std::endl;
			//if(i> 0 && j==0) assert(false);
		}
	}
	//std::cout<< "out greedy            =1"<< std::endl;
	//printGreedyLine();
	//std::cout<< "out greedy_LINE"<< std::endl;
}
void Problem_slk_greedy::greedy_Between(){
	vector<Point_2> solution_below;
	vector<Point_2> solution_above;
	vector<Point_2> solution;
	double last= -1;
	bool extendible_upper;
	bool extendible_below;
	for(int i = 0; i < numberH; i++){
		for(int upper =0; upper < k+1; upper++){
			for(int below = 0; below < k+1; below ++){
				solution_below.clear();
				solution_above.clear();
				solution.clear();
				last = -1;
				// it has below
				if(i > 0){
					getOneGroup(solution_below,i-1,below);
					getGreedyOneGroup(solution_below,i-1,below);
				}
				//it has upper
				if(i < numberH-1){
					getOneGroup(solution_above,i+1,upper);
					getGreedyOneGroup(solution_above,i+1,upper);
				}
				solution.insert(solution.end(), solution_below.begin(), solution_below.end());
				solution.insert(solution.end(), solution_above.begin(), solution_above.end());
				//assert(independency(solution,sigma,sigma));
				// TODO: no bruteforce check (check points around)
				for(int j = 0; j < numberV; j++){
					for(auto e:gridP[i][j]){
						if(e.x() < last) continue;
						extendible_below = extendible(solution_below, e,sigma,sigma);
						extendible_upper = extendible(solution_above, e,sigma,sigma);
						if(extendible_below && extendible_upper){
							greedyKK[i][upper][below][j] = e;
							solution.push_back(e);
							greedyKKSize[i][upper][below]++;
							last = e.x()+sigma;
							break;
						}
					}
				}
				/*if(i==18 && upper == 0 && below == 0){
					printSolution(solution,sigma,sigma);
					assert(false);
				}*/
				//assert(independency(solution,sigma,sigma));
			}
		}
	}
}
/*void Problem_slk_greedy::getCombineSolution_GreedyBetween(vector<Point_2>& solution,int line_below,int group_below,int line_upper,int group_upper){
	int line = line_below+1;
	assert(line_below +2 == line_upper);
	int count = 0;
	getCombineSolution(solution,line_below,group_below,line_upper,group_upper);
	assert(independency(solution,sigma,sigma));
	for(auto e: greedyKK[line][group_upper][group_below]){
		if(e != Point_2(-1,-1)){
			solution.push_back(e);
			count++;
		}
		if(!independency(solution,sigma,sigma)){
			std::cout<< "i: "<<line<< std::endl;
			std::cout<< "upper: "<<group_upper<< std::endl;
			std::cout<< "below: "<<group_below<< std::endl;
			std::cout<< "The solution"<< std::endl;
			printSolution(solution,sigma,sigma);
		}
	}
	if(!independency(solution,sigma,sigma)){
		printGreedyBetween();
	}
	if(!independency(solution,sigma,sigma)){
		std::cout<<"count: "<<count  << std::endl;
		std::cout<<"greedyKKSize[line][group_upper][group_below]: "<<greedyKKSize[line][group_upper][group_below]  << std::endl;
		std::cout<<"line: "<<line  << std::endl;
		std::cout<<"group_upper: "<< group_upper << std::endl;
		std::cout<<"group_below: "<<group_below<< std::endl;
	}
	assert(independency(solution,sigma,sigma));
	if(count != greedyKKSize[line][group_upper][group_below]){
		std::cout<<"count: "<<count  << std::endl;
		std::cout<<"greedyKKSize[line][group_upper][group_below]: "<<greedyKKSize[line][group_upper][group_below]  << std::endl;
		std::cout<<"line: "<<line  << std::endl;
		std::cout<<"group_upper: "<< group_upper << std::endl;
		std::cout<<"group_below: "<<group_below<< std::endl;
		//std::cout<<": "<<  << std::endl;
	}
	assert(count == greedyKKSize[line][group_upper][group_below]);
}
void Problem_slk_greedy::getCombineSolution(vector<Point_2>& solution,int line_below,int group_below,int line_upper,int group_upper){
	assert(line_below +2 == line_upper);
	getOneGroup(solution,line_below,group_below);
	getGreedyOneGroup(solution,line_below,group_below);
	getOneGroup(solution,line_upper,group_upper);
	getGreedyOneGroup(solution,line_upper,group_upper);
}

void Problem_slk_greedy::debugGreedyBetween(bool full){
	vector<Point_2> solution;
	vector<Point_2> fullSolution;
	for(int i = 0; i < numberH; i++){
		for(int upper = 0; upper < k+1; upper++){
			for(int below = 0; below < k+1; below++){
				solution.clear();
				getCombineSolution_GreedyBetween(solution,i-1,below,i+1,upper);
				// test independendy
				if(!independency(solution,sigma,sigma)){
					std::cout<< "i: "<<i<< std::endl;
					std::cout<< "upper: "<<upper<< std::endl;
					std::cout<< "below: "<<below<< std::endl;
					std::cout<< "The solution"<< std::endl;
					printSolution(solution,sigma,sigma);
				}
				assert(independency(solution,sigma,sigma));
				if(full){
					//check full
					fullSolution.clear();
					getCombineSolution_GreedyBetween(fullSolution,i-1,below,i+1,upper);
					// get solution of one lines (fullSolution)
					assert(fullSolution.size() == solution.size());
				}
			}
		}
	}

}
void Problem_slk_greedy::debugGreedyLine(bool full){
	vector<Point_2> solution;
	vector<Point_2> fullSolution;
	// each line
	for(int i = 0; i< numberH; i++){
		// each group
		for(int j = 0; j < (k+1); j++){
			// test independendy
			solution.clear();
			getOneGroup(solution,i,j);
			getGreedyOneGroup(solution,i,j);
			assert(independency(solution,sigma,sigma));
			//test full already
			if(full){
				fullSolution.clear();
				getFullOneGroup(fullSolution,i,j);
				if(fullSolution.size() != solution.size()){
					std::cout<< "the line: "<<i<< std::endl;
					std::cout<< "group: "<<j<< std::endl;
					std::cout<< "full solution:"<< std::endl;
					printSolution(fullSolution,sigma,sigma);
					std::cout<< "the solution:"<< std::endl;
					printSolution(solution,sigma,sigma);
				}
				assert(fullSolution.size() == solution.size());
			}
		}
	}
}*/
// get the greedy one of one group (not include the exact ones)
void Problem_slk_greedy::getGreedyOneGroup_Grid(vector<Point_2>& solution,int line, int group){
	if(line < 0 || line >= numberH) return;
	int count = 0;
	int s= (group+k)%(k+1);
	for(int j = s; j < numberV; ){
		if(greedyLine[line][j] != Point_2(-1,-1)){
			solution[j]=greedyLine[line][j];
			count++;
		}
		j+= k+1;
	}
	//assert(count == greedyLineSize[line][group]);
}
// get the greedy one of one group (not include the exact ones)
void Problem_slk_greedy::getGreedyOneGroup(vector<Point_2>& solution,int line, int group){
	if(line < 0 || line >= numberH) return;
	int count = 0;
	int s= (group+k)%(k+1);
	for(int j = s; j < numberV; ){
		if(greedyLine[line][j] != Point_2(-1,-1)){
			solution.push_back(greedyLine[line][j]);
			count++;
		}
		j+= k+1;
	}
	//assert(count == greedyLineSize[line][group]);
}
/*
void Problem_slk_greedy::getFullOneGroup(vector<Point_2>& solution,int line, int group){
	if(line < 0 || line >= numberH) return;
	getOneGroup(solution,line,group);
	for(int i = 0; i < numberV; i++){
		for(auto e: gridP[line][i]){
			if(extendible(solution,e,sigma,sigma)) solution.push_back(e);
		}
	}
}
void Problem_slk_greedy::getFullCombine(vector<Point_2>& solution,int line_below, int group_below,int line_upper, int group_upper){
	getFullOneGroup(solution,line_below,group_below);
	int line = (line_upper+line_below)/2;
	getFullOneGroup(solution,line_upper,group_upper);
	for(int i = 0; i < numberV; i++){
		for(auto e: gridP[line][i]){
			if(extendible(solution,e,sigma,sigma)) solution.push_back(e);
		}
	}
}
*/
// get maximum independent set of k lines
void Problem_slk_greedy::getMIS_k(int grid_x, int grid_y){
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
void Problem_slk_greedy::getMIS_pre(int grid_x, int grid_y){
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
void Problem_slk_greedy::forcedDeletion(unsigned int  index){};
void Problem_slk_greedy::forcedAddition(unsigned int  index){};
void Problem_slk_greedy::freeAddition(unsigned int  index){};
void Problem_slk_greedy::freeDeletion(unsigned int  index){};
//+++++++++++++++++++++++++debug++++++++++++++++++++++++++++++++++
/*
void Problem_slk_greedy::fullChange(){
	std::cout<< "in fullChange_slk"<< std::endl;
	debugMIS();
	fullAddition();
	fullDeletion();
	std::cout<< "out fullChange_sl"<< std::endl;
};
void Problem_slk_greedy::fullDeletion(){
	std::cout<<std::endl<< "in fullDeletion_slk"<< std::endl;
    debug();
    std::set<Point_2> points;
    getRandomPoints(points,100,pVector);
    for(auto e: points){
    	subPoint(e);
        debug();
    }
	if(pVector.size() != luTable.size()){
		assert(false);
	}
    //Point_2 p(440.583,222.778);
    //Point_2 p(32.1174,348.887);
	//subPoint(p);
    debug();
	std::cout<< "out fullDeletion_slk"<< std::endl;

};
void Problem_slk_greedy::fullAddition(){
	std::cout<<std::endl<< "in fullAddition_slk"<< std::endl;
    debug();
    std::set<Point_2> points;
    getRandomPoints(points, width, height,100,h_sigma);
    for(auto e: points){
    	addPoint(e);
        debug();
    }
    Point_2 p(69.5491,179.981);
    addPoint(p);
    debug();
	std::cout<< "out fullAddition_slk"<< std::endl;
};
//-------------------------debug----------------------------------
//+++++++++++++++++++++++++debug++++++++++++++++++++++++++++++++++
void Problem_slk_greedy::printGreedyLine(){
	for(int i = 0; i < numberH; i++){
		vector<int> count((k+1),0);
		for(int j = 0; j < numberV; j++){
			if(greedyLine[i][j] != Point_2(-1,-1)){
				count[(j+1)%(k+1)]++;
				std::cout<<"("<<greedyLine[i][j].x()<<","<< greedyLine[i][j].y()<<") ";
			}
		}
		int j = 0;
		for(auto e:count){
			assert(e == greedyLineSize[i][j]);
			j++;
		}
		std::cout<< endl;
	}
}
// print and only debug if the count right
void Problem_slk_greedy::printGreedyBetween(){
	int count;
	for(int i = 0; i < numberH; i++){
		for(int upper = 0; upper < k+1; upper++){
			for(int below = 0; below < k+1; below++){
				std::cout<< "i: "<< i <<" upper:"<<upper<<"　below:"<< below<< " size:"<< greedyKKSize[i][upper][below]<< std::endl;
				count = 0;
				for(auto e: greedyKK[i][upper][below]){
					if(e != Point_2(-1,-1)){
						std::cout<<"("<<e.x()<<","<<e.y()<<") ";
						count++;
					}
				}
				assert(count == greedyKKSize[i][upper][below]);
				std::cout<<std::endl;
			}
		}
	}
}
void Problem_slk_greedy::printGrid(){
	std::cout<< "The grid:"<< std::endl;
	int i = 0;
	int j = 0;
	for(auto h: gridC){
		j = 0;
		for(auto p:h){
			std::cout<< p<<":   ";
			for (auto v : gridP[i][j]){
				std::cout <<"("<< v.x() <<" , "<< v.y() << ")  ";
			}
			for (auto v : gridS[i][j]){
				std::cout <<"["<< v.x() <<" , "<< v.y() << "]  ";
			}
			j++;
		}
		i++;
		std::cout<< std::endl;
	}
}
void Problem_slk_greedy::printPreGrid(){
	std::cout<< "The preGrid:"<< std::endl;
	int line = 0;
	int grid = 0;
	for(auto l: preGridS){
		grid = 0;
		for(auto g: l){
			for(auto e: g){
				std::cout <<"("<< e.x() <<" , "<< e.y() << ")  ";
			}
			std::cout<< "***"<<preGridC[line][grid]<< "***";
			grid++;

		}
		std::cout<< endl;
		line++;
	}
}
void Problem_slk_greedy::printParameter(){
	std::cout<< "numV:"<< numberV<< std::endl;
	std::cout<< "numH:"<< numberH<< std::endl;
	std::cout<< "sigma: "<< sigma<< std::endl;
	std::cout<< "counter: "<< std::endl;
}
void Problem_slk_greedy::printCounter(){
	for(auto q: counter){
		for(auto e: q){
		std::cout<< e;
		}
		std::cout<<std::endl;
	}
	std::cout<< std::endl;
}
void Problem_slk_greedy::printMarks(){
	std::cout<< "mark: ";
	for(auto e: mark){
		std::cout<<e <<"    ";
	}
	std::cout<< std::endl;
	std::cout<< "sumE:"<<sumE<< std::endl;
	std::cout<< "sumO:"<< sumO<< std::endl;
	std::cout<< "markE:"<< markE<< std::endl;
}
void Problem_slk_greedy::printTheSolution(){
	//print solution
	vector<Point_2> solution;
	solution.clear();
	getSolution(solution);
	std::cout<< "solution:"<< std::endl;
	printSolution(solution,sigma,sigma);
	std::cout<< "out Print"<< std::endl;
}
void Problem_slk_greedy::printGridPCS(){
	std::cout<< "The grid:"<< std::endl;
	int i = 0;
	int j = 0;
	for(auto h: gridC){
		j = 0;
		for(auto p:h){
			std::cout<< p<<":   ";
			for (auto v : gridP[i][j]){
				std::cout <<"("<< v.x() <<" , "<< v.y() << ")  ";
			}
			for (auto v : gridS[i][j]){
				std::cout <<"["<< v.x() <<" , "<< v.y() << "]  ";
			}
			j++;
		}
		i++;
		std::cout<< std::endl;
	}
};
void Problem_slk_greedy::print(){
	Problem::print();
	printGridPCS();
	printGrid();
	printPreGrid();
	printGreedyLine();
	printGreedyBetween();
	printParameter();
	printCounter();
	printMarks();
	printTheSolution();
};*/
void Problem_slk_greedy::getSolution_Greedy(vector<Point_2>& solution){
	//std::cout<< "in getSolution_Greedy"<<std::endl;
	//assert(independency(solution,sigma, sigma));
	int c =(markE == true)?0:1;
	for(int i = 0; i < numberH; i++){
		if(i%2!=c){
			getGreedyBetween(solution,i);
			continue;
		};
		getOneGroup(solution,i,mark[i]);
		getGreedyOneGroup(solution,i,mark[i]);
	}
	//assert(independency(solution,sigma, sigma));
	//std::cout<< "out getSolution_Greedy"<<std::endl;
}
// get the greedy solution on this line as some between line
void Problem_slk_greedy::getGreedyBetween(vector<Point_2>& solution,int line){
	//std::cout<< "in getGreedyBetween"<<line<<std::endl;
	//assert(independency(solution,sigma, sigma));
int below = 0;
int upper = 0;

	if(line > 0){
		below = mark[line-1];
	}
	if(line < numberH-1){
		upper = mark[line+1];
	}
	for(int i = 0; i< numberV; i++){
		if(greedyKK[line][upper][below][i] != Point_2(-1,-1)){
			solution.push_back(greedyKK[line][upper][below][i]);
		}
	}
	//assert(independency(solution,sigma, sigma));
};
void Problem_slk_greedy::getSolution(vector<Point_2>& solution){
	int c =(markE == true)?0:1;
	for(int i = 0; i < numberH; i++){
		if(i%2!=c)continue;
		getOneGroup(solution,i,mark[i]);
	}
}
/*
void Problem_slk_greedy::draw_grid(cairo_t *cr,double f_w, double f_h){
    cairo_set_source_rgb (cr, 0.0, 1.0, 1.0);
	// draw lines
	double x,y;
	double end_x= f_w*1.1;
	double	end_y  = f_h*1.1;
	for(int i =0; i < numberV; i++){
		cairo_stroke (cr);
		x = i*sigma+0.1*f_w;
		cairo_move_to(cr, x, 0.1*f_h);
		cairo_line_to(cr, x, end_y);
		cairo_stroke (cr);
	}
    cairo_set_source_rgb (cr, 1.0, 0.0, 0.0);
	for(int i =0; i < numberH; i++){
		cairo_stroke (cr);
		y = end_y-i*sigma;
		cairo_move_to(cr, 0.1*f_w,y);
		cairo_line_to(cr, end_x,y);
		cairo_stroke (cr);
	}
	vector<Point_2> solution;
	solution.clear();
	getSolution_Greedy(solution);

	for(auto e: solution){
		x = e.x()+0.1*width;
		y = 1.1*height-e.y();
		drawSqure(x, y,true,false,cr,sigma);
		drawPoint(x, y, cr);
	}
};
int Problem_slk_greedy::counterPoint(int j, int i){return -1;};
void Problem_slk_greedy::debugGridPCS(){
	//std::cout<< "The grid contains all points"<< std::endl;
	int i = 0;
	int j = 0;
	int cPoint = 0;
	int cSolution = 0;
	bool contain;
	for(auto h: gridC){
		j = 0;
		for(auto p:h){
			for (auto v : gridP[i][j]){
				contain = vectorContain(pVector,v);
				if(!contain){
					std::cout<< "The grid contains all points?"<< std::endl;
					std::cout <<"("<< v.x() <<" , "<< v.y() << ")  ";
					std::cout<< "The grid contains all points?&"<< std::endl;
					//print_pVector();
				}
				assert(contain);
				cPoint++;
			}
			cSolution= 0;
			for (auto v : gridS[i][j]){
				cSolution++;
				contain = vectorContain(pVector,v);
				if(!contain) std::cout <<"("<< v.x() <<" , "<< v.y() << ")  ";
				assert(contain);
			}
			assert(cSolution == gridC[i][j]);
			cSolution= 0;
			j++;
		}
		i++;
	}
	// check pVector.size == gridP.size
	assert(cPoint == size);
	assert(pVector.size() == size);

};
void Problem_slk_greedy::debugPreGridCS(){
	int line = 0;
	int grid = 0;
	bool contain;
	int cSolution= 0;
	for(auto l: preGridS){
		grid = 0;
		for(auto g: l){
			for(auto e: g){
				cSolution++;
				contain = vectorContain(pVector,e);
				if(!contain) std::cout <<"("<< e.x() <<" , "<< e.y() << ")  ";
				assert(contain);
			}

			assert(cSolution==preGridC[line][grid]);
			cSolution= 0;
			grid++;

		}
		line++;
	}
};
void Problem_slk_greedy::debugCounter(){
	int line = 0;
	int group = 0;
	vector<Point_2> solution;
	vector<Point_2> fullSolution;
	for(auto q: counter){
		group = 0;
		for(auto e: q){
			solution.clear();
			fullSolution.clear();
			//get solution
			getOneGroup(solution, line,group);
			// test number
			if(solution.size() != e){
				std::cout<< "line:"<< line<< std::endl;
				std::cout<< "e:"<< e<< std::endl;
				std::cout<< "group:"<< group<< std::endl;
				std::cout<< "solution:"<< std::endl;
				printSolution(solution,sigma,sigma);
			}
			assert(solution.size() == e);
			// test independendy
			assert(independency(solution,sigma, sigma));
			// test full
			getFullSolution(fullSolution, line,group);
			if(fullSolution.size() != solution.size()){
				std::cout<< "line:"<< line<< std::endl;
				std::cout<< "fullSolution.size():"<< fullSolution.size()<< std::endl;
				std::cout<< "Solution.size():"<< solution.size()<< std::endl;
				std::cout<< "group:"<< group<< std::endl;
				std::cout<< "solution:"<< std::endl;
				printSolution(solution,sigma,sigma);
				std::cout<< "fullsolution:"<< std::endl;
				printSolution(fullSolution,sigma,sigma);
			}
			assert(fullSolution.size() == solution.size());
			group ++;
		}
		line++;
	}
};
void Problem_slk_greedy::debugMarkSUM(){
	// test sumE;
	//test sumO
	int sE = 0;
	int sO = 0;
	for(int i = 0; i < numberH; i++){
		// 	test mark;
		for(auto m: counter[i]){
			if(m > counter[i][mark[i]]){
				std::cout<<"i: "<< i<< std::endl;
				std::cout<<"m: "<< m<< std::endl;
				std::cout<<"mark[i]: "<<mark[i]<< std::endl;
				std::cout<<"counter[i][mark[i]]: "<<counter[i][mark[i]]<< std::endl;
			}
			assert(m <= counter[i][mark[i]]);
		}
		// test sumE;
		//test sumO
		if(i%2== 0) sE+=counter[i][mark[i]];
		else sO+=counter[i][mark[i]];
	}
	if(sumE!= sE){
		print();
		std::cout<< "sE:"<<sE<< std::endl;
		std::cout<< "sumE:"<<sumE<< std::endl;
	}
	assert(sumE== sE);
	if(sumO!= sO){
		print();
		std::cout<< "sO:"<<sO<< std::endl;
		std::cout<< "sumO:"<<sumO<< std::endl;
	}
	assert(sumO== sO);
};
void Problem_slk_greedy::debugMarkE(){
	if(markE) assert(!(sumE < sumO));
	else assert(!(sumE > sumO));
};
void Problem_slk_greedy::debugTheSolution(){
	vector<Point_2> solution;
	solution.clear();
	getSolution(solution);
	assert(independency(solution,sigma,sigma));
}

void Problem_slk_greedy::debug(){
	//std::cout<< "in Problem_slk_greedy.debug()"<< std::endl;
	//Problem::debug();
	//debugGridPCS();
	//debugPreGridCS();
	//debugCounter();
	//debugMarkSUM();
	//debugMarkE();
	//debugTheSolution();
	//debugGreedyLine(false);
	//debugGreedyBetween(false);

	//TODO: get the greedy solution
	vector<Point_2> solution;
	solution.clear();
	getSolution_Greedy(solution);
	assert(independency(solution,sigma,sigma));

	//std::cout<< "out Problem_slk_greedy.debug()"<< std::endl;
};
*/
void Problem_slk_greedy::getOneGroup_Grid(vector<Point_2>& solution, int line,int group){
	if(line < 0 || line >= numberH) return;
	//assert(line < numberH);
	//assert(group < k+1);
	int grid;
	for(int i = 0; i< numberV; i++){
		if(i %(k+1)== group){
			for(auto e: gridS[line][i]){
				grid = ceil((e.x()-h_sigma)/sigma);
				solution[grid]= e;
			}
		}
	}
	if(group-2 > -1){
		for(auto e: preGridS[line][group-2]){
			grid = ceil((e.x()-h_sigma)/sigma);
			solution[grid]= e;
		}
	}
}
// get exact solution from one group
void Problem_slk_greedy::getOneGroup(vector<Point_2>& solution, int line,int group){
	if(line < 0 || line >= numberH) return;
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
}
void Problem_slk_greedy::getFullSolution(vector<Point_2>& solution, int line,int group){
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
void Problem_slk_greedy::addPoint(Point_2 p){
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
		if(x_index >= 0) {
			addPoint_recompute(x_index,y);
			addPoint_updateGreedyLine(x_index,y);
			addPoint_updateGreedyBetween(x_index,y);
		}

	}
	//update preGrid
	if(x < k-1){
		for(int i = x; i < k-1; i++){
			addPoint_recompute_pre(i,y);
			addPoint_updateGreedyLine_pre(i,y);
			addPoint_updateGreedyBetween_pre(i,y);
			//TODO:update the geedyline
			//TODO:update the greedybetween
		}
	}
	t1 = std::chrono::high_resolution_clock::now();
	fs = t1 - t0;
	d = std::chrono::duration_cast< std::chrono::nanoseconds >(fs);
	//std:: cout<< "out addPoint"<< std::endl;
};
//update the greedy line because the group starting with x is changed
void Problem_slk_greedy::addPoint_updateGreedyLine(int x,int y){
	//std:: cout<< "in addPoint_updateGreedyLine"<< std::endl;
	int group = x%(k+1);
	if(gridC[y][x] > 0){
		Point_2 start = gridS[y][x].front();
		Point_2 end = gridS[y][x].back();
		if(x > 0){
			if(greedyLine[y][x-1] != Point_2(-1,-1)&&greedyLine[y][x-1].x()+sigma > start.x()){
				greedyLine[y][x-1] = Point_2(-1,-1);
				greedyLineSize[y][group]--;
				//assert(greedyLineSize[y][group] >= 0);
			}
		}
		if(x <numberV-k){
			if(greedyLine[y][x+k] != Point_2(-1,-1)&&greedyLine[y][x+k].x() > end.x()-sigma){
				greedyLine[y][x+k] = Point_2(-1,-1);
				greedyLineSize[y][group]--;
				//assert(greedyLineSize[y][group] >= 0);
			}
		}
	}
	//std:: cout<< "out addPoint_updateGreedyLine"<< std::endl;
}

void Problem_slk_greedy::addPoint_updateGreedyBetween(int x,int y){
	//std:: cout<< "in addPoint_updateGreedyBetween"<< std::endl;
	int group = x%(k+1);
	// between y and y-2
	if(y > 0){
		for(int i = 0; i < k+1; i++){
			// update greedyBetween[y-1][group][i]
			for(int j = x-1; j < x+k+1; j++){
				if(j < 0) continue;
				if(j> numberV-1) break;
				// if now
				if(greedyKK[y-1][group][i][j] != Point_2(-1,-1)){
					// TODO: write extendible function for a sorted solution
					if(!extendible(gridS[y][x],greedyKK[y-1][group][i][j],sigma,sigma)){
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
		for(int i = 0; i < k+1; i++){
			//update greedyBetween[y+1][i][group]
			for(int j = x-1; j < x+k+1; j++){
				if(j < 0) continue;
				if(j> numberV-1) break;
				// if now
				if(greedyKK[y+1][i][group][j] != Point_2(-1,-1)){
					// TODO: write extendible function for a sorted solution
					if(!extendible(gridS[y][x],greedyKK[y+1][i][group][j],sigma,sigma)){
						greedyKK[y+1][i][group][j]= Point_2(-1,-1);
						greedyKKSize[y+1][i][group]--;
						//assert(greedyKKSize[y+1][i][group]>= 0);
					}
				}
			}
		}
	}
	//std:: cout<< "out addPoint_updateGreedyBetween"<< std::endl;
}
// for gridS, gridC
void Problem_slk_greedy::addPoint_recompute(int x, int y){
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
void Problem_slk_greedy::addPoint_recompute_pre(int x, int y){
	//assert(x < k-1);
	int& sum = ((y%2)==0)? sumE:sumO;
	int oldCount = preGridC[y][x];
	int index = (x+2)%(k+1);
	int oldMAX = counter[y][mark[y]];
	getMIS_pre(x, y);
	int newCount = preGridC[y][x];
	//std::cout<< "newCount:"<< newCount<< std::endl;
	//std::cout<< "oldCount:"<< oldCount<< std::endl;
	if(newCount > oldCount){
		//assert(newCount == oldCount+1);
		counter[y][index]++;
		//std::cout<< "y:"<< y<< std::endl;
		//std::cout<< "index:"<< index<< std::endl;
		if(counter[y][index] > oldMAX){
			//std::cout<< "change sum:"<< y%2<< std::endl;
			mark[y]= index;
			sum++;
			//std::cout<< "sum:"<< sum<< std::endl;
			if(sumE > sumO){
				markE = true;
			}
			else markE = false;
		}
	}
	else return;

}
//update the greedy line because the group starting with x is changed
void Problem_slk_greedy::addPoint_updateGreedyLine_pre(int x,int y){
	//std:: cout<< "in addPoint_updateGreedyLine_pre"<< std::endl;
	int group = (x+2)%(k+1);
	//assert(x>= 0);
	//assert(x < k-1);
	//assert(y < numberH);
	if(preGridC[y][x] > 0){
	  /*std:: cout<< "outer if"<< std::endl;
		std:: cout<< "preGridC[y][x]: "<< preGridC[y][x] << std::endl;
		std:: cout<< "y:"<< y<< std::endl;
		std:: cout<< "x: "<< x<< std::endl;*/
		//printSolution(preGridS[y][x],sigma,sigma);
		Point_2 start = preGridS[y][x].back();
		// TODO: in the preGridS start.x maximal!
		/*for(auto e: preGridS[y][x]){
			assert(e.x() <= start.x());
		}*/
		//std:: cout<< "get front"<< std::endl;
		if(greedyLine[y][x+1] != Point_2(-1,-1)&&greedyLine[y][x+1].x()< sigma + start.x()){
			//std:: cout<< "inner if"<< std::endl;
			greedyLine[y][x+1] = Point_2(-1,-1);
			greedyLineSize[y][group]--;
			//assert(greedyLineSize[y][group] >= 0);
		}
		//std:: cout<< "inner if done"<< std::endl;
	}
	//std:: cout<< "out addPoint_updateGreedyLine_pre"<< std::endl;
}

void Problem_slk_greedy::addPoint_updateGreedyBetween_pre(int x,int y){
	//std:: cout<< "in Problem_slk_greedy::addPoint_updateGreedyBetween_pre"<< std::endl;
	int group = (x+2)%(k+1);
	// between y and y-2
	if(y > 0){
		for(int i = 0; i < k+1; i++){
			// update greedyBetween[y-1][group][i]
			for(int j = 0; j <= x+1; j++){
				// if now
				if(greedyKK[y-1][group][i][j] != Point_2(-1,-1)){
					// TODO: write extendible function for a sorted solution
					if(!extendible(preGridS[y][x],greedyKK[y-1][group][i][j],sigma,sigma)){
						greedyKK[y-1][group][i][j]= Point_2(-1,-1);
						greedyKKSize[y-1][group][i]--;
					}
				}
			}
		}
	}
	//between y and y+2
	if(y < numberH-1){
		for(int i = 0; i < k+1; i++){
			//update greedyBetween[y+1][i][group]
			for(int j = 0; j <= x+1; j++){
				// if now
				if(greedyKK[y+1][i][group][j] != Point_2(-1,-1)){
					// TODO: write extendible function for a sorted solution
					if(!extendible(preGridS[y][x],greedyKK[y+1][i][group][j],sigma,sigma)){
						greedyKK[y+1][i][group][j]= Point_2(-1,-1);
						greedyKKSize[y+1][i][group]--;
					}
				}
			}
			/*if(y == 6 && group ==4 &&  i == 0){
				assert(false);
			}*/
		}
	}
	//std:: cout<< "out Problem_slk_greedy::addPoint_updateGreedyBetween_pre"<< std::endl;
}
void Problem_slk_greedy::subPoint(Point_2 p){
	auto t0 = std::chrono::high_resolution_clock::now();
	//std:: cout<< "in slk_subPoint"<< std::endl;
	int x=ceil((p.x()-h_sigma)/sigma);
	int y=ceil((p.y()-h_sigma)/sigma);
	//std:: cout<< "delete point:"<< p.x() << ","<< p.y()<< std::endl;
	//std:: cout<< "delete point_grid:"<< x << ","<< y<< std::endl;
	int x_index;
	//assert(gridC[y][x] > 0);
	gridP[y][x].erase(p);
	// update grid
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
	subGreedy(x,y,p);
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds d = std::chrono::duration_cast< std::chrono::nanoseconds >(fs);
	t0 = std::chrono::high_resolution_clock::now();
	Problem::subThePoint(p);
	t1 = std::chrono::high_resolution_clock::now();
	fs = t1 - t0;
	d = std::chrono::duration_cast< std::chrono::nanoseconds >(fs);
	//std:: cout<< "out slk_subPoint"<< std::endl;
};
void Problem_slk_greedy::subPoint_index(unsigned int index){
	Point_2 p = pVector[index];
	subPoint(p);
};
void Problem_slk_greedy::subGreedy(int x, int y, Point_2& p){
	if(greedyLine[y][x]== p){
		greedyLine[y][x]= Point_2(-1,-1);
		greedyLineSize[y][(x+1)%(k+1)]--;
		//assert(greedyLineSize[y][(x+1)%(k+1)] >= 0);
	}
	//greedyKK
	for(int upper = 0; upper < k+1; upper++){
		for(int lower = 0; lower < k+1; lower++){
			if(greedyKK[y][upper][lower][x] == p){
				greedyKK[y][upper][lower][x]= Point_2(-1,-1);
				greedyKKSize[y][upper][lower]--;
				//assert(greedyKKSize[y][upper][lower] >= 0);
			}
		}

	}
}
// for gridS, gridC
// recompute group strating with grid_x and grid_y
void Problem_slk_greedy::subPoint_recompute(int grid_x, int grid_y){
	int oldCount = gridC[grid_y][grid_x];
	int& sum = (grid_y%2==0)? sumE:sumO;
	getMIS_k(grid_x, grid_y);
	int index;
	int max_oldValue = -1;
	int max_value = -1;
	int index_candidate= -1;
	int max_index = -1;
	int newCount = gridC[grid_y][grid_x];
	if(newCount < oldCount){
		//assert(newCount == oldCount-1);
		index = grid_x%(k+1);
		counter[grid_y][index]--;
		if(mark[grid_y]== index){
			max_oldValue = counter[grid_y][index]+1;
			for(auto e:counter[grid_y]){
				index_candidate++;
				if(e > max_value) {
					max_index = index_candidate;
					max_value = e;
				}
			}
			mark[grid_y] =max_index;
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
	checkGreedy(grid_x, grid_y);
	//TODO: because the group is recomputed, then all greedy parts(above and below) (2*k*k) needs to be checked
}
// the subragion starts with the node (grid_x, grid_y) is changed
// the greedy parts need be checked
void Problem_slk_greedy::checkGreedy(int grid_x, int grid_y){
	checkGreedy_Line(grid_x,grid_y);
	checkGreedy_Between(grid_x,grid_y);
}
void  Problem_slk_greedy::checkGreedy_Line(int grid_x, int grid_y){
	addPoint_updateGreedyLine(grid_x,grid_y);
};
void  Problem_slk_greedy::checkGreedy_Between(int grid_x, int grid_y){
	addPoint_updateGreedyBetween(grid_x,grid_y);
};
// for preGridS, preGridC
// recompute pre group strating with grid_x and grid_y(reversed to zero)
void Problem_slk_greedy::subPoint_recompute_pre(int grid_x, int grid_y){
	//assert(grid_x < k-1);
	int index_candidate= -1;
	int max_value = -1;
	int max_index = -1;
	int max_oldValue = -1;
	int& sum = ((grid_y%2)==0)? sumE:sumO;
	int oldCount = preGridC[grid_y][grid_x];
	int index = (grid_x+2)%(k+1);
	getMIS_pre(grid_x, grid_y);
	int newCount = preGridC[grid_y][grid_x];
	//std::cout<< "newCount:"<< newCount<< std::endl;
	//std::cout<< "oldCount:"<< oldCount<< std::endl;
	if(newCount < oldCount){
		//assert(newCount == oldCount-1);
		counter[grid_y][index]--;
		//std::cout<< "y:"<< y<< std::endl;
		//std::cout<< "index:"<< index<< std::endl;
		if(mark[grid_y]== index){
			max_oldValue = counter[grid_y][index]+1;
			//std::cout<< "change sum:"<< y%2<< std::endl;
			for(auto e:counter[grid_y]){
				index_candidate++;
				if(e > max_value) {
					max_index = index_candidate;
					max_value = e;
				}
			}
			mark[grid_y] =max_index;
			//assert(max_oldValue >= max_value);
			sum -= max_oldValue;
			sum+= max_value;
			if(sumE > sumO){
				markE = true;
			}
			else markE = false;
		}
	}
	//TODO: because the group is recomputed, then all greedy parts(above and below) (2*k*k) needs to be checked
	checkGreedy_pre(grid_x, grid_y);
}
void Problem_slk_greedy::checkGreedy_pre(int grid_x, int grid_y){
	checkGreedy_Line_pre(grid_x, grid_y);
	checkGreedy_Between_pre(grid_x, grid_y);

};
void Problem_slk_greedy::checkGreedy_Line_pre(int grid_x, int grid_y){
	addPoint_updateGreedyLine_pre(grid_x,grid_y);
};
void Problem_slk_greedy::checkGreedy_Between_pre(int grid_x, int grid_y){
	addPoint_updateGreedyBetween_pre(grid_x,grid_y);
};

void Problem_slk_greedy::printSolution(){
	vector<Point_2> solution;
	solution.clear();
	getSolution_Greedy(solution);
	std::cout<<"################################################################################"<<std::endl;
	for(auto e: solution){
		std::cout<< lookUp(e)<<" ";
	}
	std::cout<<std::endl<<"solution size: "<< solution.size()<< std::endl;
	std::cout<<"################################################################################"<<std::endl;
};

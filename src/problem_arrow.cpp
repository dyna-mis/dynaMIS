/*
 * problem_arrow.cpp
 *
 *  Created on: Nov 17, 2019
 *      Author: ping
 */
#include "problem_arrow.h"
Problem_arrow::Problem_arrow(double w, const char* fileName):Problem(w,fileName){
	numberH=ceil(height/sigma);
	lines =vector<set<Point_2>>(numberH,set<Point_2>{});
	solution =vector<arrowLine>(numberH,arrowLine());
	sumE= 0;
	sumO = 0;
	markE = false;
};
void Problem_arrow::setPoint(){
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
void Problem_arrow::setArrow(int grid_y){
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
/*void Problem_arrow::debugArrow(int grid_y, set<Point_2>& MIS ){
	Point_2 start = Point_2(-1,-1);
	MIS.clear();
	auto it = lines[grid_y].lower_bound(start);
	while(it != lines[grid_y].end()){
		MIS.insert(*it);
		start = (*it)+Vector(sigma,0);
		it = lines[grid_y].lower_bound(start);
	}
}*/
void Problem_arrow::simple(){
	auto t0 = std::chrono::high_resolution_clock::now();
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
};
void Problem_arrow::forcedDeletion(unsigned int  index){};
void Problem_arrow::forcedAddition(unsigned int  index){};
void Problem_arrow::freeAddition(unsigned int  index){};
void Problem_arrow::freeDeletion(unsigned int  index){};
//+++++++++++++++++++++++++debug++++++++++++++++++++++++++++++++++
/*void Problem_arrow::fullChange(){
	std::cout<< "in fullChange_arrow"<< std::endl;
	//debugMIS();
	fullAddition();
	fullDeletion();
	std::cout<< "out fullChange_arrow"<< std::endl;
};
void Problem_arrow::fullAddition(){
	std::cout<<std::endl<< "in fullAddition_arrow"<< std::endl;
    //debug();
    std::set<Point_2> points;
    getRandomPoints(points, width, height,400,h_sigma);
    for(auto e: points){
    	if(lookUp(e) != -1) continue;
    	addPoint(e);
       // debug();
    }
    Point_2 p(15.0135,19.8844);
    addPoint(p);
    //debug();
	std::cout<< "out fullAddition_arrow"<< std::endl;
};
void Problem_arrow::fullDeletion(){
	std::cout<<std::endl<< "in fullDeletion_arrow"<< std::endl;
    //debug();
    std::set<Point_2> points;
    getRandomPoints(points,400,pVector);
    for(auto e: points){
    	assert(lookUp(e) != -1);
    	std::cout<<"sub the point  "<< e.x() <<" "<< e.y()<< std::endl;
    	subPoint(e);
        //debug();
    }
    //debug();
	std::cout<< "out fullDeletion_arrow"<< std::endl;

};
//-------------------------debug----------------------------------
//+++++++++++++++++++++++++debug++++++++++++++++++++++++++++++++++
void Problem_arrow::printSL(){
	std:: cout<< "the lines"<< std::endl;
	int i = 0;
	int c =(markE == true)?0:1;
	string color;
	bool contain;
	for(auto h: lines){
		if(i%2!=c) color = YELLOW;
		else color = GREEN;
		for(auto p:h){
			contain = (solution[i].find(p) != solution[i].end());
			if(contain){
			std::cout << color <<"("<< p<<") "<< RESET ;
			}
			else std::cout << RED <<"("<< p<<") "<<  RESET ;
		}
		std::cout<< std::endl;
		i++;
	}
};
void Problem_arrow::printParameter(){
	std::cout<< "numH:"<< numberH<< std::endl;
	std::cout<< "sigma: "<< sigma<< std::endl;
};
void Problem_arrow::printSum(){
	std::cout<< "sumE:"<<sumE<< std::endl;
	std::cout<< "sumO:"<< sumO<< std::endl;
};
void Problem_arrow::printMarkE(){
	std::cout<< "markE:"<< markE<< std::endl;
};
void Problem_arrow::print(){
	printSL();
	printParameter();
	printSum();
	printMarkE();
};
void Problem_arrow::draw_Line(cairo_t *cr,double f_w, double f_h){
    cairo_set_source_rgb (cr, 0.0, 1.0, 1.0);
	// draw lines
	double x,y;
	double end_x= f_w*1.1;
	double	end_y  = f_h*1.1;
    cairo_set_source_rgb (cr, 1.0, 0.0, 0.0);
	for(int i =0; i < numberH; i++){
		cairo_stroke (cr);
		y = end_y-i*sigma;
		cairo_move_to(cr, 0.1*f_w,y);
		cairo_line_to(cr, end_x,y);
		cairo_stroke (cr);
	}
	int c =(markE == true)?0:1;
	Point_2 e;
	// draw solution
	for(int i = 0; i < numberH; i++){
		if(i%2!=c)continue;
		// draw arrow
		for(auto e: solution[i]){
				x = e.x()+0.1*f_w;
				y = 1.1*f_h-e.y();
				drawSqure(x, y,true,false,cr,sigma);
				drawPoint(x, y, cr);
		}
	}
};
void Problem_arrow::debugLines(){
	// == ones in pVector
	int count = pVector.size();
	int i = 0;
	int y;
	for(auto l: lines){
		for(auto e: l){
			vectorContain(pVector,e);
			// each line contains the right ones
			y=ceil((e.y()-h_sigma)/sigma);
			assert(y == i);
			count--;
		}
		i++;
	}
	assert(count == 0);
};
void Problem_arrow::debugSolutions(){
	// should in the line
	for(int i= 0; i < numberH; i++){
		assert(independency(solution[i],sigma,sigma));
		for(auto s: solution[i]){
			assert(lines[i].find(s) != lines[i].end());
		}
	}
	// every line real MIS
	set<Point_2> MIS;
	int i = 0;
	for(auto line: lines){
		debugArrow(i,MIS);
		assert(independency(solution[i],sigma,sigma));
		assert(independency(MIS,sigma,sigma));
		assert(MIS.size() == solution[i].size());
	}
};
void Problem_arrow::debugSum(){
	int sE = 0;
	int sO = 0;
	for(int i= 0; i < numberH; i+=2){
		sE+= solution[i].size();
	}
	for(int i= 1; i < numberH; i+=2){
		sO+= solution[i].size();
	}
	assert(sE == sumE);
	if(sO != sumO){
		for(int i= 1; i < numberH; i+=2){
			std::cout<< "i: " << i <<" "<<solution[i].size()<< std::endl;
		}
		std::cout<< "sO: " << sO<< std::endl;
		std::cout<< "sumO: " << sumO<< std::endl;
	}
	assert(sO == sumO);
};
void Problem_arrow::debugMarkE(){
	if(sumE > sumO){
		assert(markE == true);
	}
	else{
		if(sumE == sumO) return;
		assert(markE == false);
	}
};
void Problem_arrow::debug(){
	Problem::debug();
	debugLines();
	debugSolutions();
	debugSum();
	debugMarkE();
};
*/
//-------------------------debug----------------------------------
void Problem_arrow::addPoint(Point_2 p){
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
	if(oldSize == 0) {
		solution[y].insert(p);
	}
	//std:: cout<< "point A"<< std::endl;
	auto next = solution[y].lower_bound(p);
	if(next != solution[y].begin()){
		auto prev = std::prev(next);
		if(p.x() < (*prev).x()+sigma){
			t1 = std::chrono::high_resolution_clock::now();
			std::chrono::duration< double>  fss = t1 - t0;
			std::chrono::nanoseconds  dd = std::chrono::duration_cast< std::chrono::nanoseconds >(fs);
			return;
		}
	}
	if(next == solution[y].end() || p.x() <= (*next).x()-sigma){
		solution[y].insert(p);
	}
	else{
		solution[y].erase(next);
		solution[y].insert(p);
		continueLine(p);
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
	std::chrono::duration< double>  fss = t1 - t0;
	std::chrono::nanoseconds  dd = std::chrono::duration_cast< std::chrono::nanoseconds >(fs);
	//std:: cout<< "out addPoint"<< std::endl;
};

void Problem_arrow::subPoint(Point_2 p){
	int y=ceil((p.y()-h_sigma)/sigma);
	Point_2 temp;
	unsigned int oldSize = solution[y].size();
	lines[y].erase(p);
	arrowLine::iterator it;
	it = solution[y].find(p);


	// if p is in the solution
	if(it != solution[y].end()){
		if(it != solution[y].begin()){
			auto prev = std::prev(it);
			temp = *prev;
			solution[y].erase(p);
			continueLine(temp);
		}
		//if p is the start
		else{
			solution[y].erase(p);
			continueLine(Point_2(-DBL_MAX,p.y()));
		}
	}
	if(solution[y].size() < oldSize){
		if((y%2)==0) sumE--;
		else sumO--;
		if(sumE > sumO){
			markE = true;
		}
		else markE = false;

	}
	Problem::subThePoint(p);
};
void Problem_arrow::subPoint_index(unsigned int index){
	Point_2 p = pVector[index];
	subPoint(p);
};

void  Problem_arrow::continueLine(Point_2 p){
	//std::cout<< "in continueLine "<< p.x()<< " "<< p.y()<< std::endl;
	Point_2 start  = p+Vector(sigma,0);
	double grid_y=ceil((p.y()-h_sigma)/sigma);
	auto it = lines[grid_y].lower_bound(start);
	auto candidateT = solution[grid_y].lower_bound(start);
	//std::cout<< (it == lines[grid_y].end())<< std::endl;
	//std::cout<< (candidateT == solution[grid_y].end())<< std::endl;
	while(it != lines[grid_y].end()){
		if(candidateT != solution[grid_y].end()&& *it == *candidateT){
			return;
		}
		start = (*it)+Vector(sigma,0);
		solution[grid_y].insert(*it);
		//std::cout<<"it"<< (*it).x()<<" "<<(*it).y()<< std::endl;
		if(candidateT != solution[grid_y].end()){
			//std::cout<<"candidateT"<< (*candidateT).x()<<" "<<(*candidateT).y()<< std::endl;
			solution[grid_y].erase(candidateT);
		}
		// if the candidate is overlaaping with it
		// next round
		it = lines[grid_y].lower_bound(start);
		candidateT = solution[grid_y].lower_bound(start);;
	}
	//std::cout<< "out continueLine "<< p.x()<< " "<< p.y()<< std::endl;
}

void Problem_arrow::printSolution(){
	int i;
	if(markE) i = 0;
	else i = 1;
	std::cout<<"################################################################################"<<std::endl;
	for(; i < numberH; i+=2){
		for(auto e: solution[i]){
			std::cout<< lookUp(e)<<" ";
		}
	}
	if(markE) std::cout<<std::endl<<"solution size: "<< sumE<< std::endl;
	else std::cout<<std::endl<<"solution size: "<< sumO<< std::endl;
	std::cout<<"################################################################################"<<std::endl;
};


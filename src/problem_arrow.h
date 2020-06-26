/*
 * problem_arrow.h
 *
 *  Created on: Nov 17, 2019
 *      Author: ping
 */

#ifndef SRC_PROBLEM_ARROW_H_
#define SRC_PROBLEM_ARROW_H_
#include <tuple>
#include "problem.h"
typedef std::set<Point_2> arrowLine;
class Problem_arrow:public Problem{
private:
	vector<set<Point_2>> lines;
	vector<arrowLine> solution;
	int numberH;
	int sumE;
	int sumO;
	bool markE;
public:
	Problem_arrow(double sigma,const char* fileName);
	void setPoint();
	void setArrow(int grid_y);
	void simple();
	void forcedDeletion(unsigned int  index);
	void forcedAddition(unsigned int  index);
	void freeAddition(unsigned int  index);
	void freeDeletion(unsigned int  index);
	//+++++++++++++++++++++++++debug++++++++++++++++++++++++++++++++++
	//void fullChange() ;
	//void fullDeletion();
	//void fullAddition();
	//-------------------------debug----------------------------------
	//+++++++++++++++++++++++++debug++++++++++++++++++++++++++++++++++
	// print
	/*void print();
	void printSL();
	void printParameter();
	void printSum();
	void printMarkE();
	void debugArrow(int grid_y, set<Point_2>& MIS);

	// debug
	void debug();
	void debugLines();
	void debugSolutions();
	void debugSum();
	void debugMarkE();


	void draw_Line(cairo_t *cr,double f_w, double f_h);*/
	//-------------------------debug----------------------------------
	void addPoint(Point_2 p);
	void subPoint(Point_2 p);
	void subPoint_index(unsigned int index);
	void printSolution();
	void continueLine(Point_2 start);
};




#endif /* SRC_PROBLEM_ARROW_H_ */

/*
 * problem_arrow_greedy.h
 *
 *  Created on: Nov 23, 2019
 *      Author: ping
 */

#ifndef SRC_PROBLEM_ARROW_GREEDY_H_
#define SRC_PROBLEM_ARROW_GREEDY_H_
#include <tuple>
#include "problem.h"
typedef std::set<Point_2> arrowLine;
class Problem_arrow_greedy:public Problem{
private:
	vector<set<Point_2>> lines;
	vector<arrowLine> solution;
	int numberH;
	int numberV;
	int sumE;
	int sumO;
	bool markE;
	// greedy solution
	vector<vector<Point_2>> greedyLine;
	vector<int> greedyLineSize;

public:
	Problem_arrow_greedy(double sigma,const char* fileName);
	void setPoint();
	void setArrow(int grid_y);
	void debugArrow(int grid_y, set<Point_2>& MIS);
	void simple();
	void greedy();
	void forcedDeletion(unsigned int  index);
	void forcedAddition(unsigned int  index);
	void freeAddition(unsigned int  index);
	void freeDeletion(unsigned int  index);

	void addPoint(Point_2 p);
	void greedyCheck(Point_2 p);
	void inGreedyCheck(Point_2 p);
	void subPoint(Point_2 p);
	void subPoint_index(unsigned int index);
	void continueLine(Point_2 start);
	void continueLine_Greedy(Point_2 start);

	//recompute
	void setArrow_recompute(int grid_y,vector<arrowLine>& solution,vector<set<Point_2>>& lines);
	void getTheSolution_recompute(vector<Point_2>& tSolution,vector<arrowLine>& solution,double markE);
	void getGreedySolution_recompute(vector<Point_2>& tSolution,vector<vector<Point_2>>& greedyLine,double markE);

	void getTheSolution(vector<Point_2>& solution);
	void getGreedySolution(vector<Point_2>& solution);
	void printSolution();
protected:
	  ~Problem_arrow_greedy(void) {}
};
#endif /* SRC_PROBLEM_ARROW_GREEDY_H_ */

/*
 * problem_slk_greedy.h
 * k lines
 *  Created on: Nov 21, 2019
 *      Author: guangping
 */

#ifndef SRC_PROBLEM_SLK_GREEDY_H_
#define SRC_PROBLEM_SLK_GREEDY_H_
#include "problem.h"
class Problem_slk_greedy:public Problem{
	private:
	// on each grid there is a binary tree(ordered by x-axis)
	vector<vector<set<Point_2>>> gridP;
	// on each grid there is a size of　MIS for k horizontal lines, starting this grid.
	vector<vector<int>> gridC;
	// solution of k lines
	vector<vector<vector<Point_2>>> gridS;
	vector<vector<int>> preGridC;
	vector<vector<vector<Point_2>>> preGridS;
	// number of vertical lines
	int numberV;
	//number of horizontal lines
	int numberH;

	//for each line, check k versions of sl (x-line/mode k = index)
	vector<vector<int>> counter;
	// each line, which is counted
	vector<int> mark;
	// sum of taking all even lines
	int sumE;
	//sum of taking all odd lines
	int sumO;
	// if choosing even lines
	bool markE;
	// group parameter
	int k;


	//greedy solution
	vector<vector<Point_2>> greedyLine;
	vector<vector<int>> greedyLineSize;
	//greedy between
	vector<vector<vector<vector<Point_2>>>>greedyKK;
	vector<vector<vector<int>>>greedyKKSize;
	public:
		Problem_slk_greedy(double w, const char* fileName,int k);
		void setPoint();
		void getMIS_k(int grid_x, int grid_y);
		void getMIS_pre(int grid_x, int grid_y);
		void simple();
		void greedy();
		void greedy_Line();
		void greedy_Between();
		void forcedDeletion(unsigned int  index);
		void forcedAddition(unsigned int  index);
		void freeAddition(unsigned int  index);
		void freeDeletion(unsigned int  index);
		void subGreedy(int x, int y, Point_2& p);

		void getSolution(vector<Point_2>& solution);
		void getSolution_Greedy(vector<Point_2>& solution);
		void getGreedyOneGroup(vector<Point_2>& solution,int line, int group);
		void getGreedyBetween(vector<Point_2>& solution,int line);
		void getGreedyOneGroup_Grid(vector<Point_2>& solution,int line, int group);
		void getOneGroup_Grid(vector<Point_2>& solution, int line,int group);
		void getOneGroup(vector<Point_2>& solution, int line,int group);
		void getFullSolution(vector<Point_2>& solution, int line,int group);

		void addPoint(Point_2 p);
		void addPoint_recompute(int x, int y);
		void addPoint_recompute_pre(int x, int y);
//　Greedy check
		void addPoint_updateGreedyLine(int x,int y);
		void addPoint_updateGreedyBetween(int x,int y);
		void addPoint_updateGreedyLine_pre(int x,int y);
		void addPoint_updateGreedyBetween_pre(int x,int y);

		void subPoint(Point_2 p);
		void subPoint_recompute(int x, int y);
		void subPoint_recompute_pre(int x, int y);
		void checkGreedy(int grid_x, int grid_y);
		void checkGreedy_Line(int grid_x, int grid_y);
		void checkGreedy_Between(int grid_x, int grid_y);
		void checkGreedy_pre(int grid_x, int grid_y);
		void checkGreedy_Line_pre(int grid_x, int grid_y);
		void checkGreedy_Between_pre(int grid_x, int grid_y);


		void subPoint_index(unsigned int index);
		void printSolution();
};
#endif /* SRC_PROBLEM_SLK_GREEDY_H_ */

/*
 * problem_sl_greedy.h
 *
 *  Created on: Nov 19, 2019
 *      Author: guangping
 */

#ifndef SRC_PROBLEM_SL_GREEDY_H_
#define SRC_PROBLEM_SL_GREEDY_H_
#include <vector>
#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/random_polygon_2.h>
#include <algorithm>
#include <math.h>       /* ceil */
#include "problem.h"
using namespace std;
typedef CGAL::Exact_predicates_inexact_constructions_kernel  K;
typedef CGAL::Point_set_2<K>::Vertex_handle  Vertex_handle;
typedef K::Point_2                           Point_2;
typedef std::vector<Point_2> PointVector;
typedef std::pair<unsigned int, unsigned int>                     intPair;
class Problem_sl_greedy:public Problem{
	private:
		vector<vector<list<Point_2>>> gridP;
		vector<vector<int>> gridC;
		int numberV;
		int numberH;
		// size is numberH
		vector<intPair> counter;
		vector<bool> mark;
		int sumE;
		int sumO;
		bool markE;


		// greedy solution
		vector<vector<Point_2>> greedyLine;
		vector<vector<unsigned int>> greedyLineSize;
		vector<vector<Point_2>> greedyEE;//00
		vector<unsigned int>  greedyEESize;
		vector<vector<Point_2>> greedyEO;//01
		vector<unsigned int> greedyEOSize;
		vector<vector<Point_2>> greedyOE;//10
		vector<unsigned int>  greedyOESize;
		vector<vector<Point_2>> greedyOO;//11
		vector<unsigned int> greedyOOSize;

	public:
		Problem_sl_greedy(double w, const char* fileName);
		void setPoint();
		void simple();
		//+++++++++++++++++++++++++greedy++++++++++++++++++++++++++++++++++
		void greedy();
		bool checkGreedy(bool uperE, bool lowerE, int i, int j,Point_2 current);
		void addGreedy(int x, int y, Point_2& p);
		void subGreedy(int x, int y, Point_2& p,bool inSolution);

		//-------------------------greedy----------------------------------
		void forcedDeletion(unsigned int  index);
		void forcedAddition(unsigned int  index);
		void freeAddition(unsigned int  index);
		void freeDeletion(unsigned int  index);

		void addPoint(Point_2 p);
		void subPoint(Point_2 p);

		void subPoint_index(unsigned int index);
	    void getSolution(vector<Point_2>& solution);
		void printSolution();

};
#endif /* SRC_PROBLEM_SL_GREEDY_H_ */

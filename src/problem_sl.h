/*
 * problem_sl.h
 *
 *  Created on: Nov 13, 2019
 *      Author: guangping
 */
#ifndef SRC_PROBLEM_SL_H_
#define SRC_PROBLEM_SL_H_
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
class Problem_sl:public Problem{
	private:
		int numberV;
		int numberH;
		// points on each grid
		vector<vector<list<Point_2>>> gridP;
		// number of points in each grid
		vector<vector<int>> gridC;
		// size is numberH
		// solution size pair on each line
		vector<intPair> counter;
		// choice of each line
		vector<bool> mark;
		// size of even solution
		int sumE;
		// size of odd solution
		int sumO;
		// final choice
		bool markE;
	public:
		Problem_sl(double w, const char* fileName);
		void setPoint();
		void simple();
		void freeAddition(unsigned int  index);
		void freeDeletion(unsigned int  index);
		void addPoint(Point_2 p);
		void subPoint(Point_2 p);
		void subPoint_index(unsigned int index);
		void printSolution();
	protected:
		  ~Problem_sl(void) {}
};





#endif /* SRC_PROBLEM_SL_H_ */

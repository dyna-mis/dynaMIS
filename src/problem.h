/*
 * problem.h
 *
 *  Created on: Sep 13, 2019
 *      Author: guangping
 */

#ifndef PROBLEM_H_
#define PROBLEM_H_
#include <chrono>
#include <fstream>
#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>
#include <iostream>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include "graph.h"
#include "bbox.h"
#include "miscellaneous.h"
#include "jsonM.h"
typedef std::pair<double, double>                     doublePair;
using namespace std;
void rangeSearch(PointSet_CGAL& pSet,Point_2& point, double width,std::list<Vertex_handle>& LV);
class Problem{
protected:
	// sigma: width of squre
	double sigma;
	// h_sigma: half of the width of squre
	double h_sigma;
	//width: width of plane
	double width;
	//height: height of plane
	double height;
	//size: initial number of vertices (may change after add/delete vertices)
	std::size_t size;
	// vector<point>
	PointVector pVector;
	// pair<point,nodeIndex>
	lookupTable luTable;
// MIS
	// solution
	vector<short> mVector;// 1: in, 0: not tested yet, -1: not in
	int lookUp(Point_2& Point);
  ~Problem(void) {}
public:
	Problem(double sigma,const char* fileName);
	void readFile(const char* fileName);
	double getWidth();
	double getHeight();
	double getSigma();
	int getSize();
	PointVector& getPVector(){return pVector;};
	//+++++++++++++++++++++++++get MIS++++++++++++++++++++++++++++++++
	void setConflictGraph();
	virtual void simple() = 0;
	//-------------------------get MIS--------------------------------
	int counter(unsigned int index);

	//+++++++++++++++++++++++++dynamic changing+++++++++++++++++++++++
	void forcedDeletion_sl(int  index);
	void forcedAddition_sl( int  index);
	void freeDeletion_sl(int  index);
	void freeAddition_sl(int  index);
	//-------------------------dynamic changing-----------------------

	//+++++++++++++++++++++++++dynamic changing+++++++++++++++++++++++
	virtual void printSolution();
	//virtual void forcedDeletion(unsigned int index) = 0;
	virtual void freeDeletion(unsigned int index) = 0;
	//virtual void forcedAddition(unsigned int index) = 0;
	virtual void freeAddition(unsigned int index) = 0;
	void addThePoint(Point_2 p);
	virtual void addPoint(Point_2 p)= 0;
	void subThePoint(Point_2 p);
	virtual void subPoint(Point_2 p) = 0;
	virtual void subPoint_index(unsigned int index) = 0;

};




#endif /* PROBLEM_H_ */

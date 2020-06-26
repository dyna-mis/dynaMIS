/*
 * problem_cg.h
 *
 *  Created on: Nov 13, 2019
 *      Author: guangping
 */

#ifndef SRC_PROBLEM_CG_H_
#define SRC_PROBLEM_CG_H_
#include "problem.h"
class Problem_cg:public Problem{
private:
	Graph cg;
	// counter vector, counting the number of neighbors in the current solution for each vertex
	vector<unsigned> cVector;
	int solutionSize;
public:
	Problem_cg(double w, const char* fileName);
	void simple();
	void setConflictGraph();
	//+++++++++++++++++++++++++dynamic changing+++++++++++++++++++++++
	void forcedDeletion(unsigned int index);
	void freeDeletion(unsigned int index);
	void forcedAddition(unsigned int index);
	void freeAddition(unsigned int index);
	void addPoint(Point_2 p);
	void subPoint(Point_2 p);
	void subPoint_index(unsigned int index);
protected:
	  ~Problem_cg(void) {}
};




#endif /* SRC_PROBLEM_CG_H_ */

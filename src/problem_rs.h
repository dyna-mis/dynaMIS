/*
 * problem_rs.h
 *
 *  Created on: Nov 13, 2019
 *      Author: guangping
 */

#ifndef SRC_PROBLEM_RS_H_
#define SRC_PROBLEM_RS_H_
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/random_polygon_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel  K;
typedef CGAL::Point_set_2<K>::Vertex_handle  Vertex_handle;
typedef K::Point_2                           Point_2;
typedef CGAL::Aff_transformation_2<K> Transformation;
typedef std::vector<Point_2> PointVector;
typedef CGAL::Point_set_2<K> PointSet_CGAL;
#include "problem.h"
class Problem_rs:public Problem{
private:
	// range tree of all points
	PointSet_CGAL pSet;
	// range tree of points in solution
	PointSet_CGAL mSet;
	unsigned int solutionSize;
public:
	Problem_rs(double w, const char* fileName);
	void simple();
	void forcedDeletion(int  index);
	void forcedAddition(unsigned int  index);
	void freeAddition(unsigned int  index);
	void freeDeletion(unsigned int  index);
	void addPoint(Point_2 p);
	void subPoint(Point_2 p);
	void subPoint_index(unsigned int index);
protected:
	  ~Problem_rs(void) {}
};



#endif /* SRC_PROBLEM_RS_H_ */

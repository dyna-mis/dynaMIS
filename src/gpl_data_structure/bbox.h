/*
 * bbox.h
 *
 *  Created on: Oct 9, 2019
 *      Author: guangping
 */

#pragma once
#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/random_polygon_2.h>
#include <array>
#include <vector>
//#include <assert.h>
#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include "miscellaneous.h"
using namespace std;
typedef std::pair<double, double>                     doublePair;
typedef CGAL::Exact_predicates_inexact_constructions_kernel  K;
typedef CGAL::Point_set_2<K>::Vertex_handle  Vertex_handle;
typedef K::Point_2                           Point_2;
typedef std::vector<Point_2> PointVector;
typedef CGAL::Point_set_2<K> PointSet_CGAL;
typedef std::unordered_map<doublePair,int, boost::hash<doublePair>> lookupTable;
// TODO BBOX
typedef array<double, 4>     BBox_rep_2;
class isoRect{
private:
	BBox_rep_2 rep;
	bool empty= false;
public:
	isoRect( BBox_rep_2& r);
	isoRect(double xmin,double ymin,double xmax,double ymax);
	inline double getXmin(){ return rep[0]; }
	inline double getYmin(){ return rep[1]; }
	inline double getXmax(){ return rep[2]; }
	inline double getYmax(){ return rep[3]; }
	inline double isEmpty(){ return empty; }
	inline void setXmin(double xmin){rep[0]= xmin;};
	inline void setXmax(double xmax){rep[2]= xmax;};
	inline void setYmin(double ymin){rep[1]= ymin;};
	inline void setYmax(double ymax){rep[3]= ymax;};
	bool contain(Point_2 p);
	void setEmpty(bool value){empty = value;};
	void cut(isoRect& cutter,vector<isoRect>& rList,unsigned int index);
	void rangeSearch(PointSet_CGAL& pSet,std::list<Vertex_handle>& LV);

	//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
	void print();
	void draw(cairo_t* cr);
#endif
	//xPRINT---------------PRINT-------------------------

	//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
	void debug();
#endif
	//!DEBUG---------------debug------------------------

};
class isoPoly{
private:
	vector<isoRect> rList;
public:
	isoPoly(int limit);
	void add(isoRect& r);
	void cut(isoRect& cutter);
	void rangeSearch(PointSet_CGAL& pSet,std::list<Vertex_handle>& LV);
	bool contain(Point_2 p);
	//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
	void print();
	void draw(cairo_t* cr);
#endif
	//xPRINT---------------PRINT-------------------------

	//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
	void debug();
#endif
	//!DEBUG---------------debug------------------------



};

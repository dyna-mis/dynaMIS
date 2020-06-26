/*
 * miscellaneous.h
 *
 *  Created on: Oct 18, 2019
 *      Author: guangping
 */

#ifndef SRC_MISCELLANEOUS_H_
#define SRC_MISCELLANEOUS_H_
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/range_search_delaunay_2.h>
#include <CGAL/Point_set_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/random_polygon_2.h>
#include <fstream>
#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>
#include <iostream>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include <random>
#include <algorithm>
#include "graph.h"
#include "bbox.h"
extern std::mt19937 gen;

//the following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */
extern  cairo_surface_t* surface;
extern  cairo_t *cr;
typedef std::pair<double, double>                     doublePair;
using namespace std;
typedef CGAL::Exact_predicates_inexact_constructions_kernel  K;
typedef CGAL::Point_set_2<K>::Vertex_handle  Vertex_handle;
typedef K::Point_2                           Point_2;
typedef std::vector<Point_2> PointVector;
typedef CGAL::Point_set_2<K> PointSet_CGAL;
typedef CGAL::Vector_2<K> Vector;
typedef std::unordered_map<doublePair,int, boost::hash<doublePair>> lookupTable;

void rangeSearch(PointSet_CGAL& pSet,Point_2& point, double width,std::list<Vertex_handle>& LV);
bool isConflict(double px, double py, double qx, double qy, double width, double height);
bool isConflict(Point_2 p, Point_2 q, double width, double height);
bool isConflicts(vector<Point_2>const& solution, Point_2 q,double width, double height);
bool extendible_OrderedSET(set<Point_2>const& solution, Point_2 q, double width, double height);
bool extendible(vector<Point_2>const& solution, Point_2 q, double width, double height);

enum  Sign
{
    NEGATIVE = -1, ZERO = 0, POSITIVE = 1,

    // Orientation constants:
    RIGHT_TURN = -1, LEFT_TURN = 1,

    CLOCKWISE = -1, COUNTERCLOCKWISE = 1,

    COLLINEAR = 0, COPLANAR = 0, DEGENERATE = 0,

    // Oriented_side constants:
    ON_NEGATIVE_SIDE = -1, ON_ORIENTED_BOUNDARY = 0, ON_POSITIVE_SIDE = 1,

    // Comparison_result constants:
    SMALLER = -1, EQUAL = 0, LARGER = 1
};
void getRandomPoints(std::set<Point_2>& points, double w, double h,int number,double h_sigma);
void getRandomPoints(std::set<Point_2>& points,int number, vector<Point_2>& pVector);
unsigned int getIndex(unsigned int k, unsigned int upper, unsigned int below);
void getKmal(unsigned int k, unsigned int value,pair<unsigned int, unsigned int>& pair);
bool vectorContain(vector<Point_2>& v, Point_2 x);
pair<int,int> getGrid(Point_2 p,int width,int height);

void getPointSet(PointSet_CGAL& set,vector<Point_2>& points, double width,double height);
#endif /* SRC_MISCELLANEOUS_H_ */

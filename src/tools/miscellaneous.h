/*
 * miscellaneous.h
 *
 *  Created on: Oct 18, 2019
 *      Author: guangping
 */

#pragma once
#define _USE_MATH_DEFINES
#include <cmath>


#include <fstream>
#include <iostream>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include <random>
#include <algorithm>
#include "graph.h"
#include "bbox.h"
#include "jsonM.h"
#include "label_point.h"
#include "cxxopts.hpp"
#include "interval.h"
 //xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>
extern  cairo_surface_t* surface;
extern  cairo_t* cr;
extern size_t label_min;
extern size_t label_max;
extern size_t label_sum;
#endif
//xPRINT---------------PRINT-------------------------

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

struct cmp_tol {
    bool operator() (const double& a, const double& b) const {
        double factor_a = a / label_height;
        double intpart_a;
        double fractpart_a = modf(factor_a, &intpart_a);

        double factor_b = b / label_height;
        double intpart_b;
        double fractpart_b = modf(factor_b, &intpart_b);

        return (fractpart_a - fractpart_b) < -0.00003;
    };
};


extern  cairo_surface_t* surface;
extern  cairo_t *cr;
extern string modType;
extern string algoType;
extern string file;
extern double changeRatio;
extern string result_folder;
extern string tmp_dictionary;
extern jsonM measures;
extern int param_k;

extern unsigned seed;
extern bool greedy_v;
extern bool xspecial_v;
extern bool recomp_v;
extern bool rectangle_v;
extern double width;
extern double height;

extern double epsilon;
extern std::unique_ptr<std::mt19937> gen;


extern 	std::set<double, ::cmp_tol> points_x;
extern std::set<double, ::cmp_tol> points_y;

enum class SolveStatus { Optimal = 0, Feasible = 1, Unsolved = -1 };

typedef std::pair<double, double>                     doublePair;
using namespace std;
typedef CGAL::Exact_predicates_inexact_constructions_kernel  K;
typedef CGAL::Point_set_2<K>::Vertex_handle  Vertex_handle;
typedef K::Point_2                           Point_2;
typedef std::vector<Point_2> PointVector;
typedef CGAL::Point_set_2<K> PointSet_CGAL;
typedef std::unordered_map<doublePair,int, boost::hash<doublePair>> lookupTable;


void rangeSearch_neighbor(PointSet_CGAL& pSet, const Point_2& point, std::list<Vertex_handle>& LV);
void rangeSearch_neighbor(PointSet_CGAL& pSet, const label_point& point, std::list<Vertex_handle>& LV);
void rangeSearch(PointSet_CGAL& pSet, const Point_2& point, double ratio, std::list<Vertex_handle>& LV);
bool isConflicts(vector<Point_2>const& solution, const Point_2& q);
bool extendible_OrderedSET(set<interval, lexico_Interval>const& solution, const interval& q);
bool extendible_OrderedSET(set<Point_2>const& solution, const Point_2& q);


bool extendible(vector<Point_2>const& solution, const Point_2& q, double width, double height);



//bool independency(vector<Point_2>const& solution, double width, double height);
//bool independency(set<Point_2>const& solution_set, double width, double height);


unsigned int getIndex(unsigned int k, unsigned int upper, unsigned int below);
void getKmal(unsigned int k, unsigned int value,pair<unsigned int, unsigned int>& pair);
pair<int,int> getGrid(Point_2 p,int width,int height);
//void printSolution(vector<Point_2>&solution,int width,int height);
void getPointSet(PointSet_CGAL& set,vector<Point_2>& points, double width,double height);

void parseInitOptions(int argc, char* argv[]);
void printInitOptions();
void printInitUsage();

inline string get_file_name(const std::string& file) {
	const std::size_t found = file.find_last_of("/\\");
	return  file.substr(found + 1);
}


void outputMeasure(const char* append);


#include <utility>

template<typename C, typename T>
auto insert_in_container(C& c, T&& t) ->
decltype(c.push_front(std::forward<T>(t)), void())
{
    c.push_front(std::forward<T>(t));
}

template<typename C, typename T>
auto insert_in_container(C& c, T&& t) ->
decltype(c.insert(std::forward<T>(t)), void())
{
    c.insert(std::forward<T>(t));
}


template<typename C, typename T>
auto remove_in_container(C& c, T&& t) ->
decltype(c.remove(std::forward<T>(t)), void())
{
    c.remove(std::forward<T>(t));
}

template<typename C, typename T>
auto remove_in_container(C& c, T&& t) ->
decltype(c.erase(std::forward<T>(t)), void())
{
    c.erase(std::forward<T>(t));
}

bool inline isConflict(const Point_2& p, const Point_2& q) {
    if (p.x() + label_width <= q.x() || q.x() + label_width <= p.x()) return false;
    if (p.y() + label_height <= q.y() || q.y() + label_height <= p.y()) return false;
    return true;
}


bool inline isConflict(const interval& p, const interval& q) {
    if (p.second <= q.first || q.second <= p.first) return false;
    if (p.height + label_height <= q.height || q.height + label_height <= p.height) return false;
    return true;

}

bool inline isConflict(const label_point& p, const label_point& q) {
    if (p.x() + p.width_single <= q.x() || q.x() + q.width_single <= p.x()) return false;
    if (p.y() + label_height <= q.y() || q.y() + label_height <= p.y()) return false;
    return true;

}


inline double fRand(double fMin, double fMax)
{
    return ((*gen)() - gen->min()) /(double)(gen->max() - gen->min()) * (fMax - fMin) + fMin;
}

inline double bRand()
{
    return ((*gen)()%2 == 0);
}

inline size_t Rand(size_t min, size_t max) {
    return size_t(fRand(min, max));
}

inline bool comp_len_less(label_point i1, label_point i2) {

    return (i1.width_single < i2.width_single);
};

//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#if defined DYNAMIS_PRINT || defined DYNAMIS_DEBUG 
void print_point(const Point_2& point);

void print_point(const label_point& point);
void print_point(const interval& interval);
#endif

#if defined DYNAMIS_PRINT
void drawPoint(double x, double y, cairo_t* cr);

void startDrawing(double width, double height, const char* fileName);
void finishDrawing();
void drawRect(double x, double y, bool inSolution, bool greedy, cairo_t* cr, double width, double height);
void drawSqure(double x, double y, bool inSolution, bool greedy, cairo_t* cr, double sigma);
#endif
//xPRINT---------------PRINT-------------------------


//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
bool independency(vector<Point_2>const& solution);
bool independency(set<Point_2>const& solution_set);
bool independency(vector<interval>const& solution);
bool independency(std::set<interval, lexico_Interval>const& solution);
bool vectorContain(vector<Point_2>& v, Point_2 x);


int inline isConflict_debug(const Point_2& p, const Point_2& q) {
    if (p.x() + label_width <= q.x() || q.x() + label_width <= p.x()) return -1;
    if (p.y() + label_height <= q.y()|| q.y() + label_height <= p.y()) return -1;

    if (p.x() + label_width <= q.x() + epsilon || q.x() + label_width <= p.x() + epsilon) return 0;
    if (p.y() + label_height <= q.y() + epsilon || q.y() + label_height <= p.y() + epsilon) return 0;
    return 1;
}

int inline isConflict_debug(const label_point& p, const label_point& q) {
    if (p.x() + p.width_single <= q.x() || q.x() + q.width_single <= p.x()) return -1;
    if (p.y() + label_height <= q.y() || q.y() + label_height <= p.y()) return -1;
    if (p.x() + p.width_single <= q.x() + epsilon || q.x() + q.width_single <= p.x() + epsilon) return 0;
    if (p.y() + label_height <= q.y() + epsilon || q.y() + label_height <= p.y() + epsilon) return 0;
    return 1;

}

#endif
//!DEBUG---------------debug------------------------
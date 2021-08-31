#pragma once
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/random_polygon_2.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel  K;
typedef CGAL::Point_set_2<K>::Vertex_handle  Vertex_handle;
typedef K::Point_2                           Point_2;
typedef CGAL::Vector_2<K> Vector;
typedef CGAL::Aff_transformation_2<K> Transformation;
typedef std::vector<Point_2> PointVector;
typedef CGAL::Point_set_2<K> PointSet_CGAL;

extern double h_label_width;
extern double label_width;

extern double h_label_height;
extern double label_height;

struct label_point : Point_2{

	double	width_single = 0;
	size_t multiplier = 0;
	label_point(double x, double y, size_t multiplier_t) : Point_2(x,y){
		width_single = label_width * (double)multiplier_t;
		multiplier = multiplier_t;
	};
	bool operator==(const label_point& r) const { return (width_single == r.width_single)
													&& (x() == r.x()) && (y() == r.y()); }
};


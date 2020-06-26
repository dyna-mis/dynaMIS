/*
 * miscellaneous.cpp
 *
 *  Created on: Oct 18, 2019
 *      Author: guangping
 */
#include "miscellaneous.h"
std::mt19937 gen(0);

// search a square region with point as center and width as ratio (half width)
void rangeSearch(PointSet_CGAL& pSet,Point_2& point, double width,std::list<Vertex_handle>& LV){
	double center_x =point.x();
	double center_y = point.y();
	double ratio = width;
	  Point_2 pt1 = Point_2(center_x-ratio,center_y+ratio);
	  Point_2 pt2 = Point_2(center_x-ratio,center_y-ratio);
	  Point_2 pt3 = Point_2(center_x+ratio,center_y-ratio);
	  Point_2 pt4 = Point_2(center_x+ratio,center_y+ratio);
	  pSet.range_search(pt1,pt2,pt3,pt4, std::back_inserter(LV));
	  //assert(LV.size() > 0);
}

// direct check if two points are in conflict (its range box in conflict)
bool isConflict(double px, double py, double qx, double qy, double width, double height){
	//assert(width > 0);
	//assert(height > 0);
	double half_width = width/2;
	double half_height = height/2;
	double px_min, px_max, py_min, py_max;
	double qx_min, qx_max, qy_min, qy_max;
	px_min = px-half_width;
	qx_min = qx-half_width;
	py_min = py-half_height;
	qy_min = qy-half_height;
	px_max = px+half_width;
	qx_max = qx+half_width;
	py_max = py+half_height;
	qy_max = qy+half_height;
	//assert(px_min < px_max);
	//assert(qx_min < qx_max);
	//std:: cout<< "py_min: "<<py_min<< std::endl;
	//std:: cout<< "qy_max: "<<qy_max<< std::endl;
	if(px_max <=qx_min||qx_max <=px_min ) return false;
	if(py_max <=qy_min||qy_max <=py_min ) return false;
	return true;
}
// direct check if two points are in conflict (its range box in conflict)
bool isConflict(Point_2 p, Point_2 q, double width, double height){
	return isConflict(p.x(), p.y(), q.x(), q.y(),width,height);
}
bool isConflicts(vector<Point_2>const& solution, Point_2 q,double width, double height){
	bool inConflict;
	for(auto p: solution){
		if(p == q) continue;
		inConflict =  isConflict(p.x(), p.y(), q.x(), q.y(), width, height);
		if(inConflict){
			std::cout<<"IN CONFLICT"<< std::endl;
			int x=ceil((p.x()-width/2.0)/width);
			int y=ceil((p.y()-height/2.0)/height);
			std::cout<<"("<<x<<","<<y<<")"<< std::endl;
			std::cout<<"("<<p.x()<<","<<p.y()<<")"<< std::endl;
			x=ceil((q.x()-width/2.0)/width);
			y=ceil((q.y()-height/2.0)/height);
			std::cout<<"("<<x<<","<<y<<")"<< std::endl;
			std::cout<<"("<<q.x()<<","<<q.y()<<")"<< std::endl;
			return inConflict;
		}
	}
	return false;
}

bool extendible_OrderedSET(set<Point_2>const& solution, Point_2 q, double width, double height){
	//std::cout<< "in extendible_OrderedSET"<< std::endl;
	bool inConflict;
	for(auto p: solution){
		if(p == q) return false;
		inConflict =  isConflict(p.x(), p.y(), q.x(), q.y(), width, height);
		if(inConflict) return false;
	}
	return true;
}
bool extendible(vector<Point_2>const & solution, Point_2 q, double width, double height){
	bool inConflict;
	for(auto p: solution){
		if(p == q) return false;
		inConflict =  isConflict(p.x(), p.y(), q.x(), q.y(), width, height);
		if(inConflict) return false;
	}
	return true;
}


unsigned int getIndex(unsigned int k, unsigned int upper, unsigned int below){
	//assert(upper < k+1);
	//assert(below < k+1);
	return upper*(k+1)+below;
};
void getKmal(unsigned int k, unsigned int value,pair<unsigned int, unsigned int>& pair){
	//assert(value < (k+1)*(k+1));
	pair.first = value/(k+1);
	pair.second = value%(k+1);
	/*assert(pair.first < (k+1));
	assert(pair.second < (k+1));
	assert(value == pair.first*(k+1)+pair.second);*/
};

bool vectorContain(vector<Point_2>& v, Point_2 x){
	if(std::find(v.begin(), v.end(), x) != v.end()) {
	    /* v contains x */
		return true;
	} else {
		return false;
	    /* v does not contain x */
	}
}
pair<int,int> getGrid(Point_2 p,int width,int height){
	int x=ceil((p.x()-width/2)/width);
	int y=ceil((p.y()-height/2)/height);
	return pair<int,int>(x,y);
};

void getPointSet(PointSet_CGAL& set,vector<Point_2>& points, double width,double height){
	std::list<Vertex_handle> LV;
	LV.clear();
	points.clear();
	Point_2 pt1 = Point_2(-width,height);
	Point_2 pt2 = Point_2(-width,-height);
	Point_2 pt3 = Point_2(width,-height);
	Point_2 pt4 = Point_2(width,height);
	set.range_search(pt1,pt2,pt3,pt4, std::back_inserter(LV));
	for (std::list<Vertex_handle>::const_iterator it=LV.begin();it != LV.end(); it++){
	     points.push_back((*it)->point());
	}
}


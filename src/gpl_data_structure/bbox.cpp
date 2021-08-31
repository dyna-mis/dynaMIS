/*
 * bbox.cpp
 *
 *  Created on: Oct 9, 2019
 *      Author: guangping
 */

#ifndef BBOX_CPP_
#define BBOX_CPP_
#include "bbox.h"
int orderIndex(double c_min,double c_max,double t_min,double t_max){
	//cm
	if(!(c_min >t_min)){
		//std::cout<< "cm"<< std::endl;
		//cCmM
		if(!(c_max > t_min)){
			//std::cout<< "cCmM"<< std::endl;
			return 0;}
		//mC
		else{
			//cmCM
			if(c_max<t_max){return 1;}
			//cmMC
			else return 2;
		}
	}
	//mc
	else{
		//mcCM
		if(c_max< t_max){return 3;}
		//MC
		else{
			//mcMC
			if(c_min < t_max){return 4;}
			//mMcC
			else return 5;
		}
	}
	//assert(false);
	return -1;
}
isoRect::isoRect(BBox_rep_2& re){
	rep=re;
	 empty= false;
}
isoRect::isoRect(double xmin,double ymin,double xmax,double ymax){
 rep[0]=xmin;
 rep[1]=ymin;
 rep[2]=xmax;
 rep[3]=ymax;
 // TODO: check if valid(empty)
 empty= false;
};
bool isoRect::contain(Point_2 p){
	if(p.x()< rep[0]) return false;
	if(p.y()< rep[1]) return false;
	if(p.x()> rep[2]) return false;
	if(p.x()> rep[3]) return false;
	return true;
};
void isoRect::cut(isoRect& cutter,vector<isoRect>& rList,unsigned int index){
	double c_xmin = cutter.getXmin();
	double c_xmax = cutter.getXmax();
	double c_ymin = cutter.getYmin();
	double c_ymax = cutter.getYmax();
	double xmin = getXmin();
	double xmax = getXmax();
	double ymin = getYmin();
	double ymax = getYmax();
	int x_index = orderIndex(c_xmin,c_xmax,xmin,xmax);
	//std::cout<<"x_index: "<<x_index<< std::endl;
	if(x_index == 0 || x_index == 5)return;
	int y_index = orderIndex(c_ymin,c_ymax,ymin,ymax);
	//std::cout<<"y_index: "<<y_index<< std::endl;
	if(y_index == 0 || y_index == 5)return;
	switch(x_index){
	case 1:
		rList[index].setXmin(c_xmax);
		switch(y_index){
		case 1:{
			rList.push_back(isoRect(xmin,c_ymax,c_xmax,ymax));
			break;
		}
		case 3:{
			rList.push_back(isoRect(xmin,c_ymax,c_xmax,ymax));
			rList.push_back(isoRect(xmin,ymin,c_xmax,c_ymin));
			break;
		}
		case 4:{
			rList.push_back(isoRect(xmin,ymin,c_xmax,c_ymin));
			break;
		}
		//assert(false);
		}
		break;
	case 2:
		switch(y_index){
		case 1:{
			rList[index].setYmin(c_ymax);
			break;
		}
		case 2:{
			rList[index].setEmpty(true);
			break;
		}
		case 3:{
			rList[index].setYmin(c_ymax);
			rList.push_back(isoRect(xmin,ymin,xmax,c_ymin));
			break;
		}
		case 4:{
			rList[index].setYmax(c_ymin);
			break;
		}
		//assert(false);
		}
		break;
	case 3:
		switch(y_index){
		case 1:{
			rList[index].setXmax(c_xmin);
			rList.push_back(isoRect(c_xmin,c_ymax,c_xmax,ymax));
			rList.push_back(isoRect(c_xmax,ymin,xmax,ymax));
			break;
		}
		case 2:{
			rList[index].setXmax(c_xmin);
			rList.push_back(isoRect(c_xmax,ymin,xmax,ymax));
			break;
		}
		case 3:{
			rList[index].setXmax(c_xmin);
			rList.push_back(isoRect(c_xmin,c_ymax,c_xmax,ymax));
			rList.push_back(isoRect(c_xmin,ymin,c_xmax,c_ymin));
			rList.push_back(isoRect(c_xmax,ymin,xmax,ymax));
			break;
		}
		case 4:{
			rList[index].setXmax(c_xmin);
			rList.push_back(isoRect(c_xmin,ymin,c_xmax,c_ymin));
			rList.push_back(isoRect(c_xmax,ymin,xmax,ymax));
			break;
		}
		//assert(false);
		}
		break;
	case 4:
		switch(y_index){
		case 1:{
			rList[index].setXmax(c_xmin);
			rList.push_back(isoRect(c_xmin,c_ymax,xmax,ymax));
			break;
		}
		case 2:{
			rList[index].setXmax(c_xmin);
			break;
		}
		case 3:{
			//cout<< "4----3"<< std::endl;
			rList[index].setXmax(c_xmin);
			rList.push_back(isoRect(c_xmin,c_ymax,xmax,ymax));
			rList.push_back(isoRect(c_xmin,ymin,xmax,c_ymin));
			break;
		}
		case 4:{
			//cout<< "4----4"<< std::endl;
			rList[index].setXmax(c_xmin);
			rList.push_back(isoRect(c_xmin,ymin,xmax,c_ymin));
			break;
		}
		//assert(false);
		}
		break;
	//assert(false);
	}
    // TODO check
};
void isoRect::rangeSearch(PointSet_CGAL& pSet,std::list<Vertex_handle>& LV){
	  Point_2 pt1 = Point_2(getXmin(),getYmax());
	  Point_2 pt2 = Point_2(getXmin(),getYmin());
	  Point_2 pt3 = Point_2(getXmax(),getYmin());
	  Point_2 pt4 = Point_2(getXmax(),getYmax());
	  pSet.range_search(pt1,pt2,pt3,pt4, std::back_inserter(LV));
};
/*void isoRect::print(){
	cout<<"xmin: "<< rep[0]<< endl;
	cout<<"ymin: "<< rep[1]<< endl;
	cout<<"xmax: "<< rep[2]<< endl;
	cout<<"ymax: "<< rep[3]<< endl;
	cout<<"empty "<< empty<< endl;
}
void isoRect::draw(cairo_t *cr){
	if(empty) return;
	if(getXmin()< getXmax()&& getYmin()< getYmax()){
		cairo_rectangle(cr, getXmin(), getYmin(), getXmax()-getXmin(), getYmax()-getYmin());
		cairo_stroke (cr);
	}
}
void isoRect::draw_assert(cairo_t *cr){
	if(empty) return;
	if(getXmin()< getXmax()&& getYmin()< getYmax()){
		cairo_rectangle(cr, getXmin(), getYmin(), getXmax()-getXmin(), getYmax()-getYmin());
		cairo_fill (cr);
	}
	else assert(false);
}
void isoRect::draw_fill(cairo_t *cr){
	if(getXmin()< getXmax()&& getYmin()< getYmax()){
		cairo_rectangle(cr, getXmin(), getYmin(), getXmax()-getXmin(), getYmax()-getYmin());
		cairo_fill (cr);
	}
};
*/
isoPoly::isoPoly(int limit){
	rList.reserve(limit);
};
void isoPoly::cut(isoRect& cutter){
	int size = rList.size();
	int index = 0;
	for(auto r: rList){
		if(r.isEmpty()){
			index++;
			if(index==size) return;
			continue;
		}
		r.cut(cutter,rList,index);
		index++;
		if(index==size) return;
	}
};
bool isoPoly::contain(Point_2 p){
	for(auto r:rList){
		if(r.isEmpty()) continue;
		if(r.contain(p) == true) return true;
	}
	return false;

};
void isoPoly::rangeSearch(PointSet_CGAL& pSet,std::list<Vertex_handle>& LV){
	for(auto r: rList){
		if(r.isEmpty()) continue;
		r.rangeSearch(pSet, LV);
	}

};
/*void isoPoly::print(){
	int index = 0;
	for(auto r: rList){
		std::cout<< index<<":"<<std::endl;
		r.print();
		index++;
	}
};
void isoPoly::draw(cairo_t *cr){
	for(auto r: rList){
		r.draw(cr);
	}
};*/
void isoPoly::add(isoRect& r){
	rList.push_back(r);
};
//To test the cutter works well with the point set
int main_bbox(){

	std::cout<<"helloWord";
	return 0;
}
#endif /* BBOX_CPP_ */

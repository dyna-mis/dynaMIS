/*
 * problem_rs.cpp
 *
 *  Created on: Nov 13, 2019
 *      Author: guangping
 */
#include "problem_rs.h"
Problem_rs::Problem_rs(double w, const char* fileName):Problem(w,fileName){
	for (PointVector::iterator it = pVector.begin() ; it != pVector.end(); ++it){
		pSet.push_back(*it);
	}
	solutionSize = 0;
}
// TODO:: can be faster 			rangeSearch(pSet,pVector[u_index],sigma,LV);-------> search in mSet
void Problem_rs::simple(){
	auto t0 = std::chrono::high_resolution_clock::now();
	std::list<Vertex_handle> LV;
	std::list<Vertex_handle>::const_iterator itv;
	unsigned int  v_index;
	for (unsigned u_index = 0; u_index < size;u_index++  ){
		if(mVector[u_index] == 0){
			mVector[u_index] = 1;
			mSet.push_back(pVector[u_index]);
			solutionSize++;
			LV.clear();
			rangeSearch(pSet,pVector[u_index],sigma,LV);
			for (itv=LV.begin();itv != LV.end(); itv++){
				v_index = lookUp((*itv)->point());
				if(v_index == u_index) continue;
				mVector[v_index] = -1;
			}
		}
	}
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds d = std::chrono::duration_cast< std::chrono::nanoseconds >(fs);
}
void Problem_rs::forcedDeletion(int  index){
	mVector[index] = -1;
	solutionSize--;
	mSet.lookup(pVector[index]);
	mSet.remove(mSet.lookup(pVector[index]));
	isoPoly R_free = isoPoly(1000);
	double x= pVector[index].x();
	double y= pVector[index].y();
	isoRect R_1 = isoRect(x-sigma, y-sigma, x+sigma, y+sigma);
	R_free.add(R_1);
	std::list<Vertex_handle> I_2;
	I_2.clear();
	rangeSearch(mSet,pVector[index], 2*sigma,I_2);
	for (std::list<Vertex_handle>::const_iterator it=I_2.begin();it != I_2.end(); it++){
		isoRect cutter = isoRect((*it)->point().x()-sigma, (*it)->point().y()-sigma, (*it)->point().x()+sigma, (*it)->point().y()+sigma);
		R_free.cut(cutter);
	}
	std::list<Vertex_handle> w;
	while(true){
		w.clear();
		R_free.rangeSearch(pSet,w);
		if(w.size()== 0) {
			break;
		}
		if(w.size()== 1){
			if(lookUp((*(w.begin()))->point())== index){
				break;
			}
		}
		for (std::list<Vertex_handle>::const_iterator it=w.begin();it != w.end(); it++){
			if((*it)->point() == pVector[index]) continue;
			freeAddition(lookUp((*it)->point()));
			isoRect cutter = isoRect((*it)->point().x()-sigma, (*it)->point().y()-sigma, (*it)->point().x()+sigma, (*it)->point().y()+sigma);
			R_free.cut(cutter);
			break;
		}
	}
}
void Problem_rs::freeAddition(unsigned int  index){
	if(mVector[index] != -1){
		std::cout<< "mVector[index]:" << mVector[index]<< std::endl;
	}
	mVector[index] = 1;
	solutionSize++;
	mSet.push_back(pVector[index]);
}
void Problem_rs::forcedAddition(unsigned int  index){
	std::list<Vertex_handle> I_1;
	I_1.clear();
	rangeSearch(mSet,pVector[index], sigma,I_1);
	for (std::list<Vertex_handle>::const_iterator it=I_1.begin();it != I_1.end(); it++){
		if((*it)->point() == pVector[index]) continue;
		mSet.remove((*it));
		mVector[lookUp((*it)->point())] = -1;
	}
	mVector[index]=1;
	mSet.insert(pVector[index]);
	solutionSize++;
	for (std::list<Vertex_handle>::const_iterator it=I_1.begin();it != I_1.end(); it++){
		if((*it)->point() == pVector[index]) continue;
		forcedDeletion(lookUp((*it)->point()));
	}
}
void Problem_rs::freeDeletion(unsigned int  index){
	std::cout<< "in freeDeletion_rs "<<index<< std::endl;
}
void Problem_rs::addPoint(Point_2 p){
	auto t0 = std::chrono::high_resolution_clock::now();
	Problem::addThePoint(p);
	pSet.insert(p);
	std::list<Vertex_handle> I;
	I.clear();
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds d = std::chrono::duration_cast< std::chrono::nanoseconds >(fs);
	t0 = std::chrono::high_resolution_clock::now();
	rangeSearch(mSet,p, sigma,I);
	if(I.size() == 0){
		mSet.insert(p);
		mVector[size-1]=1;
		solutionSize++;
	}
	else  mVector[size-1] = -1;
	t1 = std::chrono::high_resolution_clock::now();
	fs = t1 - t0;
	d = std::chrono::duration_cast< std::chrono::nanoseconds >(fs);
};
void Problem_rs::subPoint(Point_2 p){
	int index = lookUp(p);
	if(mVector[index]==1) forcedDeletion(index);
	Problem::subThePoint(p);
	pSet.remove(pSet.lookup(p));
};
void Problem_rs::subPoint_index(unsigned int index){
	if(mVector[index]==1) forcedDeletion(index);
	Point_2 p = pVector[index];
	Problem::subThePoint(p);
	pSet.remove(pSet.lookup(p));
};

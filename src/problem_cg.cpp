/*
 * problem_cg.cpp
 *
 *  Created on: Nov 13, 2019
 *      Author: guangping
 */
#include "problem_cg.h"
Problem_cg::Problem_cg(double w, const char* fileName):Problem(w,fileName){
	cVector = vector<unsigned int>(size*2,0);
	solutionSize = 0;
}
void Problem_cg::simple(){
	setConflictGraph();
	for (unsigned u_index = 0; u_index < size;u_index++  ){
		if(mVector[u_index] == 0){
			mVector[u_index] = 1;
			solutionSize++;
			for (auto v_index: cg.adList[u_index]){
				cVector[v_index]++;
				mVector[v_index] = -1;
			}
		}
	}
}

// set conflit graph, used only for base-line algorithm
void Problem_cg::setConflictGraph(){
	cg = Graph(size);
	int u_index= 0;
	int v_index;
	double inConflict;
	for (auto p: pVector){
		v_index = 0;
		for (auto q: pVector){
		    if(u_index == v_index) continue;
		    inConflict = isConflict(p.x(), p.y(), q.x(), q.y(), sigma, sigma);
		    if(inConflict){
					cg.addEdge(u_index,v_index);
			}
			v_index++;
		}
		u_index++;
	}
}
//+++++++++++++++++++++++++dynamic changing+++++++++++++++++++++++
void Problem_cg::forcedDeletion(unsigned int index){
	mVector[index] = -1;
	solutionSize--;
	for (auto n_index: cg.adList[index]){
		cVector[n_index]--;
		if(cVector[n_index] == 0){
			freeAddition(n_index);
		}
	}
}
void Problem_cg::freeDeletion(unsigned int index){
	for (auto n_index: cg.adList[index]){
		cVector[n_index]--;
	}
	mVector[index] = -1;
	solutionSize--;
}
void Problem_cg::forcedAddition(unsigned int index){
	assert(mVector[index] == -1);
	for (auto n_index: cg.adList[index]){
		if(mVector[n_index] == 1){
			freeDeletion(n_index);
		}
		cVector[n_index]++;
	}
	mVector[index] = 1;
	solutionSize++;
};
void Problem_cg::freeAddition(unsigned int index){
	for (auto n_index: cg.adList[index]){
		cVector[n_index]++;
	}
	mVector[index] = 1;
	solutionSize++;
}
void Problem_cg::addPoint(Point_2 p){
	auto t0 = std::chrono::high_resolution_clock::now();
	int count = 0;
	Problem::addThePoint(p);
	int index = size-1;
	int v_index = 0;
	bool inConflict = false;
	vector<int> neighbors;
	neighbors.clear();
	for (auto q: pVector){
	    if(index == v_index) continue;
	    inConflict = isConflict(p.x(), p.y(), q.x(), q.y(), sigma, sigma);
	    if(inConflict){
				neighbors.push_back(v_index);
		}
		v_index++;
	}
	cg.addVertex(index,neighbors);
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds d = std::chrono::duration_cast< std::chrono::nanoseconds >(fs);
	t0 = std::chrono::high_resolution_clock::now();
	for(auto e: neighbors){
		if(mVector[e] == 1){
			count++;
		}
	}
	cVector[index] = count;
	//solution part
	if(count > 0){
		mVector[index] = -1;
		t1 = std::chrono::high_resolution_clock::now();
		fs = t1 - t0;
		d = std::chrono::duration_cast< std::chrono::nanoseconds >(fs);
		return;
	}
	else {
		mVector[index] = 1;
		solutionSize++;
	}
	for(auto e: neighbors){
		cVector[e]++;
	}
	t1 = std::chrono::high_resolution_clock::now();
	fs = t1 - t0;
	d = std::chrono::duration_cast< std::chrono::nanoseconds >(fs);
};
void Problem_cg::subPoint(Point_2 p){
	int index = lookUp(p);
	Problem_cg::subPoint_index(index);
};
void Problem_cg::subPoint_index(unsigned int index) {
	if(mVector[index]==1){
		forcedDeletion(index);
	}
	Problem::subThePoint(pVector[index]);
	cVector[index]= cVector[size];
	cg.deleteVertex(index,size);
};


/*
 * graph.cpp
 *
 *  Created on: Sep 24, 2019
 *      Author: guangping
 */
#include "graph.h"
#include <float.h>
#include <string.h>

void Graph::set(size_t n){
    numV = n;
    adList = vector<edgeList>(2*n,edgeList());
}
void Graph::add_edge(unsigned int v1,unsigned int v2){
#ifdef DYNAMIS_DEBUG
    assert(v1 != v2);
#endif
    adList[v1].insert(v2);
    adList[v2].insert(v1);
};
// check if one edge between two vertices exists
// v1: vertexID
// v2: vertexID
bool Graph::containEdge(unsigned int v1, unsigned int v2){
    return adList[v1].count(v2) != 0;
};
// check vertexID<< numV
void Graph::delete_edge(unsigned int v1, unsigned int v2){
    adList[v1].erase(v2);
    adList[v2].erase(v1);
};
void Graph::add_vertex(unsigned int index, vector<int>& neighbors){
	numV++;
	adList[index].clear();
	for(const auto& n : neighbors)
	add_edge(index,n);
};
void Graph::delete_vertex(unsigned int index,unsigned int replace){
	for(const auto& n: adList[index]){
		adList[n].erase(index);
	}
	if(index == replace){
		//assert(replace == numV-1);
		numV--;
		return;
	}
	for(auto n: adList[replace]){
		adList[n].erase(replace);
		adList[n].insert(index);
	}
	adList[index]= adList[replace];
	numV--;
};

//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT

// print adjecency list
void Graph::print() {
    int eSize = 0;
    for (unsigned int i = 0; i < numV; i++) {
        eSize += adList[i].size();
    }
    std::cout << "number of conflicts: " << eSize / 2 << endl;
    for (unsigned int i = 0; i < numV; ++i) {
        cout << i << ": ";
        for (auto itr = adList[i].begin(); itr != adList[i].end(); ++itr) {
            cout << *itr << " ";
        }
        cout << endl;
    }
}


#endif
//xPRINT---------------PRINT-------------------------

//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG

void Graph::debug() {
    edgeList::iterator it;
    for (unsigned int i = 0; i < numV; i++) {
        it = adList[i].begin();
        while (it != adList[i].end()) {
            //undirected graph (u,v)->(v,u)
            assert(containEdge(*it,i));
            //index < numV
            assert(*it <= numV);
            it++;
        }
    }
};
#endif
//!DEBUG---------------debug------------------------

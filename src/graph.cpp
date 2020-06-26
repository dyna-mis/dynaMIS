/*
 * graph.cpp
 *
 *  Created on: Sep 24, 2019
 *      Author: guangping
 */
#include "graph.h"
#include <float.h>
#include <string.h>
Graph::Graph(){

}
Graph::Graph(int n){
    numV = n;
    adList = vector<edgeList>(2*n,edgeList());
}
void Graph::addEdge(unsigned int v1,unsigned int v2){
    //assert(v1< numV);
    //assert(v2 < numV);
    adList[v1].insert(v2);
    adList[v2].insert(v1);
};
// check if one edge between two vertices exists
// v1: vertexID
// v2: vertexID
bool Graph::containEdge(unsigned int v1, unsigned int v2){
    return (adList[v1].count(v2) != 0 && adList[v2].count(v1)!= 0);
};

void Graph::deleteEdge(unsigned int v1, unsigned int v2){
    //assert(v1< numV);
    //assert(v2 < numV);
    adList[v1].erase(v2);
    adList[v2].erase(v1);

};
void Graph::addVertex(unsigned int index, vector<int>& neighbors){
	//assert(index == numV);
	numV++;
	adList[index].clear();
	for(auto n : neighbors)
	addEdge(index,n);
};
void Graph::deleteVertex(unsigned int index,unsigned int replace){
	for(auto n: adList[index]){
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


/*
 * graph.h
 *
 *  Created on: Sep 24, 2019
 *  Author: guangping
 *  Self-defined graph representation.
 *  Considering the extern algorithms we choose may be sensitive to the size of verices, we use this way.
 */

#ifndef SRC_GRAPH_H_
#define SRC_GRAPH_H_


//#include <assert.h>
#include <unordered_set>
#include <iostream>
#include<fstream>
#include <unordered_set>
#include <numeric>
#include <string.h>
#include <stdlib.h>
#include "miscellaneous.h"
using namespace std;
typedef unordered_set<unsigned int> edgeList;
class Graph{
	private:
		unsigned int numV;
    public:
		vector<edgeList> adList;
		Graph();
        Graph(int n);

        void addEdge(unsigned int source, unsigned int target);
        bool containEdge(unsigned int source, unsigned int target);

        //dynamic
        void deleteEdge(unsigned int source, unsigned int target);
        void addVertex(unsigned int index, vector<int>& neighbors);
        void deleteVertex(unsigned int index,unsigned int replace);
        int getEdgeSize(){
            int eSize = 0;
            for (unsigned int i =0 ; i < numV; i++) {
                eSize+= adList[i].size();
            }
            return eSize/2;
        }
};
#endif /* SRC_GRAPH_H_ */

/*
 * graph.h
 *
 *  Created on: Sep 24, 2019
 *  Author: guangping
 *  Self-defined graph representation.
 *  Considering the extern algorithms we choose may be sensitive to the size of verices, we use this way.
 */

#pragma once

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
class Graph {
public:
    Graph() {};
    void set(size_t n);
    vector<edgeList> adList;

    void add_edge(unsigned int source, unsigned int target);
    bool containEdge(unsigned int source, unsigned int target);
    //dynamic
    void delete_edge(unsigned int source, unsigned int target);
    void add_vertex(unsigned int index, vector<int>& neighbors);
    void delete_vertex(unsigned int index, unsigned int replace);
    size_t get_vertex_size() {
        return numV;
    };
    size_t get_edge_size() {
        size_t eSize = 0;
        for (unsigned int i = 0; i < numV; i++) {
            eSize += adList[i].size();
        }
        return eSize / 2;
    };
    //xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
    void print();
#endif
    //xPRINT---------------PRINT-------------------------

    //!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
    void debug();
#endif
    //!DEBUG---------------debug------------------------


protected:
    unsigned int numV = 0;


};


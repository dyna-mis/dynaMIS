/*
 * main.h
 *
 *  Created on: Nov 24, 2019
 *      Author: ping
 */

#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_
#include "problem.h"
#include "miscellaneous.h"
#include "problem_cg.h"
#include "problem_rs.h"
#include "problem_sl.h"
#include "problem_sl_greedy.h"
#include "problem_slk.h"
#include "problem_slk_greedy.h"
#include "problem_arrow.h"
#include "problem_arrow_greedy.h"
#include <getopt.h>
Problem* p;
char* file;
std::string algoType;
double h_sigma;
double sigma=30.0;
bool outputFlag;
//width: width of plane
double w_bound;
//height: height of plane
double h_bound;
int number;
void parseInitOptions(int argc, char *argv[]);
void parseAlgoIndex();
void mod();
void printInitUsage();
void printModUsage();
void printInitOptions();
Problem* ProblemPointer;

int k = 2;



#endif /* SRC_MAIN_H_ */

/*
 * json.h
 *
 *  Created on: Nov 24, 2019
 *      Author: ping
 */

#ifndef SRC_JSONM_H_
#define SRC_JSONM_H_
#include <iostream>
#include <fstream>
#include "json.hpp"

using namespace std;
using json = nlohmann::json ;
class jsonM{
private:
	string fileName;
	json jObject;
public:
	jsonM(const char* fileS);
	void print();
	void debug();
	void output(const char* outfolder,const char* appendix);
	void addElement(string key,string value);
};




#endif /* SRC_JSONM_H_ */

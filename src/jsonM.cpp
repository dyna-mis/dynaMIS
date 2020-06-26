/*
 * json.cpp
 *
 *  Created on: Nov 24, 2019
 *      Author: ping
 */
#include "jsonM.h"
jsonM::jsonM(const char* fileS){
	fileName = string(fileS);

};
void jsonM::print(){
	std::cout<< fileName << std::endl;
	std::cout<<jObject;
};
void jsonM::addElement(string key,string value){
	if(jObject.find(key) != jObject.end()){
		if(jObject[key].is_array()){
			jObject[key]. push_back(value);
		}
		else{
			auto oldValue =  jObject[key];
			jObject[key] = {oldValue,value};
		}
	}
	else jObject[key] = value;
}
void jsonM::debug(){
}
void jsonM::output(const char* outfolder,const char* appendix){
	std::ofstream ofs;
	string outFile = fileName.c_str();
	std::string key = "/";
	std::size_t found = outFile.rfind(key);
	outFile = outFile.substr(found+1);
	outFile = string(outfolder)+"RESULT/"+outFile;
	//outFile = "/home1/guangping/dynaMIS/RESULT/"+outFile;
	outFile+=appendix;
	std::cout<< "write file: "<<outFile<< std::endl;
	try {
		ofs.open(outFile, fstream::out);
	}
	catch(std::ofstream::failure& e){
		std::cerr << "Exception opening file: " << std::strerror(errno) << "\n";
	}
	ofs<<jObject;
	ofs.close();
}

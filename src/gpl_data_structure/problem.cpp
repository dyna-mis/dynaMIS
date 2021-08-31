#include "problem.h"


void Problem::set() {
	readFile();
	int i = 0;
	for (PointVector::iterator it = pVector.begin(); it != pVector.end(); ++it) {
		luTable[pair<double, double>((*it).x(), (*it).y())] = i;
		i++;
	}
};


int Problem::look_up(const Point_2& Point) const {
	//std::cout <<"in Problem::lookUp"<<Point.x()<<" "<< Point.y()<<std::endl;
	doublePair p(Point.x(), Point.y());
	std::unordered_map<doublePair, int, boost::hash<doublePair>>::const_iterator got = luTable.find(p);
	if (got != luTable.end()) {
		// std::cout <<"out Problem::lookUp"<< std::endl;
		return got->second;
	}
	// std::cout <<"out Problem::lookUp"<< std::endl;
	return -1;
};


void Problem::add_point(const Point_2& p) {
	luTable[pair<double, double>(p.x(), p.y())] = pVector.size();
	pVector.push_back(p);
};
void Problem::delete_point(size_t index) {
	Point_2 p = pVector[index];
	int replace = pVector.size() - 1;
	doublePair p_pair(p.x(), p.y());
	if (index != replace) {
		pVector[index] = pVector[replace];
		doublePair rep_pair(pVector[replace].x(), pVector[replace].y());
		auto it = luTable.find(rep_pair);
		//assert(it != luTable.end());
		if (it != luTable.end()) {
			it->second = index;
		}
		pVector[index] = pVector[replace];
	}
	int number = luTable.erase(p_pair);
	//assert(number == 1);
	pVector.pop_back();
};

void Problem::read_label_info(char* str) {
	strtok(str, " ");
	double x = atof(strtok(NULL, " "));
	double y = atof(strtok(NULL, " "));
	Point_2 p(x, y);
	if (check_valid(p)) {
		pVector.push_back(p);
		points_x.insert(p.x());
		points_x.insert(p.x() + label_width);
		points_y.insert(p.y());
	}

};



void Problem::readFile() {
		//cout<< "in readFile"<<endl;
		ifstream fp;
		//fp.open("Debug/instance.cnf",std::ios::in);
		fp.open(file, std::ios::in);
		if (!fp.is_open()) {
			std::cout << file << std::endl;
			perror("read file fails");
			exit(EXIT_FAILURE);
		}
		string buff;
		char head;
		getline(fp, buff);
		getline(fp, buff);
		while (!fp.eof()) {
			if (buff.empty()) continue;
			head = buff.at(0);
			if (head == 'p') {
				break;
			}
			getline(fp, buff);
		}
		//std::cout<< buff<< std::endl;
		// Parse the p line
		char* str = strdup(buff.c_str());
		const char s[2] = " ";
		strtok(str, s);
		strtok(NULL, s);
		size_t point_size = (size_t)atoi(strtok(NULL, s));
		pVector.reserve(point_size * 2);
		width = atof(strtok(NULL, s));
		height = atof(strtok(NULL, s));
		for (unsigned int i = 0; i < point_size; i++) {
			getline(fp, buff);
			if (buff.empty()) continue;
			str = strdup(buff.c_str());
			read_label_info(str);
		}
	};

//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT


void Problem::draw(cairo_t *cr){
	double x, y;
	int i = 0;
	for(auto e : pVector){
		x = e.x()+0.1*width;
		y = 1.1*height-e.y();
		cairo_move_to(cr,x,y);
		cairo_show_text(cr,std::to_string(i).c_str());
		cairo_move_to(cr,x,y);
		drawPoint(x,y,cr);
		i++;
	}
}

void Problem::print(){
	//	double sigma;
	std::cout<< "label_height: "<< label_height<<std::endl;
	std::cout << "label_width: " << label_width << std::endl;
	std::cout<< "width: "<< width<<std::endl;
	std::cout<< "height: "<< height<<std::endl;
	std::cout << "pVector: "<< std::endl;
	for (unsigned int i = 0; i < pVector.size(); i++) {
		std::cout << i + 1 << ": " << pVector[i].x() << " " << pVector[i].y() << std::endl;
	}

	std::cout << "luTable: " << luTable.size() << std::endl;
	int i = 0;
	for (auto e : luTable) {
		std::cout << i << " key:(" << e.first.first << "," << e.first.second << ") index:" << e.second << std::endl;
		i++;
	}

}
#endif
//xPRINT---------------PRINT-------------------------

//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG

void Problem:: debug(){
	  assert(luTable.size() == pVector.size());
	  int i = 0;
	  for(auto p: pVector){
		assert(look_up(p) == i);
		i++;
	  }
}
#endif
//!DEBUG---------------debug------------------------
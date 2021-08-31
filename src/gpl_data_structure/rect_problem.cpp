#include "rect_problem.h"


void rect_problem::set() {
	readFile();
	int i = 0;
	for (auto it = pVector.begin(); it != pVector.end(); ++it) {
		luTable[pair<double, double>((*it).x(), (*it).y())] = i;
		i++;
	}

}


void rect_problem::read_label_info(char* str) {
	strtok(str, " ");
	double x = atof(strtok(NULL, " "));
	double y = atof(strtok(NULL, " "));
	double width = atof(strtok(NULL, " "));
	label_point p(x, y, width);
	if (check_valid(p)) 
	{
		pVector.push_back(p);
		points_x.insert(x);
		points_x.insert(p.x() + p.width_single);
		points_y.insert(y);
		
//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
		if (p.multiplier < label_min) {
			label_min = p.multiplier;
		}
		if (p.multiplier > label_max) {
			label_max = p.multiplier;
		}
		label_sum += p.multiplier;
#endif
//xPRINT---------------PRINT-------------------------
	}


}


void rect_problem::add_point(const label_point& p) {
	luTable[pair<double, double>(p.x(), p.y())] = pVector.size();
	pVector.push_back(p);

};


void rect_problem::delete_point(size_t index) {
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


void rect_problem::readFile() {
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
	}	if(xspecial_v) sort(pVector.begin(), pVector.end(), comp_len_less);
};

int rect_problem::look_up(const label_point& Point) const {
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


//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT


void rect_problem::draw(cairo_t* cr) {
	double x, y;
	int i = 0;
	for (const auto& e : pVector) {
		x = e.x() + 0.1 * width;
		y = 1.1 * height - e.y();
		cairo_move_to(cr, x, y);
		cairo_show_text(cr, std::to_string(i).c_str());
		cairo_move_to(cr, x, y);
		drawPoint(x, y, cr);
		i++;
	}
}

void rect_problem::print() {
	//	double sigma;
	std::cout << "label_height: " << label_height << std::endl;
	std::cout << "label_width: " << label_width << std::endl;
	std::cout << "width: " << width << std::endl;
	std::cout << "height: " << height << std::endl;
	std::cout << "pVector: " << std::endl;
	for (unsigned int i = 0; i < pVector.size(); i++) {
		std::cout << i + 1 << ": " << pVector[i].x() << " " << pVector[i].y() << std::endl;
	}

	std::cout << "luTable: " << luTable.size() << std::endl;
	int i = 0;
	for (const auto& e : luTable) {
		std::cout << i << " key:(" << e.first.first << "," << e.first.second << ") index:" << e.second << std::endl;
		i++;
	}

}
#endif
//xPRINT---------------PRINT-------------------------

//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG

void rect_problem::debug() {
	assert(luTable.size() == pVector.size());
	int i = 0;
	for (const auto& p : pVector) {
		assert(look_up(p) == i);
		i++;
	}
}
#endif
//!DEBUG---
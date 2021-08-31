/*
 * miscellaneous.cpp
 *
 *  Created on: Oct 18, 2019
 *      Author: guangping
 */
#include "miscellaneous.h"
jsonM measures;
double width;
double height;
double epsilon = 0.0000001;

std::set<double, ::cmp_tol> points_x;
std::set<double, ::cmp_tol> points_y;
//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
cairo_surface_t* surface;
cairo_t* cr;
size_t label_min = 100;
size_t label_max = 0;
size_t label_sum = 0;
#endif
//xPRINT---------------PRINT-------------------------
// search a square region with point as center and width as ratio (half width)
void rangeSearch_neighbor(PointSet_CGAL& pSet, const Point_2& point,std::list<Vertex_handle>& LV){
	double center_x =point.x();
	double center_y = point.y();
	Point_2 pt1 = Point_2(center_x- label_height + epsilon,center_y+ label_height- epsilon);
	Point_2 pt2 = Point_2(center_x- label_height+ epsilon,center_y- label_height+ epsilon);
	Point_2 pt3 = Point_2(center_x+ label_height- epsilon,center_y- label_height+ epsilon);
	Point_2 pt4 = Point_2(center_x+ label_height- epsilon,center_y+ label_height- epsilon);
	  pSet.range_search(pt1,pt2,pt3,pt4, std::back_inserter(LV));
	  //assert(LV.size() > 0);
}



void rangeSearch_neighbor(PointSet_CGAL& pSet, const label_point& point, std::list<Vertex_handle>& LV) {
	double left_x = point.x() + epsilon;
	double bottom_y = point.y() + epsilon;
	double right_x = point.x() + point.width_single- epsilon;
	double top_y = point.y() + label_height - epsilon;

	Point_2 pt1 = Point_2(left_x, top_y);
	Point_2 pt2 = Point_2(left_x, bottom_y);
	Point_2 pt3 = Point_2(right_x, bottom_y);
	Point_2 pt4 = Point_2(right_x, top_y);
	pSet.range_search(pt1, pt2, pt3, pt4, std::back_inserter(LV));
	//assert(LV.size() > 0);
}




void rangeSearch(PointSet_CGAL& pSet, const Point_2& point, double ratio, std::list<Vertex_handle>& LV) {
	double center_x = point.x();
	double center_y = point.y();
	Point_2 pt1 = Point_2(center_x - ratio, center_y + ratio);
	Point_2 pt2 = Point_2(center_x - ratio, center_y - ratio);
	Point_2 pt3 = Point_2(center_x + ratio, center_y - ratio);
	Point_2 pt4 = Point_2(center_x + ratio, center_y + ratio);
	pSet.range_search(pt1, pt2, pt3, pt4, std::back_inserter(LV));
	//assert(LV.size() > 0);
}








// direct check if two points are in conflict (its range box in conflict)
bool isConflict(double px, double py, double qx, double qy, double width, double height){
	//assert(width > 0);
	//assert(height > 0);
	double half_width = width/2;
	double half_height = height/2;
	double px_min, px_max, py_min, py_max;
	double qx_min, qx_max, qy_min, qy_max;
	px_min = px-half_width;
	qx_min = qx-half_width;
	py_min = py-half_height;
	qy_min = qy-half_height;
	px_max = px+half_width;
	qx_max = qx+half_width;
	py_max = py+half_height;
	qy_max = qy+half_height;
	//assert(px_min < px_max);
	//assert(qx_min < qx_max);
	//std:: cout<< "py_min: "<<py_min<< std::endl;
	//std:: cout<< "qy_max: "<<qy_max<< std::endl;
	if(px_max <=qx_min||qx_max <=px_min ) return false;
	if(py_max <=qy_min||qy_max <=py_min ) return false;
	return true;
}

bool isConflicts(vector<Point_2>const& solution, const Point_2& q){
	bool inConflict;
	for(auto p: solution){
		if(p == q) continue;
		inConflict =  isConflict(p,q);
		if(inConflict){
			std::cout<<"IN CONFLICT"<< std::endl;
			size_t x = ceil((p.x() - h_label_width) / label_width);
			size_t y = ceil((p.y() - h_label_height) / label_height);
			std::cout << "(" << x << "," << y << ")" << std::endl;

			std::cout<<"("<<p.x()<<","<<p.y()<<")"<< std::endl;


			x=ceil((q.x()- h_label_width )/ label_width);
			y=ceil((q.y()- h_label_height)/ label_height);
			std::cout<<"("<<x<<","<<y<<")"<< std::endl;
			std::cout<<"("<<q.x()<<","<<q.y()<<")"<< std::endl;
			return inConflict;
		}
	}
	return false;
}


/*	auto next = solution[y].lower_bound(p);
	if (next != solution[y].begin()) {
		auto prev = std::prev(next);
		if (p.x() < (*prev).x() + label_width) return false;
	}*/

bool extendible_OrderedSET(set<interval, lexico_Interval>const& solution, const interval& q) {
	//std::cout<< "in extendible_OrderedSET"<< std::endl;
	auto it = solution.lower_bound(q);
	auto its = it;
	bool inConflict = false;
	while (its != solution.end() && its->first < q.second) {
		inConflict = isConflict(q, *its);
		if (inConflict) return false;
		else its = std::next(its);
	}
	// test previous one
	if(it != solution.begin()) {
		it = std::prev(it);
		inConflict = isConflict(q,*it);
		if (inConflict) return false;
		else return true;
	}
	return true;
}

bool extendible_OrderedSET(set<Point_2>const& solution, const Point_2& q){

	auto it = solution.lower_bound(q);
	bool inConflict = false;
	if (it != solution.end()) {
		inConflict = isConflict(q, *it);
		if (inConflict) return false;
	}
	// test previous one
	if (it != solution.begin()) {
		it = std::prev(it);
		inConflict = isConflict(q, *it);
		if (inConflict) return false;
		else {
			if (it != solution.begin()) {
				it = std::prev(it);
				inConflict = isConflict(q, *it);
				if (inConflict) return false;
				else return true;
			}
			else return true;
		} 
	}
	return true;
}
bool extendible(vector<Point_2>const & solution, const Point_2& q, double width, double height){
	bool inConflict;
	for(auto p: solution){
		if(p == q) return false;
		inConflict =  isConflict(p.x(), p.y(), q.x(), q.y(), width, height);
		if(inConflict) return false;
	}
	return true;
}


unsigned int getIndex(unsigned int k, unsigned int upper, unsigned int below){
	//assert(upper < k+1);
	//assert(below < k+1);
	return upper*(k+1)+below;
};
void getKmal(unsigned int k, unsigned int value,pair<unsigned int, unsigned int>& pair){
	//assert(value < (k+1)*(k+1));
	pair.first = value/(k+1);
	pair.second = value%(k+1);
	/*assert(pair.first < (k+1));
	assert(pair.second < (k+1));
	assert(value == pair.first*(k+1)+pair.second);*/
};



pair<int,int> getGrid(Point_2 p,int width,int height){
	int x=ceil((p.x()-width/2)/width);
	int y=ceil((p.y()-height/2)/height);
	return pair<int,int>(x,y);
};
/*
void printSolution(vector<Point_2>& solution,int width,int height){
	pair<int,int> grid;
	for(auto e: solution){
		grid = getGrid(e,width,height);
		std::cout<<"("<< e.x()<<"，"<< e.y()<<")"<<"[ "<< grid.first<<","<< grid.second<<"]"<< std::endl;
	}
	std::cout<<std::endl;
};

int main_kl(){
	unsigned int value = 63;
	int k = 7;
	pair<unsigned int, unsigned int> pair;
	getKmal(k,value,pair);
	//assert(value == getIndex(k,pair.first, pair.second));
	std::cout<< "alles gute"<< std::endl;
	return 0;

};*/
void getPointSet(PointSet_CGAL& set,vector<Point_2>& points, double width,double height){
	std::list<Vertex_handle> LV;
	LV.clear();
	points.clear();
	Point_2 pt1 = Point_2(-width,height);
	Point_2 pt2 = Point_2(-width,-height);
	Point_2 pt3 = Point_2(width,-height);
	Point_2 pt4 = Point_2(width,height);
	set.range_search(pt1,pt2,pt3,pt4, std::back_inserter(LV));
	for (std::list<Vertex_handle>::const_iterator it=LV.begin();it != LV.end(); it++){
	     points.push_back((*it)->point());
	}
}

string modType;
string algoType;
string file;
double changeRatio;
string result_folder;
string tmp_dictionary;
int param_k;
unsigned seed;
bool greedy_v = false;
bool xspecial_v = false;
bool recomp_v = false;
bool rectangle_v = false;
void parseInitOptions(int argc, char* argv[]) {
	cxxopts::Options options("dynaMIS", "Geometric MIS");

	options.add_options()
		("a,algorithm", "Algorithm Option", cxxopts::value<std::string>())
		("m,modification", "modType Option", cxxopts::value<std::string>())
		("r,changeRatio", "changeRatio", cxxopts::value<double>())
		("l,height", "label height", cxxopts::value<double>())
		("w,width", "label width", cxxopts::value<double>())
		("k", "gridK", cxxopts::value<size_t>()->default_value("4"))
		("h,help", "Print usage")
		("p,problem", "Rectangle Problem Model")
		("g,greedy", "Greedy Augmentation")
		("x,xspecial", "Greedy graph for rectangle")
		("f,filename", "input file", cxxopts::value<std::string>())
		("d", "dictionary", cxxopts::value<std::string>()->default_value("D:/GIT/C++/dynaMIS"))
		("t", "tmp_dictionary", cxxopts::value<std::string>()->default_value("D:/GIT/C++/dynaMIS"))
		("s", "seed", cxxopts::value<unsigned>()->default_value("0"))
		("c,recomputation", "static version")
		;
	options.allow_unrecognised_options();
	auto result = options.parse(argc, argv);
	if (result.count("filename")) {
		file = strdup(result["filename"].as<std::string>().c_str());
		string outFile(file);
		std::string key = "/";
		std::size_t found = outFile.rfind(key);
		outFile = outFile.substr(found + 1);
		measures.addElement("info", "file", outFile);
	}
	else {
		std::cerr << "Input file missing" << std::endl;
		printInitUsage();
		exit(0);
	}

	if (result.count("help"))
	{
		printInitUsage();
		exit(0);
	}

	if (result.count("xspecial"))
	{
		xspecial_v = true;
	}
	if (result.count("greedy"))
	{
		greedy_v = true;
	}

	if (result.count("recomputation"))
	{
		recomp_v = true;
	}

	if (result.count("problem"))
	{
		rectangle_v = true;
	}
	result_folder = result["d"].as<std::string>();
	tmp_dictionary = result["t"].as<std::string>();
	seed = result["s"].as<unsigned>();

	param_k = result["k"].as<size_t>();
	measures.addElement("info", "param_k", std::to_string(param_k));

	if (result.count("algorithm")) {
		algoType = result["algorithm"].as<std::string>();
		measures.addElement("info", "algo_type", algoType);
	}
	if (result.count("modification")) {
		modType = result["modification"].as<std::string>();
		measures.addElement("info", "modification", modType);
	}
	if (result.count("changeRatio")) {
		changeRatio = result["changeRatio"].as<double>();
		measures.addElement("info", "changeRatio", std::to_string(changeRatio));
	}

	if (result.count("height")) {
		label_height = result["height"].as<double>();
		h_label_height = label_height/2.0;
		measures.addElement("info", "label height", std::to_string(label_height));
	}

	if (result.count("width")) {
		label_width = result["width"].as<double>();
		h_label_width = label_width / 2.0;
		measures.addElement("info", "label width", std::to_string(label_width));
	}
};
void printInitOptions() {
	printf("dynaMIS Initialization options \n");
	cout << "file:" << file << endl;
	cout << "algorithm:" << algoType << "-" << param_k << endl;
	cout << "modType:" << modType << endl;
	cout << "changeRatio:" << changeRatio << endl;
	cout << "result_folder:" << result_folder << endl;

	cout << "label_width:" << label_width << endl;
	cout << "label_height:" << label_height << endl;

	cout << "greedy:" << greedy_v << endl;
	cout << "rectangle:" << rectangle_v << endl;

	cout << "seed:" << seed<< endl;
}

/*print the manual of the program*/
void printInitUsage() {
	cout << "\n--------dynaMIS, a dynamic MIS solver(simulation) by " << BLUE << "TUWIEN" << RESET << " -----------------\n";
	printf("\nauthor: Sujoy Bhore, Guangping Li and Martin Nöllenburg \n");
	printf("Human-centered Algorithm Engineering: Graph and Map Visualization\n");
	printf("Research Project supported by the Austrian Science Fund(FWF), grant P 31119\n");
	printf("https://www.ac.tuwien.ac.at/research/humalgo/\n");

	printf("\n-------------------Usage--------------------------------------------------------\n");
	printf("./dynaMIS  <instance> [options]\n");

	printf("\n-------------------Options------------------------------------------------------\n");
	printf("   --output, -o : output the solution\n");
	printf("   --help, -h : output this help\n");
	printf("   --greedy, -g : greedy augmentation on\n");
	printf("   --problem, -p : rectangle problem\n");
	printf("   --algorithm, -a : algorithm in use (see options below)\n");
	printf("   --sigma, -s : width of a square\n");
	printf("---------------------------------------------------------------------------------\n");
	printf("\n-------------------algorithm options------------------------------------------------------\n");
	printf("   ors : a dynamic MIS algorithm based on orthogonal range searching \n");
	printf("   graph: a graph-based dynamic MIS algorithm\n");
	printf("   grid: a grid-based 4-approximation algorithm \n");
	printf("   gridK: The group-shifting based algorithm. Need a extra parameter -k\n");
	printf("   line: stabbing-line based 2-approximation algorithm\n\n");
	printf("   *More details see our paper \"Independent Sets of Dynamic Rectangles:Algorithms and Experiments\" <ARXIV LINK>\n");
	printf("---------------------------------------------------------------------------------\n");
}


void outputMeasure(const char* append) {
	string outFile(file);
	std::string key = "/";
	std::size_t found = outFile.rfind(key);
	outFile = outFile.substr(found + 1);
	string appendix;
	appendix += "-";
	appendix += algoType;

	appendix += "-";
	appendix += modType;
	appendix += append;
	measures.output(result_folder.c_str(), appendix.c_str(), outFile.c_str());
}

//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#if defined DYNAMIS_PRINT

void startDrawing(double width, double height, const char* fileName) {
	surface = cairo_pdf_surface_create(fileName, width * 1.2, height * 1.2);
	cr = cairo_create(surface);
	cairo_select_font_face(cr, "serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
	cairo_set_font_size(cr, 10.0);
	cairo_set_source_rgb(cr, 0.0, 0.0, 1.0);
	cairo_set_line_width(cr, 2.5);
	cairo_rectangle(cr, width * 0.1, height * 0.1, width, height);
	cairo_stroke(cr);
}
void finishDrawing() {
	cairo_destroy(cr);
	cairo_surface_destroy(surface);
}


// draw the a squre of width sigma and center （x,y）
void drawSqure(double x, double y, bool inSolution, bool greedy, cairo_t* cr, double sigma) {
	if (inSolution) cairo_set_source_rgb(cr, 0.0, 1.0, 0.0);
	if (greedy) cairo_set_source_rgb(cr, 0.0, 0.0, 1.0);
	cairo_rectangle(cr, x - sigma / 2, y - sigma / 2, sigma, sigma);
	cairo_stroke(cr);
	cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
}

void drawRect(double x, double y, bool inSolution, bool greedy, cairo_t* cr, double width, double height) {
	if (inSolution) cairo_set_source_rgb(cr, 0.0, 1.0, 0.0);
	if (greedy) cairo_set_source_rgb(cr, 0.0, 0.0, 1.0);
	cairo_rectangle(cr, x - width / 2, y - height / 2, width, height);
	cairo_stroke(cr);
	cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
}
void drawPoint(double x, double y, cairo_t* cr) {
	cairo_arc(cr, x, y, 1, 0, 2 * M_PI);
	cairo_stroke_preserve(cr);
	cairo_fill(cr);
}
#endif

#if defined DYNAMIS_PRINT || defined DYNAMIS_DEBUG 

void print_point(const Point_2& point) {
	std::cout << "( " << point.x() << "," << point.y() << ") ";
}

void print_point(const label_point& point) {
	std::cout << "( " << point.x() << "," << point.y() << "," << point.width_single <<  ") ";
}

void print_point(const interval& interval ) {
	std::cout << "( " << interval.first << "," << interval.second << "," << interval.height << ") ";
}

#endif
//xPRINT---------------PRINT-------------------------




//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
bool independency(vector<Point_2>const& solution) {
	//std::cout<< "in independency"<< std::endl;
	bool inConflict;
	for (auto e : solution) {
		inConflict = isConflicts(solution, e);
		if (inConflict) {
			return false;
		}
		assert(!inConflict);
	}
	//std::cout<< "out independency"<< std::endl;
	return true;
}

bool independency(vector<interval>const& solution) {
	//std::cout<< "in independency"<< std::endl;
	bool inConflict;
	for (size_t i = 0; i < solution.size(); i++) {
		for (size_t j = i + 1; j < solution.size(); j++) {
			assert(!isConflict(solution[i], solution[j]));
		}
	}
	//std::cout<< "out independency"<< std::endl;
	return true;
}

bool independency(std::set<interval, lexico_Interval>const& solution) {
	//std::cout<< "in independency"<< std::endl;
	bool inConflict;
	for (const auto& it: solution)
	{
		for (const auto& iq:solution ){
			if (it == iq) continue;
			assert(!isConflict(it, iq));
		}
	}
	//std::cout<< "out independency"<< std::endl;
	return true;
}


bool independency(set<Point_2>const& solution_set) {
	vector<Point_2> solution;
	std::copy(solution_set.begin(), solution_set.end(), std::back_inserter(solution));
	//std::cout<< "in independency"<< std::endl;
	bool inConflict;
	for (auto e : solution) {
		inConflict = isConflicts(solution, e);
		if (inConflict) {
			return false;
		}
		assert(!inConflict);
	}
	//std::cout<< "out independency"<< std::endl;
	return true;
}

bool vectorContain(vector<Point_2>& v, Point_2 x) {
	if (std::find(v.begin(), v.end(), x) != v.end()) {
		/* v contains x */
		return true;
	}
	else {
		return false;
		/* v does not contain x */
	}
}

// Compares two label_point according to label length.



#endif
//!DEBUG---------------debug------------------------
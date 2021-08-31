#include "simulation.h"
template<class S>
void Simulation<S>:: set() {
	auto t0 = std::chrono::high_resolution_clock::now();
	
	problem_ref = make_shared<Problem>();
	problem_ref->set();
	solver.set(problem_ref);

	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds d = std::chrono::duration_cast<std::chrono::nanoseconds>(fs);
	measures.addElement("setupTime", std::to_string(d.count()));
}
template<class S>
void Simulation<S>::set_rect() {
auto t0 = std::chrono::high_resolution_clock::now();


	rect_problem_ref = make_shared<rect_problem>();
	rect_problem_ref->set();
	solver.set(rect_problem_ref);
	for (const auto& e : rect_problem_ref->pVector) {
		points_y.insert(e.y());
		points_x.insert(e.x());
		points_x.insert(e.x() + e.width_single);
	}

	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds  d = std::chrono::duration_cast<std::chrono::nanoseconds>(fs);
	measures.addElement("setupTime", std::to_string(d.count()));
}

template<class S>
void Simulation<S>::add_point(const Point_2& p) {
//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
	std::cout << "add point: ";
	print_point(p);
	std::cout << std::endl;
#endif

	points_y.insert(p.y());
	points_x.insert(p.x());
	points_x.insert(p.x() + label_width);
//xPRINT---------------PRINT-------------------------
	auto t0 = std::chrono::high_resolution_clock::now();
	solver.add_point(p);
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds  d = std::chrono::duration_cast<std::chrono::nanoseconds>(fs);
	measures.addElement("updateTime", std::to_string(d.count()));
	measures.addElement("updateSize", std::to_string(solver.get_solution_size()));
#ifdef DYNAMIS_DEBUG
	debug();
#endif
};
template<class S>
void Simulation<S>::delete_point(size_t index) {
//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
	std::cout << "delete point with index " << index<<": " ;
	print_point(problem_ref->pVector[index]);
	std::cout << std::endl;
#endif
//xPRINT---------------PRINT-------------------------
	auto t0 = std::chrono::high_resolution_clock::now();
	solver.delete_point(index);
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds  d = std::chrono::duration_cast<std::chrono::nanoseconds>(fs);
	measures.addElement("updateTime", std::to_string(d.count()));
	measures.addElement("updateSize", std::to_string(solver.get_solution_size()));
#ifdef DYNAMIS_DEBUG
	debug();
#endif
};

template<class S>
void Simulation<S>::add_point_rect(const label_point& p) {
	//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
	std::cout << "add point: ";
	print_point(p);
	std::cout << std::endl;
#endif
	points_y.insert(p.y());
	points_x.insert(p.x() + p.width_single);
	points_x.insert(p.x());
	//xPRINT---------------PRINT-------------------------
	auto t0 = std::chrono::high_resolution_clock::now();
	
	solver.add_point_rect(p);

	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds  d = std::chrono::duration_cast<std::chrono::nanoseconds>(fs);
	measures.addElement("updateTime", std::to_string(d.count()));
	measures.addElement("updateSize", std::to_string(solver.get_solution_size_rect()));

#ifdef DYNAMIS_DEBUG
	debug();
#endif
};
template<class S>
void Simulation<S>::delete_point_rect(size_t index) {
	//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
	std::cout << "delete point with index " << index << ": ";
	print_point(rect_problem_ref->pVector[index]);
	std::cout<< std::endl;
#endif
	//xPRINT---------------PRINT-------------------------
	points_y.erase(rect_problem_ref->pVector[index].y());
	points_x.erase(rect_problem_ref->pVector[index].x());
	auto t0 = std::chrono::high_resolution_clock::now();

	solver.delete_point_rect(index);

	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds  d = std::chrono::duration_cast<std::chrono::nanoseconds>(fs);
	measures.addElement("updateTime", std::to_string(d.count()));
	measures.addElement("updateSize", std::to_string(solver.get_solution_size_rect()));

#ifdef DYNAMIS_DEBUG
	debug();
#endif
};
template<class S>
void Simulation<S>::ratio_addition() {
	//	std::cout<< "in ratioAddition "<< std::endl;
	std::set<Point_2> points;
	int changeSize = problem_ref->point_size() * changeRatio;
	for (size_t i = 0; i < changeSize;  i++) {
		Point_2 e = rand_point();
		add_point(e);
	}
};
template<class S>
void Simulation<S>::ratio_addition_rect() {
	//	std::cout<< "in ratioAddition "<< std::endl;
	std::set<Point_2> points;
	int changeSize = rect_problem_ref->point_size() * changeRatio;
	for (size_t i = 0; i < changeSize; i++) {
		label_point e = rand_point_rect();
		add_point_rect(e);
	}
};

template<class S>
inline bool Simulation<S>::check_valid(const Point_2& p) {
	// grid & line
	double x_factor = p.x() / label_width;
	double intpart; 
	double fractpart = modf(x_factor, &intpart);
	if (fractpart < 0.01) return false;
	if (fractpart > 0.49 && fractpart < 0.51) return false;

	double y_factor = p.y() / label_height;
	fractpart = modf(y_factor, &intpart);
	if (fractpart < 0.01) return false;
	if (fractpart > 0.49 && fractpart < 0.51) return false;

	return true;
}


template<class S>
Point_2 Simulation<S>::rand_point() {
	while(true){
		double x = fRand(label_width, width - label_width);
		double y = fRand(label_height, height - label_height);
		Point_2 test(x, y);
			// graph and ors
		if(problem_ref->look_up(test) != -1) continue;
		if (check_valid(test)) {
			return test;
		}
	}
};

template<class S>
label_point Simulation<S>::rand_point_rect() {
	while (true) {
		double x = fRand(label_width, width - label_width);
		double y = fRand(label_height, height - label_height);
		double f = Rand(3, 10);
		label_point test(x, y,f);
		if (look_up(test.x(), test.y()) == true) continue;
		if (check_valid(test)) {
			return test;
		}
	}
};

template<class S>
void Simulation<S>::ratio_deletion() {
	size_t size = problem_ref->point_size()-1;
	size_t number = size * changeRatio;
	size_t index = 0;
	for (size_t i = 0; i < number; i ++) {
		index = Rand(0, size);
		delete_point(index);
		size--;
	}
};
template<class S>
void Simulation<S>::ratio_deletion_rect() {
	size_t size = rect_problem_ref->point_size() - 1;
	size_t number = size * changeRatio;
	size_t index = 0;
	for (size_t i = 0; i < number; i++) {
		index = Rand(0, size);
		delete_point_rect(index);
		size--;
	}
};
template<class S>
void Simulation<S>::ratio_mixture() {
	int changeSize = problem_ref->point_size() * changeRatio;
	for (int i = 0; i < changeSize; i++) {
		if (bRand()) {
			Point_2 e = rand_point();
			add_point(e);
		}

		else {
			size_t size = problem_ref->point_size()-1;
			size_t index = Rand(0, size);
			delete_point(index);
		}
	}
};

template<class S>
void Simulation<S>::ratio_mixture_rect() {
	int changeSize = rect_problem_ref->point_size() * changeRatio;
	for (int i = 0; i < changeSize; i++) {
		if (bRand()){
			label_point e = rand_point_rect();
			add_point_rect(e);
		}

		else {
			size_t size = rect_problem_ref->point_size() - 1;
			size_t index = Rand(0, size);
			delete_point_rect(index);
		}
	}
};



template<class S>
void Simulation<S>::run( )
{
	string appendix;
	appendix += "-";
	appendix += std::to_string(changeRatio);
	appendix += "-";
	appendix += modType;
	appendix += "-";
	appendix += algoType;
	appendix += std::to_string(param_k);
	if (greedy_v) {
		appendix += "_greedy";
	}
	appendix += ".json";
	std::cout << appendix << std::endl;

	auto t0 = std::chrono::high_resolution_clock::now();
	solver.init();
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds d = std::chrono::duration_cast<std::chrono::nanoseconds>(fs);
	measures.addElement("initTime", std::to_string(d.count()));
	measures.addElement("initSize", std::to_string(solver.get_solution_size()));


#ifdef DYNAMIS_DEBUG
	debug();
#endif
	ratioChange();
	outputMeasure(appendix.c_str());
}


template<class S>
void Simulation<S>::run_rect()
{

	string appendix;
	appendix += "-";
	appendix += std::to_string(changeRatio);
	appendix += "-";
	appendix += modType;
	appendix += "-";
	appendix += algoType;
	appendix += std::to_string(param_k);
	if (greedy_v) {
		appendix += "_greedy";
	}
	if (xspecial_v) {
		appendix += "_greG";
	}
	appendix += "-rect.json";
	std::cout << appendix << std::endl;


	auto t0 = std::chrono::high_resolution_clock::now();
	solver.init();
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds d = std::chrono::duration_cast<std::chrono::nanoseconds>(fs);
	measures.addElement("initTime", std::to_string(d.count()));
	measures.addElement("initSize", std::to_string(solver.get_solution_size_rect()));


#ifdef DYNAMIS_DEBUG
	debug();
#endif
	ratioChange_rect();

	outputMeasure(appendix.c_str());

}


template<class S>
void Simulation<S>::add_point_recompute(const Point_2& p) {
	//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
	std::cout << "add point: ";
	print_point(p);
	std::cout << std::endl;
#endif

	points_y.insert(p.y());
	points_x.insert(p.x());

	//xPRINT---------------PRINT-------------------------
	auto t0 = std::chrono::high_resolution_clock::now();
	solver.add_point_recompute(p);
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds  d = std::chrono::duration_cast<std::chrono::nanoseconds>(fs);
	measures.addElement("updateTime", std::to_string(d.count()));
	measures.addElement("updateSize", std::to_string(solver.get_solution_size()));
#ifdef DYNAMIS_DEBUG
	debug();
#endif
};
template<class S>
void Simulation<S>::delete_point_recompute(size_t index) {
	//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
	std::cout << "delete point with index " << index << ": ";
	print_point(problem_ref->pVector[index]);
	std::cout << std::endl;
#endif
	//xPRINT---------------PRINT-------------------------
	auto t0 = std::chrono::high_resolution_clock::now();
	solver.delete_point_recompute(index);
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds  d = std::chrono::duration_cast<std::chrono::nanoseconds>(fs);
	measures.addElement("updateTime", std::to_string(d.count()));
	measures.addElement("updateSize", std::to_string(solver.get_solution_size()));
#ifdef DYNAMIS_DEBUG
	debug();
#endif
};

template<class S>
void Simulation<S>::add_point_rect_recompute(const label_point& p) {
	//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
	std::cout << "add point: ";
	print_point(p);
	std::cout << std::endl;
#endif

	points_y.insert(p.y());
	points_x.insert(p.x() + p.width_single);
	points_x.insert(p.x());
	//xPRINT---------------PRINT-------------------------
	auto t0 = std::chrono::high_resolution_clock::now();

	solver.add_point_rect_recompute(p);

	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds  d = std::chrono::duration_cast<std::chrono::nanoseconds>(fs);
	measures.addElement("updateTime", std::to_string(d.count()));
	measures.addElement("updateSize", std::to_string(solver.get_solution_size_rect()));
	points_y.insert(p.y());
	points_x.insert(p.x());
	points_x.insert(p.x() + p.width_single);
#ifdef DYNAMIS_DEBUG
	debug();
#endif
};
template<class S>
void Simulation<S>::delete_point_rect_recompute(size_t index) {
	//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
	std::cout << "delete point with index " << index << ": ";
	print_point(rect_problem_ref->pVector[index]);
	std::cout << std::endl;
#endif
	//xPRINT---------------PRINT-------------------------
	points_y.erase(rect_problem_ref->pVector[index].y());
	points_x.erase(rect_problem_ref->pVector[index].x());
	auto t0 = std::chrono::high_resolution_clock::now();

	solver.delete_point_rect_recompute(index);

	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds  d = std::chrono::duration_cast<std::chrono::nanoseconds>(fs);
	measures.addElement("updateTime", std::to_string(d.count()));
	measures.addElement("updateSize", std::to_string(solver.get_solution_size_rect()));

#ifdef DYNAMIS_DEBUG
	debug();
#endif
};
template<class S>
void Simulation<S>::ratio_addition_recompute() {
	//	std::cout<< "in ratioAddition "<< std::endl;
	std::set<Point_2> points;
	int changeSize = problem_ref->point_size() * changeRatio;
	for (size_t i = 0; i < changeSize; i++) {
		Point_2 e = rand_point();
		add_point_recompute(e);
	}
};
template<class S>
void Simulation<S>::ratio_addition_rect_recompute() {
	//	std::cout<< "in ratioAddition "<< std::endl;
	std::set<Point_2> points;
	int changeSize = rect_problem_ref->point_size() * changeRatio;
	for (size_t i = 0; i < changeSize; i++) {
		label_point e = rand_point_rect();
		add_point_rect_recompute(e);
	}
};



template<class S>
void Simulation<S>::ratio_deletion_recompute() {
	size_t size = problem_ref->point_size() - 1;
	size_t number = size * changeRatio;
	size_t index = 0;
	for (size_t i = 0; i < number; i++) {
		index = Rand(0, size);
		delete_point_recompute(index);
		size--;
	}
};
template<class S>
void Simulation<S>::ratio_deletion_rect_recompute() {
	size_t size = rect_problem_ref->point_size() - 1;
	size_t number = size * changeRatio;
	size_t index = 0;
	for (size_t i = 0; i < number; i++) {
		index = Rand(0, size);
		delete_point_rect_recompute(index);
		size--;
	}
};
template<class S>
void Simulation<S>::ratio_mixture_recompute() {
	int changeSize = problem_ref->point_size() * changeRatio;
	for (int i = 0; i < changeSize; i++) {
		if (rand() % 2 == 0) {
			Point_2 e = rand_point();
			add_point_recompute(e);
		}

		else {
			size_t size = problem_ref->point_size() - 1;
			size_t index = Rand(0, size);
			delete_point_recompute(index);
		}
	}
};

template<class S>
void Simulation<S>::ratio_mixture_rect_recompute() {
	int changeSize = rect_problem_ref->point_size() * changeRatio;
	for (int i = 0; i < changeSize; i++) {
		if (rand() % 2 == 0) {
			label_point e = rand_point_rect();
			add_point_rect_recompute(e);
		}

		else {
			size_t size = rect_problem_ref->point_size() - 1;
			size_t index = Rand(0, size);
			delete_point_rect_recompute(index);
		}
	}
};



template<class S>
void Simulation<S>::run_recompute()
{
	string appendix;
	appendix += "-";
	appendix += std::to_string(changeRatio);
	appendix += "-";
	appendix += modType;
	appendix += "-";
	appendix += algoType;
	appendix += std::to_string(param_k);
	if (greedy_v) {
		appendix += "_greedy";
	}
	appendix += "_recompute.json";
	std::cout << appendix << std::endl;


	auto t0 = std::chrono::high_resolution_clock::now();
	solver.init();
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds d = std::chrono::duration_cast<std::chrono::nanoseconds>(fs);
	measures.addElement("initTime", std::to_string(d.count()));
	measures.addElement("initSize", std::to_string(solver.get_solution_size()));


#ifdef DYNAMIS_DEBUG
	debug();
#endif
	ratioChange_recompute();
	outputMeasure(appendix.c_str());
}


template<class S>
void Simulation<S>::run_rect_recompute()
{

	string appendix;
	appendix += "-";
	appendix += std::to_string(changeRatio);
	appendix += "-";
	appendix += modType;
	appendix += "-";
	appendix += algoType;
	appendix += std::to_string(param_k);
	if (greedy_v) {
		appendix += "_greedy";
	}
	appendix += "-rect_recompute.json";
	std::cout << appendix << std::endl;


	auto t0 = std::chrono::high_resolution_clock::now();
	solver.init();
	auto t1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration< double> fs = t1 - t0;
	std::chrono::nanoseconds d = std::chrono::duration_cast<std::chrono::nanoseconds>(fs);
	measures.addElement("initTime", std::to_string(d.count()));
	measures.addElement("initSize", std::to_string(solver.get_solution_size_rect()));


#ifdef DYNAMIS_DEBUG
	debug();
#endif
	ratioChange_rect_recompute();

	outputMeasure(appendix.c_str());

}








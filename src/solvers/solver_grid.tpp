template<typename T>
void solver_grid<T>::set(const shared_ptr<Problem>& P){
	Solver::set(P);
	numberV = ceil(width / label_width) +1;
	numberH = ceil(height / label_height) +1;
	gridP = vector<vector<T>>(numberH, vector<T>(numberV, T{}));

	counter = vector<vector<int>>(numberH, vector<int>(param_k + 1, 0));
	mark = vector<int>(numberH, -1);


	int x, y;
	for (auto p : problem_ref->pVector) {
		x = ceil((p.x() - h_label_width) / label_width);
		y = ceil((p.y() - h_label_height) / label_height);
		insert_in_container(gridP[y][x], p);
	}
};

template<typename T>
void solver_grid<T>::init() {
	int max_index = -1;
	int max_value = -1;
	int index = -1;

	for (int i = 0; i < numberH; i++) {
		for (auto e : counter[i]) {
			index++;
			if (e > max_value) {
				max_index = index;
				max_value = e;
			}
		}

		mark[i] = max_index;
		if (i % 2 == 0) sumE += max_value;
		else sumO += max_value;
		max_index = -1;
		max_value = -1;
		index = -1;

	}
	markE = (sumE > sumO);
}


template<typename T>
void solver_grid<T>::recompute() {
	counter = vector<vector<int>>(numberH, vector<int>(param_k + 1, 0));
	mark = vector<int>(numberH, -1);
	sumE = 0;
	sumO = 0;
}


template<typename T>
bool solver_grid<T>::add_point(const Point_2& p) {
	Solver::add_point(p);
	int x = ceil((p.x() - h_label_width) / label_width);
	int y = ceil((p.y() - h_label_height) / label_height);
	insert_in_container(gridP[y][x], p);
	return true;
};


template<typename T>
bool solver_grid<T>::delete_point(size_t index) {
	Point_2 p = problem_ref->pVector[index];
	int x = ceil((p.x() - h_label_width) / label_width);
	int y = ceil((p.y() - h_label_height) / label_height);
	remove_in_container(gridP[y][x], p);
	Solver::delete_point(index);
	return true;
};


//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
template<typename T>
void solver_grid<T>::print(){
	Solver::print();
	std::cout << "numV:" << numberV << std::endl;
	std::cout << "numH:" << numberH << std::endl;
	//gridP
	std::cout << "The grid:" << std::endl;
	int i = 0;
	int j = 0;
	for (size_t i = 0; i < numberH; i++) {
		for (size_t j = 0; j < numberV; j++) {
			for (auto v : gridP[i][j]) {
				std::cout << "(" << v.x() << " , " << v.y() << ")  ";
			}
		}
		std::cout << std::endl;
	}
	//solution
	std::cout << "The counter:" << std::endl;
	for (const auto& e : counter) {
		for (const auto& i : e) {
			std::cout << i<< " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout << "mark: ";
	for (auto e : mark) {
		std::cout << e << "    ";
	}
	std::cout << std::endl;

	std::cout << "sumE:" << sumE << std::endl;
	std::cout << "sumO:" << sumO << std::endl;

	std::cout << "markE:" << markE << std::endl;
};
template<typename T>
void solver_grid<T>::draw(cairo_t* cr) {
	Solver::draw(cr);
	cairo_set_source_rgb(cr, 0.0, 1.0, 1.0);
	// draw lines
	double x, y;
	double end_x = width * 1.1;
	double	end_y = height * 1.1;
	for (int i = 0; i < numberV; i++) {
		cairo_stroke(cr);
		x = i * label_width + 0.1 * width;
		cairo_move_to(cr, x, 0.1 * height);
		cairo_line_to(cr, x, end_y);
		cairo_stroke(cr);
	}
	cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
	for (int i = 0; i < numberH; i++) {
		cairo_stroke(cr);
		y = end_y - i * label_height;
		cairo_move_to(cr, 0.1 * width, y);
		cairo_line_to(cr, end_x, y);
		cairo_stroke(cr);
	};
};
#endif
//xPRINT---------------PRINT-------------------------

//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
template<typename T>
void solver_grid<T>::debug() {
	Solver::debug();
	//test gridP
	size_t size = 0;
	for (int i = 0; i < numberH; i++) {
		for (int j = 0; j < numberV; j++) {
			for (auto v : gridP[i][j]) {
				bool contain = vectorContain(problem_ref->pVector, v);
				assert(contain);
				size++;
			}
		}
	}
	assert(problem_ref->pVector.size() == size);

	//test marks
	int sE = 0;
	int sO = 0;
	for (int i = 0; i < numberH; i++) {
		// 	test mark;
		for (const auto& m : counter[i]) {
			if (m > counter[i][mark[i]]) {
				std::cout << "i:" << i << std:: endl;
				std::cout << "m:" << m << std:: endl;
				std::cout << "mark[i]:" << mark[i] << std::endl;
				std::cout << "counter[i][mark[i]]:" << counter[i][mark[i]] << std::endl;
				
			
			
			}
			assert(m <= counter[i][mark[i]]);
		}
		// test sumE;
		//test sumO
		if (i % 2 == 0) sE += counter[i][mark[i]];
		else sO += counter[i][mark[i]];
	}

	assert(sumE == sE);
	assert(sumO == sO);

	if (markE) assert(!(sumE < sumO));
	else assert(!(sumE > sumO));
};
#endif
//!DEBUG---------------debug------------------------
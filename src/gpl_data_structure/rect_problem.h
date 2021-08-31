#pragma once
#include "problem.h"




class rect_problem {
public:
	rect_problem() {};
	virtual void set();

	vector<label_point> pVector;
	inline size_t point_size() const {
		return pVector.size();
	};
	int look_up(const label_point& Point) const;


	virtual void add_point(const label_point& p);
	void delete_point(size_t index);


	virtual void readFile();

	//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
	void draw(cairo_t* cr);
	void print();
#endif
	//xPRINT---------------PRINT-------------------------

	//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
	void debug();

#endif
	//!DEBUG---------------debug------------------------

protected:
	lookupTable luTable;

	virtual void read_label_info(char* str);

private:
	inline bool look_up_x(double x) {
		if (points_x.find(x) != points_x.end()) {
			return true;
		}
		return false;
	}


	inline bool look_up_y(double y) {
		if (points_y.find(y) != points_y.end()) {
			return true;
		}
		return false;
	}

	inline 	bool check_valid(const label_point& p) {
		if (look_up_y(p.y()) == true) return false;
		if (look_up_x(p.x()) == true) return false;
		if (look_up_x(p.x() + p.width_single) == true) return false;


		// grid & line
		double x_factor = p.x()/ label_width;
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

};
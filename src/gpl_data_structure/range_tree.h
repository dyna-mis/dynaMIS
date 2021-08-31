#pragma once
#include "miscellaneous.h"
struct Node {
	double spliter = 0;
	interval spliter_interval;
	Node* left = NULL;
	Node* right = NULL;
	std::set<interval, cmp_second> secondary_set;

	Node() {};
	void set() {
	}
	inline void add_element(const interval& I) {
		secondary_set.insert(I);

	}
	inline 	interval search(double left) {
		if(secondary_set.size() == 0) return interval(DBL_MAX, DBL_MAX);
		auto it = secondary_set.begin();
		if ((*it).first > left) {
			return *it;
		}
		else {
			return interval(DBL_MAX, DBL_MAX);
		}
	
	};
	inline size_t delete_element(const interval& I) {
		if (I == spliter_interval) {
			spliter_interval = interval(I.first, DBL_MAX);
		}
		return secondary_set.erase(I);
	}
	//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
	void print() {
		std::cout << "spliter: " << spliter << std:: endl;
		std::cout << "spliter_interval: ";
		print_point(spliter_interval);
		std::cout << "secondary_set: ";
		for (const auto& e : secondary_set) {
			print_point(e);
		}
	
	};
#endif
	//xPRINT---------------PRINT-------------------------

	//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
	void debug(std::set<interval, lexico_Interval> intervals, double upper_bound, double lower_bound) {
		assert(spliter >= 0);
		assert(spliter_interval.first == spliter);
		assert(spliter_interval.second == DBL_MAX || secondary_set.find(spliter_interval) != secondary_set.end());
		for (const auto& i : intervals) {
			if (i.first <= lower_bound || i.first >= upper_bound) continue;
			else {
				assert(secondary_set.find(i) != secondary_set.end());
			}
		}
	};
#endif
	//!DEBUG---------------debug------------------------
};


class range_tree {
public:
	range_tree() {};
	~range_tree(){
		destroy_tree(root);
	};

	void set(const std::vector<interval>& interval_vector);
	interval search(double left);

	void add_element(const interval& I);
	void delete_element(const interval& I);
	//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
	void print() {
		print(root);
	
	};
#endif
	//xPRINT---------------PRINT-------------------------

	//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
	void debug(std::set<interval, lexico_Interval> intervals) {
		if (root != NULL) {
			debug(root, intervals, DBL_MAX, -DBL_MIN);
		}
	};
#endif
	//!DEBUG---------------debug------------------------


protected:

	Node* root = NULL;


	void destroy_tree(Node* leaf);
	void add_element(const interval& I, Node* leaf);
	void delete_element(const interval& I, Node* leaf);
	interval search(double left, Node* leaf);
	void set(const std::vector<interval>& interval_vector, Node* leaf, size_t s_index, size_t t_index);


	//xPRINT+++++++++++++++PRINT+++++++++++++++++++++++++
#ifdef DYNAMIS_PRINT
	void print(Node* leaf) {
		if (leaf->left != NULL) {
			print(leaf->left);
		}

		if (leaf->right != NULL) {
			print(leaf->right);
		}

	};
#endif
	//xPRINT---------------PRINT-------------------------

	//!DEBUG+++++++++++++++debug++++++++++++++++++++++++
#ifdef DYNAMIS_DEBUG
	void debug(Node* leaf, std::set<interval, lexico_Interval> intervals, double upper, double lower) {
		leaf->debug(intervals, upper, lower);
		if (leaf->left != NULL) {
			debug(leaf->left, intervals, leaf->spliter, lower);
		}

		if (leaf->right != NULL) {
			debug(leaf->right, intervals, upper, leaf->spliter);
		}
	};
#endif
	//!DEBUG---------------debug------------------------


};
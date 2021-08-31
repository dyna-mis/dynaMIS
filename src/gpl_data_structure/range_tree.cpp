#include "range_tree.h"

void range_tree::destroy_tree(Node* leaf) {
	if (leaf != NULL) {
		destroy_tree(leaf->left);
		destroy_tree(leaf->right);
		delete leaf;
	}
}


void range_tree::add_element(const interval& I) {
	if (root != NULL) {
		add_element(I, root);
	}
	else {
		root = new Node;
		root->spliter = I.first;
		root->spliter_interval = I;
		root->add_element(I);
	}
}
// leaf is not NULL
void range_tree::add_element(const interval& I, Node* leaf) {
	leaf->add_element(I);

	double key = I.first;
	if (key < leaf->spliter) {
		if (leaf->left != NULL) {
			add_element(I, leaf->left);
		}
		else {
			leaf->left = new Node;
			leaf->left->spliter = key;
			leaf->left->spliter_interval = I;
			leaf->left->add_element(I);
		}
	}
	else if (key >= leaf->spliter) {
		if (leaf->right != NULL) {
			add_element(I, leaf->right);
		}
		else {
			leaf->right = new Node;
			leaf->right->spliter = key;
			leaf->right->spliter_interval = I;
			leaf->right->add_element(I);
		}
	}

}
// assume the vector is sorted
//not empty
void range_tree::set(const std::vector<interval>& interval_vector)
{
	root = new Node;
	set(interval_vector, root, 0, interval_vector.size() - 1);
}
// assume the vector is sorted
// the pointer exists
void range_tree::set(const std::vector<interval>& interval_vector, Node* leaf, size_t s_index, size_t t_index)
{
	for (size_t i = s_index; i <= t_index; i++) {
		leaf->add_element(interval_vector[i]);
	}

	size_t t_size = t_index - s_index + 1;
	size_t middle = t_size / 2 + s_index;
	interval sp = interval_vector[middle];
	leaf->spliter = sp.first;
	leaf->spliter_interval = sp;
	if (middle > s_index) {
		leaf->left = new Node;
		set(interval_vector, leaf ->left, s_index, middle - 1);
	}
	if (middle < t_index) {
		leaf->right = new Node;
		set(interval_vector, leaf->right, middle + 1, t_index);

	}


}




//key: left bound
interval range_tree::search(double bound) {
	if (root != NULL) {
		return search(bound, root);
	}
	else {
		return interval(DBL_MAX, DBL_MAX);
	}
}

// assert leaf is not NULL
interval range_tree::search(double bound, Node* leaf) {
	interval I = leaf->search(bound);
	if (I.second != DBL_MAX) {
		return I;
	}

	if (leaf->spliter_interval.first > bound) {
		I = leaf->spliter_interval;
	}
	if (leaf->left != NULL && bound < leaf->spliter) {
		interval I_left = search(bound, leaf->left);
		if (I_left.second != DBL_MAX && I_left.second < I.second) {
			I = I_left;
		}
	}
	if (leaf->right != NULL) {
		interval I_right = search(bound, leaf->right);
		if (I_right.second != DBL_MAX && I_right.second < I.second) {
			I = I_right;
		}
	}
	return I;
}

void range_tree::delete_element(const interval& I) {
	if (root != NULL) {
		delete_element(I, root);
	}
}
// leaf is not NULL
void range_tree::delete_element(const interval& I, Node* leaf) {
	size_t in = leaf->delete_element(I);
	if (in == 0) return;
	double key = I.first;
	if (key < leaf->spliter && leaf->left != NULL) {
		delete_element(I, leaf->left);
	}
	else if (key >= leaf->spliter && leaf->right != NULL) {
		delete_element(I, leaf->right);
	}
}

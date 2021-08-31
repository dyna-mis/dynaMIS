
#pragma once

#include "solver_graph.h"
#include "graph.h"

class wmis_solver: public solver_graph
{

	public:
		wmis_solver() {};
		//SolveStatus init() override;
		void init();

		void recompute();

		bool add_point(const Point_2& p) override;
		bool delete_point(size_t index) override;


		bool add_point_rect(const label_point& p) override;
		bool delete_point_rect(size_t index) override;


		bool add_point_recompute(const Point_2& p) override;
		bool delete_point_recompute(size_t index) override;
		bool add_point_rect_recompute(const label_point& p) override;
		bool delete_point_rect_recompute(size_t index) override;



	protected:
		void outputDIMACS();
		void readWCNF();

		void set_solution() override;

};
//#include "wmis_solver.tpp";




#include "wmis_solver.h"
std::string maxHS_PATH = "/home1/guangping/mis_solvers/maxhs ";



bool wmis_solver::add_point(const Point_2& p) {
    solver_graph::add_point(p);
    init();
    return true;
};
bool wmis_solver::delete_point(size_t index) {
    solver_graph::delete_point(index);
    init();
    return true;
};


bool wmis_solver::add_point_rect(const label_point& p) {
    solver_graph::add_point_rect(p);
    init();
    return true;
};
bool wmis_solver::delete_point_rect(size_t index) {
    solver_graph::delete_point_rect(index);
    init();
    return true;
};

bool wmis_solver::add_point_recompute(const Point_2& p) {
    solver_graph::add_point(p);
    recompute();
    return true;
};
bool wmis_solver::delete_point_recompute(size_t index) {
    solver_graph::delete_point(index);
    recompute();
    return true;
};


bool wmis_solver::add_point_rect_recompute(const label_point& p) {
    solver_graph::add_point_rect(p);
    recompute();
    return true;
};
bool wmis_solver::delete_point_rect_recompute(size_t index) {
    solver_graph::delete_point_rect(index);
    recompute();
    return true;
};






void wmis_solver::init(){
    outputDIMACS();
    string o = maxHS_PATH + tmp_dictionary + "/" + get_file_name(file) + "_wmis.tmp>"
        + tmp_dictionary + "/" + get_file_name(file) + "_wmis_solution.tmp";
    if (EXIT_SUCCESS != system(o.c_str()))
    {
        assert(false);
        exit(1);
    }
    //system("/home1/guangping/dynaMIS/maxhs /home1/guangping/dynaMIS/dimacs.txt >/home1/guangping/dynaMIS/b.txt");
    readWCNF();
};
void wmis_solver::recompute() {
    init();
};

void wmis_solver::outputDIMACS(){
  ofstream outdata;
  int cost = 1;
  size_t numV = conflict_graph.get_vertex_size();
  int top = 2* numV;
  string filename = tmp_dictionary + "/" + get_file_name(file)+"_wmis.tmp";
  outdata.open (filename.c_str());
  if(!outdata){
    // file couldn't be opened
    cerr << "Error: file could not be opened" << endl;
    exit(1);
  }
  outdata << "c test"<<endl;
  int eSize = conflict_graph.get_edge_size();
  outdata<<"p wcnf "<< numV -1<<" "<< eSize <<" "<<top<<endl;
  for (int i =0 ; i < numV; i++) {
	  std::vector<double> output(conflict_graph.adList[i].size());
	  std::copy(conflict_graph.adList[i].begin(), conflict_graph.adList[i].end(), output.begin());
	  sort(output.begin(), output.end());
    for(const auto &q :output){
        if(i < q){
         outdata<<top<<" "<<-(i+1)<<" "<< -(q+1)<<" "<< 0<< endl;
        }
    }
  }
   for (unsigned int i =0 ; i < numV; i++) {
      outdata<<cost<<" "<<i+1<<" "<< 0<< endl;
   }
  outdata.close();
}




void wmis_solver::readWCNF(){
  std::fill(mVector.begin(), mVector.end(), 0);
  string WCNF = tmp_dictionary + "/" + get_file_name(file) + "_wmis_solution.tmp";
  ifstream indata;
  indata.open(WCNF.c_str());
  if(!indata){
  // file couldn't be opened
      cerr<< "Error: file could not be opened" << endl;
      exit(1);
  }
  string line;
  char head;
  while (getline(indata, line)){
    if(line.empty()) continue;
		head =line.at(0);
		if(head == 'v'){
			break;
		}
  }
  int i;
  char* str = strdup(line.c_str());
  const char s[2] = " ";
  char* token = strtok(str, s);
  token = strtok(NULL, s);
  while(token != NULL){
    i = atoi(token);
    if(i>0){
    mVector[i-1] = 1;
    }
    token = strtok(NULL, s);
  }
}

void wmis_solver::set_solution() {
};
/*
 * main.cpp
 *
 *  Created on: Nov 24, 2019
 *      Author: Guangping
 */
#include "main.h"
void parseInitOptions(int argc, char *argv[]){
	struct option longopts[] ={{"help",no_argument,0,'h' },
							   {"output", no_argument,0, 'o'},
							   {"algorithm", required_argument,0, 'a'},
							   {"sigma", required_argument,0, 's'},
							   {"kigma", required_argument,0, 'k'},
	                           //the opt array must terminate with a all zero element.
							   {0,0,0,0}
	                          };
	int result;
	int option_index = 0;

	while ((result = getopt_long(argc, argv, "hk:oa:s:",longopts,&option_index)) != -1){
		switch (result) {
		case 'h':
			printInitUsage();
			exit(0);
			break;
		case 's':
			sigma = atof(optarg);
			break;
		case 'a':
			algoType =  optarg;
			break;
		case 'o':
			outputFlag =  true;
			break;
		case 'k':
			k =  atoi(optarg);
			break;
		default:
			printInitUsage();
			exit(0);
		}
	}
	if(optind >= argc){
		std::cout<<RED<<"ERROR"<<RESET<<": FILE is not given.";
		printInitUsage();
		exit(0);
	}
	file = *(argv + optind);
}
void printInitOptions(){
	printf("dynaMIS Initialization options \n");
	cout<< "file:"<<file<<endl;
	cout<<"output the solution:"<< outputFlag<<endl;
	cout<<"sigma:" << sigma<< endl;
	if (algoType.compare("graph" )== 0){
		return;
	}
	cout<<"algorithm:" << algoType<<"-"<<k<< endl;
}
/*print the manual of the program*/
void printInitUsage(){
	cout<<"\n--------dynaMIS, a dynamic MIS solver by "<<BLUE<< "TUWIEN"<<RESET<<" -----------------\n";
	printf("\nautor: Sujoy Bhore, Guangping Li and Martin Nöllenburg \n");
	printf("Human-centered Algorithm Engineering: Graph and Map Visualization\n");
	printf("Research Project supported by the Austrian Science Fund(FWF), grant P 31119\n");
	printf("https://www.ac.tuwien.ac.at/research/humalgo/\n");

	printf("\n-------------------Usage--------------------------------------------------------\n");
	printf("./dynaMIS  <instance> [options]\n");

	printf("\n-------------------Options------------------------------------------------------\n");
	printf("   --output, -o : output the solution\n");
	printf("   --help, -h : output this help\n");
	printf("   --algorithm, -a : algorithm in use (see options below)\n");
	printf("   --sigma, -s : width of a square\n");
	printf("---------------------------------------------------------------------------------\n");
	printf("\n-------------------algorithm options------------------------------------------------------\n");
	printf("   ors : a dynamic MIS algorithm based on orthogonal range searching \n");
	printf("   graph: a graph-based dynamic MIS algorithm\n");
	printf("   grid:a grid-based 4-approximation algorithm \n");
	printf("   gridK: : The group-shifting based algorithm. Need a extra parameter -k\n");
	printf("   line,  : stabbing-line based 2-approximation algorithm\n\n");
	printf("   *More details see our paper \"Independent Sets of Dynamic Rectangles:Algorithms and Experiments\" <ARXIV LINK>\n");
	printf("---------------------------------------------------------------------------------\n");

}
void printModUsage(){
	printf("\n-------------------Options------------------------------------------------------\n");
	printf("add <x> <y>, a : add a point(%.2f<x <%.2f,%.2f<y < %.2f)\n",h_sigma,w_bound,h_sigma,h_bound);
	printf("delete <i>, d : delete the point with index i <%d\n",number);
	printf("help, h : output this help\n");
	printf("print q to quite the program");
	printf("---------------------------------------------------------------------------------\n");
}
void mod(string modline){
   	char* str = strdup(modline.c_str());
    const char* s = " ,.-";
    double x,y;
    int index;
    char * pch;
    pch = strtok(str, s);
    if(strcmp (pch,"add") == 0 || strcmp (pch,"a") == 0){
		x =atof(strtok(NULL, s));
		y =atof(strtok(NULL, s));
		if(x >= w_bound || x < h_sigma){
			std::cout<<RED<<"ERROR"<<RESET<<": add <x> <y>, a :"<< h_sigma <<"< x < " << w_bound <<"\n";
    		return;
		}
		else{
			if(y>= h_bound){
				std::cout<<RED<<"ERROR"<<RESET<<": add <x> <y>, a : y must be <" << h_bound <<"\n";
	    		return;
			}
			else{
				p->addPoint(Point_2(x,y));
				p->printSolution();
			}
		}
    }
    else{
    	if(strcmp (pch,"delete") == 0 || strcmp (pch,"d") == 0){
    		index = atoi(strtok(NULL, s));
    		if(index >= number){
				std::cout<<RED<<"ERROR"<<RESET<<"delete <i>, d : i must be <" <<number;
	    		return;
    		}
    		std::cout<< "delete "<< index<< std::endl;
			p->subPoint_index(index);
			p->printSolution();
    	}
    	else{
    		printModUsage();
    	}
    }
}


//todo: build a dummy loop
int main(int argc, char *argv[]){
	std::cout<<"hello dynaMIS"<< std::endl;
	parseInitOptions(argc, argv);
	printInitOptions();

	//INIT
	if(algoType.compare("cg" )== 0){
	    p = new Problem_cg(sigma,file);
	}
	else{
		if(algoType.compare("rs" )== 0){
			p = new Problem_rs(sigma,file);
		}
		else{
			if(algoType.compare("sl" )== 0){
				p = new Problem_sl(sigma,file);
			}
			else{
				if(algoType.compare("gsl" )== 0){
					p = new Problem_sl_greedy(sigma,file);
				}
				else{
					if(algoType.compare("slk" )== 0){
						p = new Problem_slk(sigma,file,k);
					}
					else{
						if(algoType.compare("gslk" )== 0){
							p = new Problem_slk_greedy(sigma,file,k);
						}
						else{
							if(algoType.compare("arrow" )== 0){
								p = new Problem_arrow(sigma,file);
							}
							else{
								if(algoType.compare("garrow" )== 0){
									p = new Problem_arrow_greedy(sigma,file);
								}
								else{
									std::cout<<RED<<"ERROR"<<RESET<<": Algorithm  is not known.";
									printInitUsage();
									exit(0);
								}
							}
						}
					}
				}
			}
		}
	}
	h_sigma = 0.5* sigma;
	w_bound = p->getWidth()-h_sigma;;
	h_bound = p->getHeight()-h_sigma;;
	p->simple();
	p->printSolution();
    string line;
	number = p->getSize();
    while(true){
    	number = p->getSize();
    	printModUsage();
    	std::getline(std::cin, line);
    	if(line.compare("q")== 0) break;
    	mod(line);
//todo: size change remember
    }
	printf("Thanks for your usage of dynaMIS !");

}

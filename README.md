## <img src="dynamis_icon.gif" width="100"> Dynamic Vienna Maximum/maximal Independent Set Solver  

# Requirements
<ol>
<li>CGAL (>= 4.11.2)</li>
<li>  g++ </li>
</ol>
# Description
Our Framework contains 5 algorithms.
* ors : a dynamic MIS algorithm based on orthogonal range searching 
* graph: a graph-based dynamic MIS algorithm
* grid: a grid-based 4-approximation algorithm 
* gridK: The group-shifting based algorithm. Need a extra parameter -k
* line: stabbing-line based 2-approximation algorithm


# Installation
Compile the source with Debug/Makefile.
# Usage
./dynaMIS  <instance> [options]


--output, -o : output the solution</br>
--help, -h : output this help</br>
--algorithm, -a : algorithm in use， please using abbreviations (see description above)</br>
--sigma, -s : width of a square</br>



# Acknowledgments
[Our project "Human-Centered Algorithm Engineering: Graph and Map Visualization"](https://www.ac.tuwien.ac.at/research/humalgo/) is supported by the Austrian Science Fund (FWF) under Grant P31119.
# Reference
More details see our paper "An Algorithmic Study of Fully Dynamic Independent Sets for Map Labeling"([ARXIV LINK](https://arxiv.org/abs/2002.07611))


<script type="text/javascript" src="http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML"> </script>
## Overview

The independent set problem is a fundamental graph problem with a wide range of applications. Given a graph $$G = (V, E)$$, a set of vertices $$M \subset V$$ is independent if no two vertices in $$M$$ are adjacent in $$G$$. A maximal independent set (**MIS**) is an independent set that
is not a proper subset of any other independent set. A maximum independent set **Max-IS**
is a maximum cardinality independent set. While Max-IS is one of Karp’s 21 classic NP-complete problems, computing a MIS can easily be done by a simple greedy algorithm in
$$O(|E|)$$ time. The **MIS** problem has been studied in the context of several other prominent
problems, e.g., graph coloring, maximum matching, and vertex cover. On the
other hand, Max-IS serves as a natural model for many real-life optimization problems,
including map labeling, computer vision, information retrieval, and scheduling.


This is the open source project DynaMIS-a dynamic maximum independent solver by Institute of Logic and Computation, TUWIEN.
So far our framework contains the first deterministic algorithm for maintaining a **MIS** (and thus a $$4$$-approximate **Max-IS**) of a dynamic set of uniform rectangles with amortized sub-logarithmic update time. This breaks the natural barrier of $$\Omega(\Delta)$$ update time. We also provide a series of deterministic dynamic approximation schemes for maintain a **Max-IS**. The algorithm **GRID** that maintains a $$4$$-approximate **Max-IS** with $$O(1)$$ update time. Its subsequent algorithm **GRID-K** establishs the trade-off between approximation quality $$2(1+\frac{1}{k})$$ and update time  $$O(k^2\log n)$$. Our framework includes an algorithm that maintains unit interval graphs. With this algorithm,  a $$2$$-approximate **Max-IS** for dynamic sets of unit rectangles with $$O(\omega \log n)$$ update time can be maintained, where $$\omega$$ is the maximum size of an independent set of rectangles stabbed by any horizontal line.  

## Licence

This project is under MIT licence. 
## Reference
if you want to know more about our implemented algorithms, please refer to [our paper](https://arxiv.org/abs/2002.07611). 
Please aknowledge our work if you publish your result using our algorithms or code.

## Download
Source code (availbale in our [github repository](https://github.com/GPLi-TUWIEN/dynaMIS))

## Support
Please write us an Email if you need support
We are glad to get any comments and error reports.
A random instances generators are available upon request
## Acknowledgments
[Our project "Human-Centered Algorithm Engineering: Graph and Map Visualization"](https://www.ac.tuwien.ac.at/research/humalgo/) is supported by the Austrian Science Fund (FWF) under Grant P31119.

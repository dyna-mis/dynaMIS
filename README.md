<script type="text/javascript"
   src="http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML">
</script>
## Overview

The independent set problem is a fundamental graph problem with a wide range of applications. Given a graph $$G = (V, E)$$, a set of vertices $$M \subset V$$ is independent if no two vertices in $$M$$ are adjacent in $$G$$. A maximal independent set (MIS) is an independent set that
is not a proper subset of any other independent set. A maximum independent set (Max-IS)
is a maximum cardinality independent set. While Max-IS is one of Karp’s 21 classic NP56 complete problems, computing a MIS can easily be done by a simple greedy algorithm in
$$O(|E|)$$ time. The MIS problem has been studied in the context of several other prominent
problems, e.g., graph coloring, maximum matching, and vertex cover. On the
other hand, Max-IS serves as a natural model for many real-life optimization problems,
including map labeling, computer vision, information retrieval, and scheduling.


This is the open source project DynaMIS-a dynamic maximum independent solver by Institute of Logic and Computation, TUWIEN.
So far our framework contains the first deterministic algorithm for maintaining a MIS (and thus a §$4$§-approximate \textsc{Max-IS}) of a dynamic set of uniform rectangles with amortized sub-logarithmic update time. This breaks the natural barrier of $$\Omega(\Delta)$$ update time. We also provide a series of deterministic dynamic approximation schemes for maintain a Max-IS. The algorithm $Grid$ that maintains a $$4$$-approximate Max-IS with $$O(1)$$ update time. Its subsequent algorithm $$Grid-k$$ establishs the trade-off between approximation quality $$2(1+\frac{1}{k})$$ and update time  $$O(k^2\log n)$$, for $$k\in \mathbb{N}$$. Our framework includes an algorithm that maintains unit interval graphs. With this algorithm,  a $$2$$-approximate Max-IS for dynamic sets of unit rectangles with $$O(\omega \log n)$$ update time can be maintained, where $$\omega$$ is the maximum size of an independent set of rectangles stabbed by any horizontal line.  

## Licence

Markdown is a lightweight and easy-to-use syntax for styling your writing. It includes conventions for

```markdown
Syntax highlighted code block

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```

For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).

## Download

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/GPLi-TUWIEN/dynaMIS/settings). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

## Support

Having trouble with Pages? Check out our [documentation](https://help.github.com/categories/github-pages-basics/) or [contact support](https://github.com/contact) and we’ll help you sort it out.
## References

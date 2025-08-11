A little program to decide whether a given graph has inversion diameter at most a given integer k.

Usage:
        make
        cat <input_file> | ./compute_diam_I.native <k>

Output: all the graphs in <input_file> with inversion diameter bigger than k. On the stderr output is printed the binary string corresponding to the values on the edges of the graphs which is not reached from 0. The edges are in the lexicographic order.

Format: all graphs are in graph6 format (see https://users.cecs.anu.edu.au/~bdm/data/formats.html).

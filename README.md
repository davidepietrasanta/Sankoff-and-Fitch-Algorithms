# Sankoff-and-Fitch-Algorithms
C implementation of Sankoff and Fitch Algorithms.
Tree are coded in Newick Format, for example (F,(C,D,E)B,(W,L)K)A is [Here](https://github.com/davidepietrasanta/Sankoff-and-Fitch-Algorithms/blob/master/Tree.pdf)
Rule:
- all input must be correct
- name of unkown species must be without numbers
- character-state must be in [0,9]
- maximum cost to change state should not be greater than 134217727
- path name should not be greater than 10.000 char
- matrix files must have the number of characters before the matrix

Info:
- the output is written on Terminal and saved in a file
- all input must be correct to have a correct output

How to run the code(example):
- open the terminal
- cd into makefile directory
- make
- ./start
- 0
- newick.txt
- cost.txt
-You can use other files, just write their path.

Build documentation with doxygen:
- doxygen Doxyfile
It will build the html 

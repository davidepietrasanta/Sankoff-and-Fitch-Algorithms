# Sankoff-and-Fitch-Algorithms
C implementation of Sankoff and Fitch Algorithms.
Tree are coded in Newick Format, for example (F,(C,D,E)B,(W,L)K)A is [Here](https://github.com/davidepietrasanta/Sankoff-and-Fitch-Algorithms/blob/master/Newick%20Parser/Tree.pdf)
Rule:
- name of unkown species must be without numbers
- character-state must be in [0,9]
- maximum cost to change state should not be greater than 134217727
- path name should not be greater than 10.000 char

Info:
- the output is written on Terminal and saved in a file

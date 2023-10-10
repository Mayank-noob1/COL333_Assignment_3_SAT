# The Clique Problem :
Reference : [Wikipedia](https://en.wikipedia.org/wiki/Clique_problem)

## Problem Statement:
### Finding cliques of size k1 and k2 in a graph G:
  - Decompose graph in clauses.
  - Clauses are of three forms.
    1) Clauses of edges not present in the graph.
    2) Clauses of picking k variables out of i variables.  (Refer : [Conference Paper](mapleconference.pdf))
    3) Clauses mentioned above for both k1 and k2 size subgraphs don't share a vertex.
  - Run minisat on clauses to get the assignment.

### Finding maximal cliques in graph G:
  - Here we used the same routine as above and binary search on the size of the clique.

## How to Run:

## Improvements:
1) Degree heuristic for pruning of clauses.
2) Space optimization by using previous clauses in finding maximal clique.

## Collaborators:
1) Mayank
2) Rishit

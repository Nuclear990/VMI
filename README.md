# DPLL SAT Solver with Conflict-Driven Clause Learning

This project implements a SAT solver using the DPLL (Davis-Putnam-Logemann-Loveland) algorithm, enhanced with Conflict-Driven Clause Learning (CDCL). The solver checks if a formula in Conjunctive Normal Form(CNF) is satisfiable, and prints a satisfying assignment if possible. The solver is built using an object-oriented approach and includes three primary classes: Literal, Clause, and Solver.

Features
DPLL Algorithm: Implements the core DPLL procedure for satisfiability checking.
Conflict-Driven Clause Learning (CDCL): Enhances the DPLL algorithm by learning new clauses during conflicts to reduce the search space.
Literal Class: Manages individual literals and their assignments.
Clause Class: Represents clauses and provides methods to manage and evaluate them.
Solver Class: Executes the SAT-solving process, tracks decision levels, and implements clause learning.
Usage
Compile the project using your preferred C++ compiler with C++ standard 20. For example:

g++ -std=c++20 main.cpp -o solver 
Run the program interactively by providing inputs directly from the command line when prompted.

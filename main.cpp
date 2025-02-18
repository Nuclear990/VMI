#include "solver.h"
#include <iostream>

int main() {
    // Create a Solver instance
    Solver solver;

    // Take Input from User
    std::cout << "Enter number of clauses : ";
    int nclauses; 
    std::cin >> nclauses;

    for (int i = 0; i < nclauses; i++){
        std::cout << "Enter number of literals in clause : ";
        int nliterals;
        std::cin >> nliterals;

        std::cout << "Enter the literals in clause : ";
        std::vector<int> clause(nliterals);
        for (int i = 0; i < nliterals; i++)
            std::cin >> clause[i];

        solver.addClause(clause);
        std::cout << "------------------------------------------" << std::endl;
    } 

    // Print initial state
    std::cout << "Checking SAT..." << std::endl;

    // Perform SAT solving
    solver.checkSAT();

    // Display result
    std::cout << "Result: " << solver.result << std::endl;

    std::cout << "------------------------------------------" << std::endl;

    if (solver.result == "SAT"){
        std::cout << solver.printAssignment() << std::endl;
    }

    return 0;
}

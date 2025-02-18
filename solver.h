#include "clause.h"
#include <string>

class Solver{
    // pointers to clauses
    std::vector<Clause *> clauses;

    // only positive versions
    std::set<int> labels;
    
    // positive label -> pointer to Literal object
    std::shared_ptr<std::map<int, Literal *>> literals;

    // literal -> level at which it was assigned    
    std::map<int, int> decision_level;

    int current_decision_level = 0;

    // stores what decision
    std::map<int, std::set<int>> decision_sources;

public:
    std::string result;

    Solver(){
        literals = std::make_shared<std::map<int, Literal *>>();
        result = "TBD";
    }

    ~Solver(){
        for (Clause * c : clauses){
            delete c;
        }

        for (int label : labels){
            delete (*literals)[label];
        }
    }

    // inputs and a vector and adds it as a clause in the CNF Formula
    void addClause(std::vector<int> atoms){
        int size = atoms.size();
        
        // don't allow addition of an empty class
        if (size == 0) return;

        Clause * new_clause = new Clause(literals);

        // first add to labels if not present
        for (int atom : atoms){
            int pos = (atom > 0) ? atom : -atom;

            // found a new literal
            if (labels.contains(pos) == false){
                labels.insert(pos);
                Literal * new_literal = new Literal(pos);
                literals->emplace(pos, new_literal);
            }

            new_clause->addAtom(atom);
        }

        clauses.push_back(new_clause);
    }

    // gets the index of unit clause in vector clauses
    int getUnitClause(){
        int nclauses = clauses.size();
        for (int i = 0; i < nclauses; i++){
            if (clauses[i]->isUnitClause())
                return i;
        }

        return -1;
    }

    // check if all clauses have been assigned the value true
    bool allClausesSatisfied(){
        for (Clause * c : clauses){
            if (c->assigned == false || c->value == false)
                return false;
        }
        return true;
    }

    // makes a decision to progress if there are no more unit clauses
    void makeDecision(){
        for (auto & literal_pair : * literals){
            if (literal_pair.second->assigned == false){
                assignLiteral(literal_pair.first, true);
                current_decision_level++;
                return;
            }
        }
    }

    // assigns the literal to the value in every clause, and recalculates status
    void assignLiteral(int label, bool value){
        Literal * L = (*literals)[label];
        L->assign(value);

        for (Clause * c : clauses){
            c->assignAtom(label);
            c->recalculateStatus();
        }

        decision_level[label] = current_decision_level;
        return;
    }

    // deassigns the literal in every clause, and recalculates status
    void deassignLiteral(int label){
        Literal * L = (*literals)[label];
        L->deassign();

        for (Clause * c : clauses){
            c->deassignAtom(label);
            c->recalculateStatus();
        }
    }

    // backtracks to the decision level by reverting all decisions with level > backtrack_level
    void backtrack(int backtrack_level){
        // erase all assignments with level > backtrack_level
        std::set<int> to_deassign;

        for (auto it = decision_level.begin(); it != decision_level.end(); it++){
            if (it->second > backtrack_level){
                deassignLiteral(it->first);
                to_deassign.insert(it->first);
            }
        }

        for (int literal : to_deassign)
            decision_level.erase(literal);

        // recalculate status
        for (Clause * c : clauses)
            c->recalculateStatus();
        
        current_decision_level = backtrack_level;
    }

    // tries to learn a clause when a conflict is reached, returns nullptr otherwise
    Clause * analyzeConflict(Clause * conflict_clause){
        std::set<int> learnt_atoms;

        for (int atom : conflict_clause->atoms){
            learnt_atoms.insert(decision_sources[std::abs(atom)].begin(), decision_sources[std::abs(atom)].end());
        }

        Clause * learnt_clause = new Clause(literals);
        for (int atom : learnt_atoms){
            learnt_clause->addAtom(-atom);
        }

        if (learnt_clause == nullptr || learnt_clause->atoms.empty()){
            delete learnt_clause;
            return nullptr;
        }

        return learnt_clause;
    }

    // checks if given CNF formula is satisfiable, and stores in result
    void checkSAT(){
        while (true){
            // assign the unit clauses
            int index_unit_clause = getUnitClause();

            while(index_unit_clause != -1){
                // assign the unassigned literals in the unit clauses
                int unassigned = clauses[index_unit_clause]->getUnassignedAtom();

                // need to assign true to make clause true
                if (unassigned > 0){
                    (*literals)[unassigned]->assign(true);
                }
                // need to assign false to make clause true
                else{
                    (*literals)[-unassigned]->assign(false);
                }

                // set decision_sources
                for (int atom : clauses[index_unit_clause]->atoms){
                    if (atom != unassigned){
                        decision_sources[unassigned].insert(decision_sources[atom].begin(), decision_sources[atom].end());
                    }
                }

                // recalculate status and check if any clause was assigned false
                for (Clause * c : clauses){
                    c->assignAtom(unassigned);
                    c->recalculateStatus();

                    // conflict here
                    if (c->assigned == true && c->value == false){
                        Clause * learnt_clause = analyzeConflict(c);

                        if (learnt_clause == nullptr){
                            result = "UNSAT";
                            return;
                        }

                        // add the new clause
                        clauses.push_back(learnt_clause);

                        // find the backtrack_level(second highest level)
                        std::set<int> levels;
                        for (int atom : learnt_clause->atoms)
                            levels.insert(decision_level[atom]);
                        levels.erase(--levels.end());

                        int backtrack_level = * levels.rbegin();

                        backtrack(backtrack_level);
                        break;
                    }
                }

                index_unit_clause = getUnitClause();
            }  

            if (allClausesSatisfied()){
                result = "SAT";
                return;
            }

            makeDecision();
        }
    }

    // return the satisfying assignment as a string
    std::string printAssignment(){
        std::string assignment = "";
        for (auto & literal_pair : * literals){
            assignment += "Literal " + std::to_string(literal_pair.first) + " : " + ((literal_pair.second->assigned) ? std::to_string(literal_pair.second->value) : "0") + "\n";
        }
        return assignment;
    }
};

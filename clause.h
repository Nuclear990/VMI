#include "literal.h"
#include <vector>
#include <set>
#include <map>
#include <memory>

class Clause{
public:
    bool assigned;
    bool value;
    std::set<int> atoms;
    
    Clause(std::shared_ptr<std::map<int, Literal *>> __literals){
        assigned = false;
        literals = __literals;
    }

    // each literal must be inserted one by one, initially they are unassigned
    void addAtom(int label){
        atoms.insert(label);
        unassigned_atoms.insert(label);
    }

    bool isUnitClause(){
        int nunassigned = unassigned_atoms.size();
        return (assigned == false) && (nunassigned == 1);
    }

    int getUnassignedAtom(){
        return * unassigned_atoms.begin();
    }

    // to assign an unassigned atom
    void assignAtom(int label){
        bool label_present = atoms.contains(label);
        bool neglabel_present = atoms.contains(-label);

        // both label and -label are not present
        if (!label_present && !neglabel_present)
            return;
        
        if (label_present){
            unassigned_atoms.erase(label);
        }
        // -label is present
        else{
            unassigned_atoms.erase(-label);
        }
    }

    void deassignAtom(int label){
        bool label_present = atoms.contains(label);
        bool neglabel_present = atoms.contains(-label);

        // both label and -label are not present
        if (!label_present && !neglabel_present)
            return;
        
        if (label_present){
            unassigned_atoms.insert(label);
        }
        // -label is present
        else{
            unassigned_atoms.insert(-label);
        }
    }

    // to recalculate status of the clause
    void recalculateStatus(){
        bool all_assigned = true;
        bool true_atom_present = false;

        for (int atom : atoms){
            int pos = (atom > 0) ? atom : -atom;
            Literal * L = (*literals)[pos];

            // if the literal is unassigned, continue search for a positive atom
            if (L->assigned == false){
                all_assigned = false;
                continue;
            }

            // if atom has been assigned to true, we are done
            if ((L->value == true && atom > 0) || (L->value == false && atom < 0)){
                true_atom_present = true;
                break;
            }
        }

        if (true_atom_present){
            assigned = true;
            value = true;
            return;
        }
        // all assigned but no true atom present => all atoms are false
        else if (all_assigned){
            assigned = true;
            value = false;
            return;
        }
        
        // otherwise, there is no true atom and all atoms haven't been assigned
        assigned = false;
        return;
    }

private:
    std::set<int> unassigned_atoms;
    std::shared_ptr<std::map<int, Literal *>> literals;
};

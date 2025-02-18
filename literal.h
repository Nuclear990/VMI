struct Literal{
    int label;
    bool assigned;
    bool value;

    Literal(int __label){
        label = __label;
        assigned = false;
    }

    void assign(bool __value){
        assigned = true;
        value = __value;
    }

    void deassign(){
        assigned = false;
    }
};

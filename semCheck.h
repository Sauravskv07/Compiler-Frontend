/*
GROUP NO. = 46
2017A7PS0006P PIKLU PAUL
2017A7PS0007P RAJABABU SAIKIA
2017A7PS0090P SAURAV VIRMANI
2017A7PS0111P SIDDHANT KHARBANDA
2017A7PS0275P SREYAS RAVICHANDRAN
*/


int sem_num_errors;

void checkSemRules(astnode *t,symnode* current);
symnode* modulesParHamla(astnode *t,symnode* root);
int fillTheParams(astnode* t,parameters *pr,int current_offset);
VarType typeCheck(astnode *op,astnode *first,astnode *second);
symnode* semCheck(astnode *t);
ht_item* getID(astnode *t,symnode *current);
void checkOutputList(astnode *t,symnode *current,parameters *pr);

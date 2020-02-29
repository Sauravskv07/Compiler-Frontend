/*
GROUP NO. = 46
2017A7PS0006P PIKLU PAUL
2017A7PS0007P RAJABABU SAIKIA
2017A7PS0090P SAURAV VIRMANI
2017A7PS0111P SIDDHANT KHARBANDA
2017A7PS0275P SREYAS RAVICHANDRAN
*/

#include "parserDef.h"

error_list* errors;

error_list* parseTree(char* parseTreeFile);
error_list* parseTree2(char* parseTreeFile);

void printParseTree(treenode* root,char *parseTreeFile);

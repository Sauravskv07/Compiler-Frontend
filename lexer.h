/*
GROUP NO. = 46
2017A7PS0006P PIKLU PAUL
2017A7PS0007P RAJABABU SAIKIA
2017A7PS0090P SAURAV VIRMANI
2017A7PS0111P SIDDHANT KHARBANDA
2017A7PS0275P SREYAS RAVICHANDRAN
*/
#include <stdio.h>
#include "lexerDef.h"

FILE * getStream(FILE* fp);
Token* createToken(int type,int bp,int fp, int ln);
Token* getNextToken();
void removeComments(char* testcaseFile, char* cleanFile);


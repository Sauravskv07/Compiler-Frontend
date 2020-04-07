/*
GROUP NO. = 46
2017A7PS0006P PIKLU PAUL
2017A7PS0007P RAJABABU SAIKIA
2017A7PS0090P SAURAV VIRMANI
2017A7PS0111P SIDDHANT KHARBANDA
2017A7PS0275P SREYAS RAVICHANDRAN
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_BUFF_SIZE 500
#define MAX_SIZE_MAPPING_TABLE 251


typedef enum{KEYWORD,TERMINAL, NONTERMINAL} Type;

typedef union
{
	int i_val;
	float f_val;
}Value;

typedef struct
{
	char lexeme[21];
	Value val;
	int index;
	int LN;
} Token;

FILE* fp;
char buffer[MAX_BUFF_SIZE];
int forwardPointer;
int backPointer;
int state;
char character_read;
int LN;
char token[20];
int j_pointer;
int endReached;
int num_terminals;
int num_nonterminals;


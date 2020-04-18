/*
GROUP NO. = 46
2017A7PS0006P PIKLU PAUL
2017A7PS0007P RAJABABU SAIKIA
2017A7PS0090P SAURAV VIRMANI
2017A7PS0111P SIDDHANT KHARBANDA
2017A7PS0275P SREYAS RAVICHANDRAN
*/



typedef enum{START,NOP,CALL,RET,DEC,PRINT,INP,ARR_GET,ARR_ASSIGN,BRANCH,JUMP,ASSIGN,INC,EQUATE,
UNARY,PLUS,MINUS,MUL,DIV,AND,OR,LT,LE,GT,GE,EQ,NE,PUSH,POP} Operator;

typedef struct
{
	int offset;
	VarType baseType;
}temp_item;

typedef struct quad_row quad_row;

typedef union{
    var_item* v_item;
    Token* tk_item;
    quad_row* qr_item;
    temp_item* t_item;
} quad_entry;

struct quad_row
{
	int srno;
	Operator op;
	quad_entry val[3];
	int tag[3];
	quad_row *prev;
	quad_row *next;
	struct symnode *right;
	struct symnode *parent;
};

quad_row *quad_row_head;
quad_row *quad_row_tail;
symnode* vis;
func_item* funcsym;
int tempLabel;
int srno;
ht_hash_table* label_table;

void printIR(quad_row *q);
void genIRTable(astnode *t,symnode* sym);
symnode* getNextSymbolTable();
void setNextSymbolTable(func_item* fsym);
ht_item* getVar(symnode* sym, Token *t);
void resetIRParams();
temp_item* getTempVariable(VarType baseType);
quad_row * getEmptyRow();

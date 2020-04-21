
typedef enum
{AL, BL, SPL, BPL, SIL, DIL, DL, CL, R8B, R9B, R10B, R11B, R12B, R13B, R14B, R15B, AX, BX, SP, BP, SI, DI, DX, CX, R8W, R9W, R10W, R11W,R12W, R13W, R14W,R15W, EAX, EBX, ESP, EBP, ESI, EDI, EDX, ECX, R8D, R9D, R10D, R11D, R12D, R13D, R14D, R15D, RAX, RBX, RSP, RBP, RSI, RDI, RDX, RCX, R8, R9, R10, R11, R12, R13, R14, R15}Reg;


void deallocMemory(symnode* current_func,FILE* fp);
void codeGen(quad_row *q);
int getTok(Token* tk,quad_row *q,FILE* fp);
void allocMemory(symnode* current_func,FILE* fp);
int getVarReg(var_item* v,quad_row *q,FILE* fp);
int getTemp(temp_item* t,quad_row *q);
void freeAllReg();
void setAddr(var_item* v,FILE* fp);
int getReg(int size,quad_row *q);
void freeReg(int reg);

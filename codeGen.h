
typedef enum
{AL, BL, SPL, BPL, SIL, DIL, CL, DL, R8B, R9B, R10B, R11B, R12B, R13B, R14B, R15B, AX, BX, SP, BP, SI, DI, CX, DX, R8W, R9W, R10W, R11W,R12W, R13W, R14W,R15W, EAX, EBX, ESP, EBP, ESI, EDI, ECX, EDX, R8D, R9D, R10D, R11D, R12D, R13D, R14D, R15D, RAX, RBX, RSP, RBP, RSI, RDI, RCX, RDX, R8, R9, R10, R11, R12, R13, R14, R15}Reg;

void codeGen(quad_row *q);
int getTok(Token* tk,quad_row *q);
void allocMemory(symnode* current_func);
int getVarReg(var_item* v,quad_row *q);
int getTemp(temp_item* t,quad_row *q);
void freeAllReg();
void setAddr(var_item* v);
int getReg(int size,quad_row *q);
void freeReg(int reg);

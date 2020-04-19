ht_item* reg_arr[16];
typedef enum
{
RAX, 
RCX, 
RDX, 
RBX, 
RSP, 
RBP, 
RSI, 
RDI, 
R8, 
R9,
R10,
R11,
R12,
R13,
R14,
R15
}



int getReg();

void genCode(quad_row *q);


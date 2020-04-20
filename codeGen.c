#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "hash_table.h"
#include "parseRules.h"
#include "utils.h"
#include "adt.h"
#include "ast.h"
#include "symbol_table.h"
#include "ir.h"
#include "codeGen.h"

int temp_arr[1000][2];
int reg_arr[16];

const char *registers[] = {"AL", "BL", "SPL", "BPL", "SIL", "DIL", "CL", "DL", "R8B", "R9B", "R10B", "R11B", "R12B", "R13B", "R14B", "R15B", "AX", "BX", "SP", "BP", "SI", "DI", "CX", "DX", "R8W", "R9W", "R10W", "R11W", "R12W", "R13W", "R14W", "R15W", "EAX", "EBX", "ESP", "EBP", "ESI", "EDI", "ECX", "EDX", "R8D", "R9D", "R10D", "R11D", "R12D", "R13D", "R14D", "R15D", "RDX", "RBX", "RSP", "RBP", "RSI", "RDI", "RAX", "RCX", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15"};

int current_reg=5;

void allocMemory(symnode* current_func)
{
	int offset=current_func->current_offset;
	
	int i=16-offset%16;
	if(i>8)
		i=i-8;
	if(i<8)
		i=i+8;
	
	fprintf(fp,"\tsub\tRSP,\t%d\n",offset+i);
}

int getTok(Token* tk,quad_row *q)
{
	int reg=0;	
	if(tk->index==2)
	{
		reg=getReg(2,q);
		fprintf(fp,"\nmov\t%s,\t%d\n",registers[reg],tk->val.i_val);
	}
	else if(tk->index==8)
	{
		reg=getReg(1,q);
		fprintf(fp,"\nmov\t%s,\t1\n",registers[reg]);
	}
	else if(tk->index==9)
	{
		reg=getReg(1,q);
		fprintf(fp,"\nmov\t%s,\t0\n",registers[reg]);
	}
	else
	{
		printf("Real Tokens Not Handled\n");
	}

	reg_arr[reg]=-2;

	return reg;
}
void freeReg(int reg_index)
{
	reg_arr[reg_index%16]=-1;
}

int getVarReg(var_item* v,quad_row *q)
{
	int reg;
	int offset;

	if(offset>=0)
		offset=offset+8;
	fprintf(fp,"\tmov\trbx,\trsp\n");
	fprintf(fp,"\tadd\trbx,\t%d\n",offset);

	if(v->baseType==1)
	{
		reg=getReg(1,q);
		fprintf(fp,"\nmov\t%s,\t[rbx]\n",registers[reg]);
	}
	if(v->baseType==0)
	{
		reg=getReg(2,q);
		fprintf(fp,"\nmov\t%s,\t[rbx]\n",registers[reg]);
	}
	if(v->baseType==3)
	{
		reg=getReg(4,q);
		if(v->low==-1 || v->high==-1)
		{
			fprintf(fp,"\nmov\t%s,\t[rbx]\n",registers[reg]);
		}
		else
			fprintf(fp,"\nmov\t%s,\trbx\n",registers[reg]);
	}

	reg_arr[reg]=-2;

	return reg;
}

int getTemp(temp_item* t,quad_row *q)
{
	int arr[]={2,1,3,4};
	int reg;
	if(temp_arr[t->offset][0]==-1)
	{
		reg=getReg(arr[t->baseType],q);
		temp_arr[t->offset][0]=reg;
		temp_arr[t->offset][1]=0;	
	}
	else
	{
		reg=temp_arr[t->offset][0];
		temp_arr[t->offset][1]++;
	}
	return reg;
}

void freeAllReg()
{
	for(int i=0;i<16;i++)
		reg_arr[i]=-1;
}

void setAddr(var_item* v)
{
	int offset;

	if(offset>=0)
		offset=offset+8;

	fprintf(fp,"\tmov\trbx,\trsp\n");
	fprintf(fp,"\tadd\trbx,\t%d\n",offset);

	if(v->baseType==3)
	{
		if(v->low==-1 || v->high==-1)
		{
			fprintf(fp,"\nmov\trbx,\t[rbx]\n");
		}
	}

}

int getReg(int size,quad_row *q)
{

	//tage care that current_reg 6..15
	printf("inside Get Reg\n");
	int reg;
	int flag=0;
	for(int i=0;i<9;i++)
	{
		current_reg++;
		if(current_reg>15)
			current_reg=6;
		int flag2=0;
		if(reg_arr[current_reg]==-2)
			reg_arr[current_reg]++;
		else if(reg_arr[current_reg]==-1)
		{
			reg=current_reg;
			flag=1;
			break;
		}
		else if(reg_arr[current_reg]>=0)
		{
			if(temp_arr[reg_arr[current_reg]][1]>0)
			{
				while(q!=NULL)
				{
					flag2=0;
					for(int j=0;j<2;j++)
					{
						if(q->tag[i]==3 && q->val[i].t_item->offset==reg_arr[current_reg])
						{
							flag2=1;
							break;
						}
					}
					if(flag2==1)
						break;
					q=q->next;
				}
			}
			if(flag2==0)
			{
				reg=current_reg;
				flag=1;	
			}
		}	
	}

	if(flag==1)
	{
		if(size==1)
			reg=reg;
		else if(size==2)
			reg=reg+16;
		else if(size==3)
			reg=reg+32;
		else if(size==4)
			reg=reg+48;
		else
			printf("invalid size of register\n");
	}
	else
	{
		printf("registers filled with temprories, abort \n");
		exit(0);	
	}
	printf("outside get reg\n");
}

void codeGen(quad_row *q)
{


	for(int i=0;i<1000;i++)
	{
		temp_arr[i][0]=-1;
		temp_arr[i][1]=-1;
	}
	FILE* fp;

	//fp= fopen("code.asm","w");
	fp=stdout;
	if(fp==NULL)
	{
		printf("some error while opening the code file\n");
		return;
	}

	fprintf(fp,"global  main\nextern printf,scanf\n,section .text");

	symnode* current_func=sym_root->child;
	int func_start =1;
	int main_label=-1;
	char buf[12];
	int reg;
	int regop1,regop2,regop3,regres;

	freeAllReg();

	const char *OPNames[] = {"START","END","NOP","CALL","RET","DEC","PRINT","INP","ARR_GET","ARR_ASSIGN","BRANCH", "JUMP","ASSIGN","INC","EQUATE","UNARY","PLUS","MINUS","MUL","DIV,","AND","OR","LT","LE","GT","GE","EQ","NE","PUSH","POP","POP2"};


	while(q!=NULL)
	{
		
		printf("\n");
		printf("%d \t\t",q->srno);
		printf("%s \t\t",OPNames[q->op]);
		for(int i=0;i<3;i++)
		{
			if(q->tag[i]==0) printf("v-%d \t\t",q->val[i].v_item->offset);
			else if(q->tag[i]==1) printf("tk-%s \t\t",q->val[i].tk_item->lexeme);
			else if(q->tag[i]==2) printf("l-%d \t\t",q->val[i].qr_item->srno);
			else if(q->tag[i]==3) printf("t-%d \t\t",q->val[i].t_item->offset);
			else printf("\t\t");
		}
		
		if(q->next->op==NOP)
		{
			main_label=q->next->srno;
		}


		switch(q->op)
		{
			case DEC:
			{
				if(q->tag[0]==0)
				{
					if(q->val[0].v_item->baseType==3)
					{
						if(q->val[0].v_item->low==-1 || q->val[0].v_item->high==-1)
						{
							if(q->val[0].v_item->low==-1)
							{
								if(q->val[0].v_item->lowNode!=NULL)
								{
									regop1=getVarReg(q->val[0].v_item->lowNode->data->v_item,q);
								}
								else
									printf("Error Dynamic Array\n");
							}
							else
							{
								regop1=getReg(2,q);
								fprintf(fp,"\tmov\t%s,\t%d\n",registers[regop1],q->val[0].v_item->low);
							}
							if(q->val[0].v_item->high==-1)
							{
								if(q->val[0].v_item->highNode!=NULL)
								{
									regop2=getVarReg(q->val[0].v_item->highNode->data->v_item,q);
								}
								else
									printf("Error Dynamic Array\n");
							}
							else
							{
								regop2=getReg(2,q);
								fprintf(fp,"\tmov\t%s,\t%d\n",registers[regop2],q->val[0].v_item->high);	
							}
							
							regop3=getReg(3,q);
							fprintf(fp,"\tmov\t%s,\t%s\n",registers[regop3],registers[regop2]);
							fprintf(fp,"\tsub\t%s,\t%s\n",registers[regop2],registers[regop1]);
							
							if(q->val[0].v_item->eleType==0)
								fprintf(fp,"\tadd\t%s,\t%s\n",registers[regop2],registers[regop2]);
							fprintf(fp,"\tsub\trsp,\t%s\n",registers[regop2]);		
							
							fprintf(fp,"\tpush\t%s\n",registers[regop3]);
							fprintf(fp,"\tpush\t%s\n",registers[regop1]);

							fprintf(fp,"\tmov\t[rbp+%d],\trsp\n",q->val[0].v_item->offset);
							freeReg(regop1);
							freeReg(regop2);
							freeReg(regop3);
						}
						else
						{
							setAddr(q->val[0].v_item);
							fprintf(fp,"\tmov\tword [RBX],%d\n",q->val[0].v_item->low);
							fprintf(fp,"\tmov\tword [RBX+2],%d\n",q->val[0].v_item->high);
						}	
					}
				}
				else
					printf("error dec 1 \n");
				break;
			}
			case INC:
			{
				if(q->tag[0]==0)
				{
					setAddr(q->val[0].v_item);
					fprintf(fp,"\tinc\tword [RBX]\n");
				}
				else
					printf("error inc 1 \n");		
				break;
			}
			case EQUATE:
			case LT:
			case LE:
			case GT:
			case GE:
			case EQ:
			case NE:
			{
				if(q->tag[0]==0)
					regop1=getVarReg(q->val[0].v_item,q);
				else if(q->tag[0]==3)
					regop1=getTemp(q->val[0].t_item,q);

				else printf("Relation operator error 1 \n");

				if(q->tag[1]==1)
					regop2=getTok(q->val[1].tk_item,q);
				else if(q->tag[1]==3)
					regop2=getTemp(q->val[1].t_item,q);
				else printf("Relation operator error 2 \n");

				if(q->tag[2]==3)
					regop3=getTemp(q->val[2].t_item,q);
				else printf("Relation operator error 3 \n");

				fprintf(fp,"\tCMP\t%s,\t%s\n",registers[regop1],registers[regop2]);
				
				if(q->op==LT)
					fprintf(fp,"\tJGE\tJMP%d\n",q->srno);
				else if(q->op==LE)
					fprintf(fp,"\tJG\tJMP%d\n",q->srno);
				else if(q->op==GT)
					fprintf(fp,"\tJLE\tJMP%d\n",q->srno);
				else if(q->op==GE)
					fprintf(fp,"\tJL\tJMP%d\n",q->srno);
				else if(q->op==EQ)
					fprintf(fp,"\tJNE\tJMP%d\n",q->srno);
				else if(q->op==EQUATE)
					fprintf(fp,"\tJNE\tJMP%d\n",q->srno);
				else if(q->op==NE)
					fprintf(fp,"\tJE\tJMP%d\n",q->srno);
				
				fprintf(fp,"\tmov\t%s,\t1\n",registers[regop3]);

				fprintf(fp,"\tJMP\t%d:\n",q->srno);

				freeReg(regop1);
				freeReg(regop2);

				break;
			}

			case BRANCH:
			{
				if(q->tag[0]==3)
					regop1=getTemp(q->val[0].t_item,q);
				else
					printf("error branch 1 \n");

				if(q->tag[1]!=3 || q->tag[2]!=3)
				{	
					printf("error branch 2-3 \n");
					break;
				}
				fprintf(fp,"\tCMP\t%s,\t0\n",registers[regop1]);
				fprintf(fp,"\tJE Label%d\n",q->val[3].qr_item->srno);
				fprintf(fp,"\tJMP Label%d\n",q->val[2].qr_item->srno);
				break;	
			}
			case UNARY:
			{
				if(q->tag[1]==0)
					regop1=getVarReg(q->val[1].v_item,q);
				else if(q->tag[1]==1)
					regop1=getTok(q->val[1].tk_item,q);
				else if(q->tag[1]==3)
					regop1=getTemp(q->val[1].t_item,q);
				else
					printf("error unary op 2\n");
				if(q->tag[0]==1)
				{
					if(q->tag[2]==3)
						regop2=getTemp(q->val[2].t_item,q);
					else
					{
						printf("error unary op 3\n");
					}
					if(q->val[1].tk_item->index==10)
						fprintf(fp,"\tmov\t%s,\t%s\n",registers[regop2],registers[regop1]);
					else
					{
						fprintf(fp,"\tneg\t%s\n",registers[regop1]);
						fprintf(fp,"\tmov\t%s,\t%s\n",registers[regop2],registers[regop1]);
					}
					break;
				}
				else
					printf("error unary op 1\n");
				freeReg(regop1);
				break;
			}
			
			case ARR_ASSIGN:
			{
				if(q->tag[0]==3)
					regop2=getTemp(q->val[0].t_item,q);
				else
					printf("Invalid operand arr_get 1\n");

				if(q->tag[1]==1)
					regop1=getTok(q->val[1].tk_item,q);
				
				else if(q->tag[1]==0)
					regop1=getVarReg(q->val[1].v_item,q);
				
				else
					printf("Invalid operand arr_get 2\n");
				
				fprintf(fp,"\tmovsx\tRSI,\t%s\n",registers[regop1]);

				if(q->tag[2]==0)
				{	setAddr(q->val[2].v_item);
					if(q->val[2].v_item->baseType==0)
						fprintf(fp,"\tmov\t[RBX+2*RSI],\t%s\n",registers[regop2]);
					else if(q->val[2].v_item->baseType==2)
						fprintf(fp,"\tmov\t[RBX+RSI],\t%s\n",registers[regop2]);
					else
						printf("invalid type arr-assign \n");
				}
				else
					printf("Invalid operand arr_get 3\n");

				freeReg(regop1);
				freeReg(regop2);
				break;			
			}
			case ARR_GET:
			{
				if(q->tag[0]==0)
					setAddr(q->val[0].v_item);
				else
					printf("Invalid operand arr_get 1\n");
				if(q->tag[1]==1)
					regop1=getTok(q->val[1].tk_item,q);
				else if(q->tag[1]==3)
					regop1=getTemp(q->val[1].t_item,q);
				else if(q->tag[1]==0)
					regop1=getVarReg(q->val[1].v_item,q);
				else
					printf("Invalid operand arr_get 2\n");

				fprintf(fp,"\tmovsx\tRSI,\t%s\n",registers[regop1]);

				if(q->tag[2]==3)
				{
					regop2=getTemp(q->val[2].t_item,q);
					if(q->val[2].t_item->baseType==0)
						fprintf(fp,"\tmov\t%s,\t[RBX+2*RSI]\n",registers[regop2]);
					else if(q->val[2].t_item->baseType==2)
						fprintf(fp,"\tmov\t%s,\t[RBX+RSI]\n",registers[regop2]);
					else
						printf("invalid type arr-get \n");				
				}
				else
					printf("Invalid operand arr_get 3\n");
				freeReg(regop1);
				break;			
			}

			case ASSIGN:
			{
				if(q->tag[0]==0)
					regop1=getVarReg(q->val[0].v_item,q);
				else if(q->tag[0]==1)
					regop1=getTok(q->val[0].tk_item,q);
				else if(q->tag[0]==3)
					regop1=getTemp(q->val[0].t_item,q);
				else
					printf("Invalid op to assign 1\n");

				if(q->tag[2]==0)
				{
					setAddr(q->val[0].v_item);
					fprintf(fp,"\tmov\t[RBX],\t%s\n",registers[regop1]);
				}
				else if(q->tag[2]==3)
				{
					regop2=getTemp(q->val[0].t_item,q);
					fprintf(fp,"\tmov\t%s,\t%s\n",registers[regop2],registers[regop1]);
				}
				else
					printf("Invalid op to assign 2\n");

				if(q->tag[0]==1)
					freeReg(regop1);
				if(q->tag[0]==3 && q->tag[2]==0)
					freeReg(regop1);

				break;									
	
			}
			case CALL:
			{
				fprintf(fp,"\tcall\tLabel%d\n",q->val[0].qr_item->srno);
				break;	
			}
			case START:
			{			
				break;
			}
			case END:
			{
				fprintf(fp,"\tmov\trax,\t60\t\t; system call for exit\n");
				fprintf(fp,"\txor\trdi,\trdi\t\t; exit code 0\n");
				fprintf(fp,"\tsyscall\t\t; invoke operating system to exit\n");
				break;
			}
			case RET:
			{
				fprintf(fp,"\tret\n");
				current_func=current_func->right;
				func_start=1;
				break;
			}
			case NOP:
			{
				if(q->srno==main_label)
					fprintf(fp,"main:\n");
				else
				{
      					fprintf(fp,"Label%d:\n", q->srno);
				}
				
				if(func_start==1)
				{
					allocMemory(current_func);//new function
					freeAllReg();//new function;
					func_start=0;
				}
				break;
			}
			case JUMP:
			{
				if(main_label==-1)
					main_label=q->val[0].qr_item->srno;
				else
				{
					fprintf(fp,"\tJMP\tLabel%d\n", q->val[0].qr_item->srno);
				}
				break;
			}

			case PUSH:
			{
				if(q->tag[0]==0)
				{
					setAddr(q->val[0].v_item);
					if(q->val[0].v_item->baseType==0)
						fprintf(fp,"\tPUSH\tword [rbx]\n");
					if(q->val[0].v_item->baseType==2)
						fprintf(fp,"\tPUSH\tbyte [rbx]\n");
					if(q->val[0].v_item->baseType==3)
						fprintf(fp,"\tPUSH\tqword [rbx]\n");
					else
						printf("type not compatible with pop\n");
				}
				else
					printf("Other types of push not possible\n");
				break;
			}

			case POP:
			{
				if(q->tag[0]==0)
				{
					setAddr(q->val[0].v_item);
					if(q->val[0].v_item->baseType==0)
						fprintf(fp,"\tPOP\tword [rbx]\n");
					if(q->val[0].v_item->baseType==2)
						fprintf(fp,"\tPOP\tbyte [rbx]\n");
					if(q->val[0].v_item->baseType==3)
						fprintf(fp,"\tPOP\tqword [rbx]\n");
					else
						printf("real type not compatible with pop\n");
				}
				else
					printf("Error pop\n");

				break;
			}
			case POP2:
			{
				if(q->tag[0]==0)
				{
					reg=getVarReg(q->val[0].v_item,q);
					fprintf(fp,"\tPOP\t%s\n",registers[reg]);
					freeReg(reg);//new function
				}
				else
					printf("Other types of POP not possible\n");

				break;
			}
			case OR:
			case AND:
			case DIV:
			case MUL:
			case PLUS:
			case MINUS:
			{
				if(q->tag[0]==3)
					regop1=getTemp(q->val[0].t_item,q);
				else
					printf("Invalid Operand for Arith Operator\t");

				if(q->tag[1]==3)
					regop2=getTemp(q->val[1].t_item,q);
				else
					printf("Invalid Operand for Arith Operator");	
				
				if(q->tag[2]==3)
				{
					regop3=getTemp(q->val[2].t_item,q);//new function
					fprintf(fp,"\tMOV\t%s,\t%s\n",registers[regres],registers[regop1]);
				}
				else
					printf("Invalid Operand for Arith Operator");

				
				switch(q->op)
				{
					case PLUS:
					{
						fprintf(fp,"\tADD\t%s,\t%s\n",registers[regop3],registers[regop2]);
						break;	
					}
					case MINUS:
					{
						fprintf(fp,"\tSUB\t%s,\t%s\n",registers[regop3],registers[regop2]);
						break;	
					}
					case MUL:
					{
						fprintf(fp,"\tMUL\t%s,\t%s\n",registers[regop3],registers[regop2]);
						break;	
					}
					case DIV:
					{
						fprintf(fp,"\tDIV\t%s,\t%s\n",registers[regop3],registers[regop2]);
						break;	
					}
					case AND:
					{
						fprintf(fp,"\tAND\t%s,\t%s\n",registers[regop3],registers[regop2]);
						break;	
					}
					case OR:
					{
						fprintf(fp,"\tOR\t%s,\t%s\n",registers[regop3],registers[regop2]);
						break;	
					}

				}
				
				freeReg(regop1);
				freeReg(regop2);

				break;	
			}
			case PRINT:
			{
				if((q->tag[0]!=2 && q->tag[0]!=-1))
				{	
					if(q->tag[0]==3)
						reg=getTemp(q->val[0].t_item,q);//getTemp	
					else  if(q->tag[0]==0)
						reg=getVarReg(q->val[0].v_item,q);//getVar
					else if(q->tag[0]==1)
						reg=getTok(q->val[0].tk_item,q);//getTok
					
					fprintf(fp,"\tmov\trdi,\tOutput\n");
					fprintf(fp,"\txor\trax,\trax\n");
					fprintf(fp,"\tcall\tprintf\n");	
	
					if(q->tag[0]==0 && q->val[0].v_item->baseType==3)
					{
						fprintf(fp,"\tmov\trbx,\t%s\n",registers[reg]);
						regop1=getReg(2,q);//new fucntion
						regop2=getReg(2,q);
						fprintf(fp,"\tmov\t%s,\t[RBX]\n",registers[regop1]);
						fprintf(fp,"\tmov\t%s,\t[RBX+2]\n",registers[regop2]);
						fprintf(fp,"\tsub\t%s,\t%s\n",registers[regop2],registers[regop1]);
						fprintf(fp,"inc\t%s\n",registers[regop2]);
						fprintf(fp,"\tadd\trbx,\t4\n");
						fprintf(fp,"\tPLA%d:\n",q->srno);
						if(q->val[0].v_item->eleType==2)
						{
							fprintf(fp,"\tCMP\tbyte [RBX],\t0\n\tJZ PLF%d\n",q->srno);
							fprintf(fp,"\tmov\trdi,\tBoolTrue\n\tJMP\tPLT%d\n",q->srno);
							fprintf(fp,"PLF%d:\n",q->srno);
							fprintf(fp,"\tmov\trdi,\tBoolFalse\n");
							fprintf(fp,"PLT%d:\n",q->srno);
							fprintf(fp,"\tadd\trbx,\t1\n");				
						}
						else if(q->val[0].v_item->eleType==0)
						{
							fprintf(fp,"\tmov\tesi,\t[RBX]\n");
							fprintf(fp,"\tadd\trbx,2");
						}

						fprintf(fp,"\txor\trax,\trax\n");
						fprintf(fp,"\tcall printf\n");

						if(q->val[0].v_item->eleType==1)
						{
							printf("REAL Values are not handled");
						}						
						fprintf(fp,"\tdec\t%s\n",registers[regop2]);
						fprintf(fp,"\tjnz PLA%d\n",q->srno);
						freeReg(regop1);
						freeReg(regop2);						
					}
					else
					{			
						if((q->tag[0]==3 && q->val[0].t_item->baseType==2) ||(q->tag[0]==0 && q->val[0].v_item->baseType==2)||(q->tag[0]==1 && (q->val[0].tk_item->index==8 || q->val[0].tk_item->index==9)))
						{
							fprintf(fp,"\tCMP\t%s,\t0\n\tJE PLF%d\n",registers[reg],q->srno);
							fprintf(fp,"\tmov\trdi,\tBoolTrue\n\tJMP\tPLT%d\n",q->srno);
							fprintf(fp,"PLF%d:\n",q->srno);
							fprintf(fp,"\tmov\trdi,\tBoolFalse\n");
							fprintf(fp,"PLT%d:\n",q->srno);
						}
						else if((q->tag[0]==3 && q->val[0].t_item->baseType==0) ||(q->tag[0]==0 && q->val[0].v_item->baseType==0)||(q->tag[0]==1 && q->val[0].tk_item->index==2))
						{
							fprintf(fp,"\tmov\trdi,\tIntOutput\n");
							fprintf(fp,"\tmov\trsi,\t%s\n",registers[reg]);
						}
						else
						{
							printf("printf only handles bool and int");
						}
						
						fprintf(fp,"\txor\trax,\trax\n");
						fprintf(fp,"\tcall\tprintf\n");
					}
					freeReg(reg);
				}
				break;
			}
			case INP:
			{
				if(q->tag[0]==0)
				{
					setAddr(q->val[0].v_item);
					if(q->val[0].v_item->baseType==0 ||q->val[0].v_item->baseType==2)
					{
						if(q->val[0].v_item->baseType==0)
							fprintf(fp,"\tmov\trdi,\tIntInputString\n");
						if(q->val[0].v_item->baseType==2)	
							fprintf(fp,"\tmov\trdi,\tBoolInputString\n");

						fprintf(fp,"\txor\trax,\trax\n");
						fprintf(fp,"\tcall printf\n");
					
						fprintf(fp,"\tmov\trdi,\tIntInput\n");
						fprintf(fp,"\tmov\trsi,intvar\t\n");
						fprintf(fp,"\txor\trax,\trax\n");
						fprintf(fp,"\tcall\tscanf\n");
					
						fprintf(fp,"\tmov\tax,\t[intvar]\n");
						if(q->val[0].v_item->baseType==0)
							fprintf(fp,"\tmov\t[RBX],\tax\n");
						if(q->val[0].v_item->baseType==2)	
							fprintf(fp,"\tmov\t[RBX],\tal\n");

					}
					else if(q->val[0].v_item->baseType==3)
					{
						regop1=getReg(2,q);//new fucntion
						regop2=getReg(2,q);
						fprintf(fp,"\tmov\t%s,\t[RBX]\n",registers[regop1]);
						fprintf(fp,"\tmov\t%s,\t[RBX+2]\n",registers[regop2]);
						fprintf(fp,"\tsub\t%s,\t%s\n",registers[regop2],registers[regop1]);
						fprintf(fp,"inc\t%s\n",registers[regop2]);
						fprintf(fp,"\tadd\trbx,\t4\n");

						if(q->val[0].v_item->baseType==0)
							fprintf(fp,"\tmov\trdi,\tIntArrInputString\n");
						if(q->val[0].v_item->baseType==2)	
							fprintf(fp,"\tmov\trdi,\tBoolArrInputString\n");
						
						fprintf(fp,"\tmovzx\trsi,\t%s\n",registers[regop2]);

						fprintf(fp,"\txor\trax,\trax\n");
						fprintf(fp,"\tcall printf\n");

						fprintf(fp,"\tILA%d:\n",q->srno);
					
						fprintf(fp,"\tmov\trdi,\tIntInput\n");
						fprintf(fp,"\tmov\trsi,intvar\t\n");
						fprintf(fp,"\txor\trax,\trax\n");
						fprintf(fp,"\tcall\tscanf\n");
							
						fprintf(fp,"\tmov\tax,\t[intvar]\n");
						if(q->val[0].v_item->baseType==0)
						{
							fprintf(fp,"\tmov\t[RBX],\tax\n");
							fprintf(fp,"\tadd\trbx,2\n");
						}
						if(q->val[0].v_item->baseType==2)	
						{	
							fprintf(fp,"\tmov\t[RBX],\tal\n");
							fprintf(fp,"\tadd\trbx,1");
						}
						else if(q->val[0].v_item->eleType==1)
						{
							printf("REAL Values are not handled");
						}						
						fprintf(fp,"\tdec\t%s\n",registers[regop2]);
						fprintf(fp,"\tjnz ILA%d\n",q->srno);
						freeReg(regop1);
						freeReg(regop2);
					}
					else
						printf("real values not handled");					
				}
				else
				{
					printf("INPUT Invalid \n");
				}
				
				break;
			}
		}

		q=q->next;	
	}

	fprintf(fp,"section .data\n");
	fprintf(fp,"Output:\n\tdb \"Output =\",10,0\n");
	fprintf(fp,"BoolTrue:\n\tdb \"true\",10,0\n");
	fprintf(fp,"BoolFalse:\n\tdb \"false\",10,0\n");
	fprintf(fp,"IntOutput:\n\tdb \"%%hd\",10,0\n");
	fprintf(fp,"IntInput:\n\tdb \"%%hd\",0,\n");
	fprintf(fp,"IntInputString:\n\tdb \"Please Enter an Integer\",10,0\n");
	fprintf(fp,"BoolInputString:\n\tdb \"Please Enter 0 for false and 1 for true\",10,0\n");
	fprintf(fp,"IntArrInputString:\n\tdb \"Please Enter an %%d Integers\",10,0 \n");
	fprintf(fp,"BoolInputString:\n\tdb \"Please Enter %%d 0s or 1s for false and true\",10,0");
	fprintf(fp,"section .bss\n");
	fprintf(fp,"intvar: resw 1\n");
	fclose(fp);

}

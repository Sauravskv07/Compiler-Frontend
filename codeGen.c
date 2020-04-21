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

const char *registers[] = {"AL", "BL", "SPL", "BPL", "SIL", "DIL", "DL","CL", "R8B", "R9B", "R10B", "R11B", "R12B", "R13B", "R14B", "R15B", "AX", "BX", "SP", "BP", "SI", "DI", "DX", "CX", "R8W", "R9W", "R10W", "R11W", "R12W", "R13W", "R14W", "R15W", "EAX", "EBX", "ESP", "EBP", "ESI", "EDI", "EDX", "ECX", "R8D", "R9D", "R10D", "R11D", "R12D", "R13D", "R14D", "R15D","RAX", "RBX", "RSP", "RBP", "RSI", "RDI", "RDX", "RCX", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15"};


int current_reg=6;

void regState()
{
	printf("\n");
	for(int reg=0;reg<16;reg++)
	{
		printf("%s ",registers[reg]);
	}
	printf("\n");
	for(int reg=0;reg<16;reg++)
	{
		printf("%d  ",reg_arr[reg]);
	}
	printf("\n");
}

int getReg(int size,quad_row *q)
{

	current_reg++;
	//printf("GET_REG\n");
	int reg;
	int flag=0;
	for(int i=0;i<9;i++)
	{
		if(current_reg>15)
			current_reg=7;
		int flag2;
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
				if(flag2==0)
				{
					reg=current_reg;
					flag=1;
					break;
				}
			}
		}
		current_reg++;
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
	printf("outside get reg %s %d\n",registers[reg],size);
	return reg;
}

void allocMemory(symnode* current_func,FILE* fp)
{
	//printf("INSIDE ALLOC MEMORY\n");
	int offset=current_func->current_offset;
	
	int i=16-offset%16;
	if(i>8)
		i=i-8;
	if(i<8)
		i=i+8;

	fprintf(fp,"\tmov\tRBP,\tRSP\n");
	fprintf(fp,"\tsub\tRSP,\t%d\n",offset+i);
}

void deallocMemory(symnode* current_func,FILE* fp)
{
	//printf("INSIDE ALLOC MEMORY\n");
	int offset=current_func->current_offset;
	
	int i=16-offset%16;
	if(i>8)
		i=i-8;
	if(i<8)
		i=i+8;

	fprintf(fp,"\tadd\tRSP,\t%d\n",offset+i);
}

int getTok(Token* tk,quad_row *q,FILE* fp)
{
	int reg=0;	
	if(tk->index==2)
	{
		reg=getReg(2,q);
		fprintf(fp,"\tmov\t%s,\t%d\n",registers[reg],tk->val.i_val);
	}
	else if(tk->index==8)
	{
		reg=getReg(1,q);
		fprintf(fp,"\tmov\t%s,\t1\n",registers[reg]);
	}
	else if(tk->index==9)
	{
		reg=getReg(1,q);
		fprintf(fp,"\tmov\t%s,\t0\n",registers[reg]);
	}
	else
	{
		printf("Real Tokens Not Handled\n");
	}

	reg_arr[reg%16]=-2;

	return reg;
}
void freeReg(int reg_index)
{
	if(reg_index>0)
		reg_arr[reg_index%16]=-1;
	else
		printf("Invalid register %d\n",reg_index);
}

int getVarReg(var_item* v,quad_row *q,FILE* fp)
{
	//printf("INSIDE GET VAR\n");
	int reg=0;
	int offset=v->offset;
	
	//printf("OFFSET = %d\n",offset);

	if(offset>=0)
		offset=offset+16;

	fprintf(fp,"\tmov\trbx,\trbp\n");
	fprintf(fp,"\tadd\trbx,\t%d\n",offset);


	if(v->baseType==0)
	{
		reg=getReg(2,q);
		fprintf(fp,"\tmov\t%s,\t[rbx]\n",registers[reg]);
	}
	if(v->baseType==2)
	{
		reg=getReg(1,q);
		fprintf(fp,"\tmov\t%s,\t[rbx]\n",registers[reg]);
	}
	else if(v->baseType==1)
	{
		reg=getReg(3,q);
		fprintf(fp,"\tmov\t%s,\t[rbx]\n",registers[reg]);
	}
	else if(v->baseType==3)
	{
		reg=getReg(4,q);
		if(v->low==-1 || v->high==-1)
		{
			fprintf(fp,"\tmov\t%s,\t[rbx]\n",registers[reg]);
		}
		else
			fprintf(fp,"\tmov\t%s,\trbx\n",registers[reg]);
	}
	
	reg_arr[reg%16]=-2;

	return reg;
}

int getTemp(temp_item* t,quad_row *q)
{
	//printf("INSIDE GET TEMP %d\n",t->baseType);
	int arr[]={2,1,3,4};
	int reg;
	if(temp_arr[t->offset][0]==-1)
	{
		reg=getReg(arr[t->baseType],q);
		reg_arr[reg%16]=t->offset;
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

void setAddr(var_item* v,FILE* fp)
{
	//printf("INSIDE SET ADDR\n");
	int offset=v->offset;

	//printf("OFFSET = %d\n",offset);

	if(offset>=0)
		offset=offset+16;

	fprintf(fp,"\tmov\trbx,\trbp\n");
	fprintf(fp,"\tadd\trbx,\t%d\n",offset);

	if(v->baseType==3)
	{
		if(v->low==-1 || v->high==-1)
		{
			fprintf(fp,"\tmov\trbx,\t[rbx]\n");
		}
	}

}


void codeGen(quad_row *q,char * codeFile)
{


	for(int i=0;i<1000;i++)
	{
		temp_arr[i][0]=-1;
		temp_arr[i][1]=-1;
	}
	FILE* fp;

	fp= fopen(codeFile,"w");
	//fp=stdout;
	if(fp==NULL)
	{
		printf("some error while opening the code file\n");
		return;
	}

	fprintf(fp,"global  main\nextern printf,scanf\nsection .text\n");

	symnode* current_func=sym_root->child;
	int func_start =1;
	int main_label=-1;
	char buf[12];
	int reg;
	int regop1,regop2,regop3,regres;

	freeAllReg();

	const char *OPNames[] = {"START","END","NOP","CALL","RET","DEC","PRINT","INP","ARR_GET","ARR_ASSIGN","BRANCH", "JUMP","ASSIGN","INC","EQUATE","UNARY","PLUS","MINUS","MUL","DIV,","AND","OR","LT","LE","GT","GE","EQ","NE","PUSH","POP","POP2"};

	if(q->next!=NULL && q->next->op==NOP)
	{
		main_label=q->next->srno;
	}

	while(q!=NULL)
	{
		
		//printf("Next Line\n");
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
		//printf("Next Line\n");
		
		if(q->op!=NOP && q->op!=DEC && q->op!=JUMP && q->op!=START)
		{
			fprintf(fp,"Label%d:\n",q->srno);
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
									regop1=getVarReg(q->val[0].v_item->lowNode->data->v_item,q,fp);
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
									regop2=getVarReg(q->val[0].v_item->highNode->data->v_item,q,fp);
								}
								else
									printf("Error Dynamic Array\n");
							}
							else
							{
								regop2=getReg(2,q);
								fprintf(fp,"\tmov\t%s,\t%d\n",registers[regop2],q->val[0].v_item->high);	
							}
							
							regop3=getReg(2,q);
							fprintf(fp,"\txor\t%s,\t%s\n",registers[regop3],registers[regop3]);
							fprintf(fp,"\tmov\t%s,\t%s\n",registers[regop3],registers[regop2]);
							fprintf(fp,"\tsub\t%s,\t%s\n",registers[regop3],registers[regop1]);
							fprintf(fp,"\tinc\t%s\n",registers[regop3]);
							
							if(q->val[0].v_item->eleType==0)
								fprintf(fp,"\tadd\t%s,\t%s\n",registers[regop3],registers[regop3]);

							fprintf(fp,"\tmov\t[rbp%d],\trsp\n",q->val[0].v_item->offset);
							fprintf(fp,"\tpush\t%s\n",registers[regop2]);
							fprintf(fp,"\tpush\t%s\n",registers[regop1]);
							fprintf(fp,"\tsub\trsp,\t%s\n",registers[regop3+32]);		

							freeReg(regop1);
							freeReg(regop2);
							freeReg(regop3);
						}
						else
						{
							setAddr(q->val[0].v_item,fp);
							fprintf(fp,"\tmov\tword [RBX],%d\n",q->val[0].v_item->low);
							fprintf(fp,"\tmov\tword [RBX+2],%d\n",q->val[0].v_item->high);
						}	
					}
				}
				else
					printf("error dec 1 \n");

				//printf("OUTSIDE DEC\n");
				break;
			}
			case INC:
			{
				if(q->tag[0]==0)
				{
					setAddr(q->val[0].v_item,fp);
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
					regop1=getVarReg(q->val[0].v_item,q,fp);
				else if(q->tag[0]==3)
					regop1=getTemp(q->val[0].t_item,q);

				else printf("Relation operator error 1 \n");

				if(q->tag[1]==1)
					regop2=getTok(q->val[1].tk_item,q,fp);
				else if(q->tag[1]==3)
					regop2=getTemp(q->val[1].t_item,q);
				else printf("Relation operator error 2 \n");

				if(q->tag[2]==3)
				{
					regop3=getReg(1,q);
					reg_arr[reg%16]=q->val[2].t_item->offset;
					temp_arr[q->val[2].t_item->offset][0]=regop3;
					temp_arr[q->val[2].t_item->offset][1]=0;
				}
				else printf("Relation operator error 3 \n");

				fprintf(fp,"\txor\t%s,\t%s\n",registers[regop3],registers[regop3]);
				fprintf(fp,"\tCMP\t%s,\t%s\n",registers[regop1],registers[regop2]);
				
				if(q->op==LT)
					fprintf(fp,"\tJGE\tJRP%d\n",q->srno);
				else if(q->op==LE)
					fprintf(fp,"\tJG\tJRP%d\n",q->srno);
				else if(q->op==GT)
					fprintf(fp,"\tJLE\tJRP%d\n",q->srno);
				else if(q->op==GE)
					fprintf(fp,"\tJL\tJRP%d\n",q->srno);
				else if(q->op==EQ)
					fprintf(fp,"\tJNE\tJRP%d\n",q->srno);
				else if(q->op==EQUATE)
					fprintf(fp,"\tJNE\tJRP%d\n",q->srno);
				else if(q->op==NE)
					fprintf(fp,"\tJE\tJRP%d\n",q->srno);
				
				fprintf(fp,"\tmov\t%s,\t1\n",registers[regop3]);

				fprintf(fp,"JRP%d:\n",q->srno);

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

				if(q->tag[1]!=2 || q->tag[2]!=2)
				{	
					printf("error branch 2-3 \n");
					break;
				}
				fprintf(fp,"\tCMP\t%s,\t0\n",registers[regop1]);
				fprintf(fp,"\tJE Label%d\n",q->val[2].qr_item->srno);
				fprintf(fp,"\tJMP Label%d\n",q->val[1].qr_item->srno);
				break;	
			}
			case UNARY:
			{
				if(q->tag[1]==0)
					regop1=getVarReg(q->val[1].v_item,q,fp);
				else if(q->tag[1]==1)
					regop1=getTok(q->val[1].tk_item,q,fp);
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
					regop1=getTok(q->val[1].tk_item,q,fp);
				
				else if(q->tag[1]==0)
					regop1=getVarReg(q->val[1].v_item,q,fp);
				
				else
					printf("Invalid operand arr_get 2\n");

				if(q->tag[2]==0)
				{
					setAddr(q->val[2].v_item,fp);

					if(q->val[2].v_item->low==-1 || q->val[2].v_item->high==-1)
					{
						if(q->tag[1]==0)
						{
			
							fprintf(fp,"\tcmp\t%s,\t[RBX]\n",registers[regop1]);
							fprintf(fp,"\tJGE TESTH%d\n",q->srno);
							fprintf(fp,"\tmov\trdi,\tErrorLow\n");
							fprintf(fp,"\txor\trax,\trax\n");
							fprintf(fp,"\tPUSH\t%s\n",registers[regop1]);
							fprintf(fp,"\tPUSH\tRBX\n");
							fprintf(fp,"\tPUSH\tRBP\n");
							fprintf(fp,"\tcall\tprintf\n");
							fprintf(fp,"\tPOP\tRBP\n");
							fprintf(fp,"\tPOP\tRBX\n");
							fprintf(fp,"\tPOP\t%s\n",registers[regop1]);

							fprintf(fp,"\tmov\trax,\t60\t\t; system call for exit\n");
							fprintf(fp,"\txor\trdi,\trdi\t\t; exit code 0\n");
							fprintf(fp,"\tsyscall\t\t; invoke operating system to exit\n");
	
							fprintf(fp,"TESTH%d:\n",q->srno);
							fprintf(fp,"\tcmp\t%s,\t[RBX+2]\n",registers[regop1]);
							fprintf(fp,"\tJLE TESTC%d\n",q->srno);
							fprintf(fp,"\tmov\trdi,\tErrorHigh\n");
							fprintf(fp,"\txor\trax,\trax\n");
							fprintf(fp,"\tPUSH\t%s\n",registers[regop1]);
							fprintf(fp,"\tPUSH\tRBX\n");
							fprintf(fp,"\tPUSH\tRBP\n");
							fprintf(fp,"\tcall\tprintf\n");
							fprintf(fp,"\tPOP\tRBP\n");
							fprintf(fp,"\tPOP\tRBX\n");
							fprintf(fp,"\tPOP\t%s\n",registers[regop1]);

	
							fprintf(fp,"\tmov\trax,\t60\t\t; system call for exit\n");
							fprintf(fp,"\txor\trdi,\trdi\t\t; exit code 0\n");
							fprintf(fp,"\tsyscall\t\t; invoke operating system to exit\n");
						
							fprintf(fp,"TESTC%d:\n",q->srno);
						}

						fprintf(fp,"\tsub\t%s,\t[RBX]\n",registers[regop1]);
					}
					else
					{

						if(q->tag[1]==0)
						{
							fprintf(fp,"\tcmp\t%s,\t%d\n",registers[regop1],q->val[2].v_item->low);
							fprintf(fp,"\tJGE TESTH%d\n",q->srno);
							fprintf(fp,"\tmov\trdi,\tErrorLow\n");
							fprintf(fp,"\txor\trax,\trax\n");
							fprintf(fp,"\tPUSH\t%s\n",registers[regop1]);
							fprintf(fp,"\tPUSH\tRBX\n");
							fprintf(fp,"\tPUSH\tRBP\n");
							fprintf(fp,"\tcall\tprintf\n");
							fprintf(fp,"\tPOP\tRBP\n");
							fprintf(fp,"\tPOP\tRBX\n");
							fprintf(fp,"\tPOP\t%s\n",registers[regop1]);
	
							fprintf(fp,"\tmov\trax,\t60\t\t; system call for exit\n");
							fprintf(fp,"\txor\trdi,\trdi\t\t; exit code 0\n");
							fprintf(fp,"\tsyscall\t\t; invoke operating system to exit\n");
	
							fprintf(fp,"TESTH%d:\n",q->srno);
							fprintf(fp,"\tcmp\t%s,\t%d\n",registers[regop1],q->val[2].v_item->high);
							fprintf(fp,"\tJLE TESTC%d\n",q->srno);
							fprintf(fp,"\tmov\trdi,\tErrorHigh\n");
							fprintf(fp,"\txor\trax,\trax\n");
							fprintf(fp,"\tPUSH\t%s\n",registers[regop1]);
							fprintf(fp,"\tPUSH\tRBX\n");
							fprintf(fp,"\tPUSH\tRBP\n");
							fprintf(fp,"\tcall\tprintf\n");
							fprintf(fp,"\tPOP\tRBP\n");
							fprintf(fp,"\tPOP\tRBX\n");
							fprintf(fp,"\tPOP\t%s\n",registers[regop1]);
	
							fprintf(fp,"\tmov\trax,\t60\t\t; system call for exit\n");
							fprintf(fp,"\txor\trdi,\trdi\t\t; exit code 0\n");
							fprintf(fp,"\tsyscall\t\t; invoke operating system to exit\n");				
					
							fprintf(fp,"TESTC%d:\n",q->srno);
						}

						fprintf(fp,"\tsub\t%s,\t%d\n",registers[regop1],q->val[2].v_item->low);
					}

					fprintf(fp,"\tadd\trbx,\t4\n");
				
					fprintf(fp,"\tmovsx\tRSI,\t%s\n",registers[regop1]);

					if(q->val[2].v_item->eleType==0)
						fprintf(fp,"\tmov\t[RBX+2*RSI],\t%s\n",registers[regop2]);
					else if(q->val[2].v_item->eleType==2)
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
				if(q->tag[1]==1)
					regop1=getTok(q->val[1].tk_item,q,fp);
				else if(q->tag[1]==3)
					regop1=getTemp(q->val[1].t_item,q);
				else if(q->tag[1]==0)
					regop1=getVarReg(q->val[1].v_item,q,fp);
				else
					printf("Invalid operand arr_get 2\n");

				if(q->tag[0]==0)
					setAddr(q->val[0].v_item,fp);
				else
					printf("Invalid operand arr_get 1\n");

				if(q->val[0].v_item->low==-1 || q->val[0].v_item->high==-1)
				{
					if(q->tag[1]==0||q->tag[1]==3)
					{
			
						fprintf(fp,"\tcmp\t%s,\t[RBX]\n",registers[regop1]);
						fprintf(fp,"\tJGE TESTH%d\n",q->srno);
						fprintf(fp,"\tmov\trdi,\tErrorLow\n");
						fprintf(fp,"\txor\trax,\trax\n");
						fprintf(fp,"\tPUSH\t%s\n",registers[regop1]);
						fprintf(fp,"\tPUSH\tRBX\n");
						fprintf(fp,"\tPUSH\tRBP\n");
						fprintf(fp,"\tcall\tprintf\n");
						fprintf(fp,"\tPOP\tRBP\n");
						fprintf(fp,"\tPOP\tRBX\n");
						fprintf(fp,"\tPOP\t%s\n",registers[regop1]);

						fprintf(fp,"\tmov\trax,\t60\t\t; system call for exit\n");
						fprintf(fp,"\txor\trdi,\trdi\t\t; exit code 0\n");
						fprintf(fp,"\tsyscall\t\t; invoke operating system to exit\n");
	
						fprintf(fp,"TESTH%d:\n",q->srno);
						fprintf(fp,"\tcmp\t%s,\t[RBX+2]\n",registers[regop1]);
						fprintf(fp,"\tJLE TESTC%d\n",q->srno);
						fprintf(fp,"\tmov\trdi,\tErrorHigh\n");
						fprintf(fp,"\txor\trax,\trax\n");
						fprintf(fp,"\tPUSH\t%s\n",registers[regop1]);
						fprintf(fp,"\tPUSH\tRBX\n");
						fprintf(fp,"\tPUSH\tRBP\n");
						fprintf(fp,"\tcall\tprintf\n");
						fprintf(fp,"\tPOP\tRBP\n");
						fprintf(fp,"\tPOP\tRBX\n");
						fprintf(fp,"\tPOP\t%s\n",registers[regop1]);

	
						fprintf(fp,"\tmov\trax,\t60\t\t; system call for exit\n");
						fprintf(fp,"\txor\trdi,\trdi\t\t; exit code 0\n");
						fprintf(fp,"\tsyscall\t\t; invoke operating system to exit\n");
						
						fprintf(fp,"TESTC%d:\n",q->srno);
					}

					fprintf(fp,"\tsub\t%s,\t[RBX]\n",registers[regop1]);
				}
				else
				{

					if(q->tag[1]==0||q->tag[1]==3)
					{
						fprintf(fp,"\tcmp\t%s,\t%d\n",registers[regop1],q->val[0].v_item->low);
						fprintf(fp,"\tJGE TESTH%d\n",q->srno);
						fprintf(fp,"\tmov\trdi,\tErrorLow\n");
						fprintf(fp,"\txor\trax,\trax\n");
						fprintf(fp,"\tPUSH\t%s\n",registers[regop1]);
						fprintf(fp,"\tPUSH\tRBX\n");
						fprintf(fp,"\tPUSH\tRBP\n");
						fprintf(fp,"\tcall\tprintf\n");
						fprintf(fp,"\tPOP\tRBP\n");
						fprintf(fp,"\tPOP\tRBX\n");
						fprintf(fp,"\tPOP\t%s\n",registers[regop1]);
	
						fprintf(fp,"\tmov\trax,\t60\t\t; system call for exit\n");
						fprintf(fp,"\txor\trdi,\trdi\t\t; exit code 0\n");
						fprintf(fp,"\tsyscall\t\t; invoke operating system to exit\n");
	
						fprintf(fp,"TESTH%d:\n",q->srno);
						fprintf(fp,"\tcmp\t%s,\t%d\n",registers[regop1],q->val[0].v_item->high);
						fprintf(fp,"\tJLE TESTC%d\n",q->srno);
						fprintf(fp,"\tmov\trdi,\tErrorHigh\n");
						fprintf(fp,"\txor\trax,\trax\n");
						fprintf(fp,"\tPUSH\t%s\n",registers[regop1]);
						fprintf(fp,"\tPUSH\tRBX\n");
						fprintf(fp,"\tPUSH\tRBP\n");
						fprintf(fp,"\tcall\tprintf\n");
						fprintf(fp,"\tPOP\tRBP\n");
						fprintf(fp,"\tPOP\tRBX\n");
						fprintf(fp,"\tPOP\t%s\n",registers[regop1]);
	
						fprintf(fp,"\tmov\trax,\t60\t\t; system call for exit\n");
						fprintf(fp,"\txor\trdi,\trdi\t\t; exit code 0\n");
						fprintf(fp,"\tsyscall\t\t; invoke operating system to exit\n");				
					
						fprintf(fp,"TESTC%d:\n",q->srno);
					}

					fprintf(fp,"\tsub\t%s,\t%d\n",registers[regop1],q->val[0].v_item->low);
				}

				fprintf(fp,"\tadd\trbx,\t4\n");

				fprintf(fp,"\tmovsx\tRSI,\t%s\n",registers[regop1]);

				if(q->tag[2]==3)
				{
					regop2=getTemp(q->val[2].t_item,q);
					if(q->val[0].v_item->eleType==0)
						fprintf(fp,"\tmov\t%s,\t[RBX+2*RSI]\n",registers[regop2]);
					else if(q->val[0].v_item->eleType==2)
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

				//printf("here assign\n");
				if(q->tag[0]==3 && q->tag[2]==3)
				{
					regop1=getTemp(q->val[0].t_item,q);
					regop2=getTemp(q->val[2].t_item,q);
					fprintf(fp,"\tmov\t%s,\t%s\n",registers[regop2],registers[regop1]);
					break;
				}		

				if(q->tag[0]==0)
					regop1=getVarReg(q->val[0].v_item,q,fp);
				else if(q->tag[0]==1)
					regop1=getTok(q->val[0].tk_item,q,fp);
				else if(q->tag[0]==3)
					regop1=getTemp(q->val[0].t_item,q);
				else
					printf("Invalid op to assign 1\n");

				//printf("here assign also\n");

				if(q->tag[2]==0)
				{
					setAddr(q->val[2].v_item,fp);
					fprintf(fp,"\tmov\t[RBX],\t%s\n",registers[regop1]);
				}
				else if(q->tag[2]==3)
				{
					temp_arr[q->val[2].t_item->offset][0]=regop1;
					reg_arr[regop1%16]=q->val[2].t_item->offset;
					temp_arr[q->val[2].t_item->offset][1]=0;
				}
				else
					printf("Invalid op to assign 2\n");
				
				if(q->tag[2]==0 && (q->tag[0]==1||q->tag[0]==3))
					freeReg(regop1);
				//printf("here assign\n");

				break;									
	
			}
			case CALL:
			{
				fprintf(fp,"\tPUSH\tRBP\n");
				fprintf(fp,"\tcall\tLabel%d\n",q->val[0].qr_item->srno);
				fprintf(fp,"\tPOP\tRBP\n");
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
				deallocMemory(current_func,fp);
				fprintf(fp,"\tret\n");
				current_func=current_func->right;
				func_start=1;
				break;
			}
			case NOP:
			{
				//printf("INSIDE NOP: %d %d\n",main_label,func_start);
				if(q->srno==main_label)
					fprintf(fp,"main:\n");
				else
				{
      					fprintf(fp,"Label%d:\n", q->srno);
				}
				
				if(func_start==1)
				{
					allocMemory(current_func,fp);//new function
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
					setAddr(q->val[0].v_item,fp);
					if(q->val[0].v_item->baseType==0)
						fprintf(fp,"\tPUSH\tword [rbx]\n");
					if(q->val[0].v_item->baseType==1)
					{
						fprintf(fp,"dec\trsp\n");
						fprintf(fp,"\tmov\tAL, byte [rbx]\n");
						fprintf(fp,"\tmov\trbx,\trsp\n");
						fprintf(fp,"\tmov\t[rbx],\tAl\n");
					}
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
					if(q->val[0].v_item->baseType==0)
					{
						setAddr(q->val[0].v_item,fp);
						fprintf(fp,"\tPOP\tword [rbx]\n");
					}
					else if(q->val[0].v_item->baseType==2)
					{
						fprintf(fp,"\tmov\trbx,\trsp\n");
						fprintf(fp,"\tmov\tAL,\tbyte [rbx]\n");
						setAddr(q->val[0].v_item,fp);
						fprintf(fp,"\tmov\t[rbx],\tAl\n");
						fprintf(fp,"inc\trsp\n");
					}
					else if(q->val[0].v_item->baseType==3)
						fprintf(fp,"\tadd\trsp,\t8\n");
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
					if(q->val[0].v_item->baseType==2)
					{
						fprintf(fp,"inc\trsp\n");
					}					

					else if(q->val[0].v_item->baseType==0)
					{
						fprintf(fp,"\tadd\trsp,\t2\n");
					}

					else if(q->val[0].v_item->baseType==3)
					{
						fprintf(fp,"\tadd\trsp,\t8\n");
					}

					else
					{
						printf("Type not handled\n");
					}					
				
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
					temp_arr[q->val[2].t_item->offset][0]=regop1;
					reg_arr[regop1%16]=q->val[2].t_item->offset;
					temp_arr[q->val[2].t_item->offset][1]=0;
				}
				else
					printf("Invalid Operand for Arith Operator");

				
				switch(q->op)
				{
					case PLUS:
					{
						fprintf(fp,"\tADD\t%s,\t%s\n",registers[regop1],registers[regop2]);
						break;	
					}
					case MINUS:
					{
						fprintf(fp,"\tSUB\t%s,\t%s\n",registers[regop1],registers[regop2]);
						break;	
					}
					case MUL:
					{
						fprintf(fp,"\tIMUL\t%s,\t%s\n",registers[regop1],registers[regop2]);
						break;	
					}
					case DIV:
					{
						fprintf(fp,"\txor\tDX,\tDX\n");
						fprintf(fp,"\tmov\tAX,\t%s\n",registers[regop1]);
						fprintf(fp,"\tIDIV\t%s\n",registers[regop2]);
						fprintf(fp,"\tmov\t%s,\tAX\n",registers[regop1]);
						break;
					}
					case AND:
					{
						fprintf(fp,"\tAND\t%s,\t%s\n",registers[regop1],registers[regop2]);
						break;	
					}
					case OR:
					{
						fprintf(fp,"\tOR\t%s,\t%s\n",registers[regop1],registers[regop2]);
						break;	
					}

				}
				
				freeReg(regop2);

				break;	
			}
			case PRINT:
			{
				if((q->tag[0]!=2 && q->tag[0]!=-1))
				{	
					fprintf(fp,"\tmov\trdi,\tOutput\n");
					fprintf(fp,"\txor\trax,\trax\n");
					fprintf(fp,"PUSH\tRBP\n");
					fprintf(fp,"\tcall\tprintf\n");
					fprintf(fp,"POP\tRBP\n");

					if(q->tag[0]==3)
						reg=getTemp(q->val[0].t_item,q);//getTemp	
					else  if(q->tag[0]==0)
						reg=getVarReg(q->val[0].v_item,q,fp);//getVar
					else if(q->tag[0]==1)
						reg=getTok(q->val[0].tk_item,q,fp);//getTok
					if(q->tag[0]==0 && q->val[0].v_item->baseType==3)
					{

						fprintf(fp,"\tmov\trbx,\t%s\n",registers[reg]);
						if(q->val[0].v_item->low==-1 || q->val[0].v_item->high==-1)
						{
							regop1=getReg(2,q);//new fucntion
							regop2=getReg(2,q);
							fprintf(fp,"\tmov\t%s,\t[RBX]\n",registers[regop1]);
							fprintf(fp,"\tmov\t%s,\t[RBX+2]\n",registers[regop2]);
							fprintf(fp,"\tsub\t%s,\t%s\n",registers[regop2],registers[regop1]);
							freeReg(regop1);
						}
						else
						{
							regop2=getReg(2,q);
							fprintf(fp,"\tmov\t%s,\t%d\n",registers[regop2],q->val[0].v_item->high-q->val[0].v_item->low);
						}
						fprintf(fp,"\tinc\t%s\n",registers[regop2]);
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
							fprintf(fp,"\tmov\trdi,\tIntOutput\n");
							fprintf(fp,"\tmovsx\trsi,\tword [RBX]\n");
							fprintf(fp,"\tadd\trbx,2\n");
						}

						fprintf(fp,"\txor\trax,\trax\n");
						fprintf(fp,"PUSH\t%s\n",registers[regop2]);
						fprintf(fp,"PUSH\tRBX\n");
						fprintf(fp,"PUSH\tRBP\n");
						fprintf(fp,"\tcall\tprintf\n");
						fprintf(fp,"POP\tRBP\n");
						fprintf(fp,"POP\tRBX\n");
						fprintf(fp,"POP\t%s\n",registers[regop2]);

						if(q->val[0].v_item->eleType==1)
						{
							printf("REAL Values are not handled");
						}						
						fprintf(fp,"\tdec\t%s\n",registers[regop2]);
						fprintf(fp,"\tjnz PLA%d\n",q->srno);
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
							fprintf(fp,"\tmovsx\trsi,\t%s\n",registers[reg]);
						}
						else
						{
							printf("printf only handles bool and int");
						}
						
						fprintf(fp,"\txor\trax,\trax\n");
						fprintf(fp,"PUSH\tRBP\n");
						fprintf(fp,"\tcall\tprintf\n");
						fprintf(fp,"POP\tRBP\n");
					}
					freeReg(reg);
				}
				//printf("OUTSIDE PRINTF\n");
				break;
			}
			case INP:
			{
				
				//printf("INSIDE INP\n");
				if(q->tag[0]==0)
				{
					setAddr(q->val[0].v_item,fp);
					if(q->val[0].v_item->baseType==0 ||q->val[0].v_item->baseType==2)
					{
						if(q->val[0].v_item->baseType==0)
							fprintf(fp,"\tmov\trdi,\tIntInputString\n");
						if(q->val[0].v_item->baseType==2)	
							fprintf(fp,"\tmov\trdi,\tBoolInputString\n");

						fprintf(fp,"\txor\trax,\trax\n");
	
						fprintf(fp,"PUSH\tRBX\n");
						fprintf(fp,"PUSH\tRBP\n");
						fprintf(fp,"\tcall printf\n");
						fprintf(fp,"POP\tRBP\n");
						fprintf(fp,"POP\tRBX\n");
					
						fprintf(fp,"\tmov\trdi,\tIntInput\n");
						fprintf(fp,"\tmov\trsi,\tintvar\t\n");
						fprintf(fp,"\txor\trax,\trax\n");

						fprintf(fp,"PUSH\tRBX\n");
						fprintf(fp,"PUSH\tRBP\n");
						fprintf(fp,"\tcall\tscanf\n");
						fprintf(fp,"POP\tRBP\n");
						fprintf(fp,"POP\tRBX\n");
				
						fprintf(fp,"\tmov\tax,\t[intvar]\n");
						if(q->val[0].v_item->baseType==0)
							fprintf(fp,"\tmov\t[RBX],\tax\n");
						if(q->val[0].v_item->baseType==2)	
							fprintf(fp,"\tmov\t[RBX],\tal\n");

					}
					else if(q->val[0].v_item->baseType==3)
					{
						if(q->val[0].v_item->low==-1 || q->val[0].v_item->high==-1)
						{
							regop1=getReg(2,q);//new fucntion
							regop2=getReg(2,q);
							fprintf(fp,"\tmov\t%s,\t[RBX]\n",registers[regop1]);
							fprintf(fp,"\tmov\t%s,\t[RBX+2]\n",registers[regop2]);
							fprintf(fp,"\tsub\t%s,\t%s\n",registers[regop2],registers[regop1]);
							freeReg(regop1);
						}
						else
						{
							regop2=getReg(2,q);
							fprintf(fp,"\tmov\t%s,\t%d\n",registers[regop2],q->val[0].v_item->high-q->val[0].v_item->low);
						}

						fprintf(fp,"\tinc\t%s\n",registers[regop2]);
						fprintf(fp,"\tadd\trbx,\t4\n");

						if(q->val[0].v_item->eleType==0)
							fprintf(fp,"\tmov\trdi,\tIntArrInputString\n");
						if(q->val[0].v_item->eleType==2)	
							fprintf(fp,"\tmov\trdi,\tBoolArrInputString\n");
						
						fprintf(fp,"\tmovsx\trsi,\t%s\n",registers[regop2]);

						fprintf(fp,"\txor\trax,\trax\n");
						
						fprintf(fp,"PUSH\tRBX\n");
						fprintf(fp,"PUSH\tRBP\n");
						fprintf(fp,"PUSH\t%s\n",registers[regop2]);
						fprintf(fp,"\tcall printf\n");
						fprintf(fp,"POP\t%s\n",registers[regop2]);
						fprintf(fp,"PUSH\tRBP\n");
						fprintf(fp,"PUSH\tRBX\n");

						fprintf(fp,"\tILA%d:\n",q->srno);
					
						fprintf(fp,"\tmov\trdi,\tIntInput\n");
						fprintf(fp,"\tmov\trsi,intvar\t\n");
						fprintf(fp,"\txor\trax,\trax\n");
						fprintf(fp,"PUSH\tRBX\n");
						fprintf(fp,"PUSH\tRBP\n");
						fprintf(fp,"PUSH\t%s\n",registers[regop2]);
						fprintf(fp,"\tcall scanf\n");
						fprintf(fp,"POP\t%s\n",registers[regop2]);
						fprintf(fp,"PUSH\tRBP\n");
						fprintf(fp,"PUSH\tRBX\n");
							
						fprintf(fp,"\tmov\tax,\t[intvar]\n");
						if(q->val[0].v_item->eleType==0)
						{
							fprintf(fp,"\tmov\t[RBX],\tax\n");
							fprintf(fp,"\tadd\trbx,2\n");
						}
						if(q->val[0].v_item->eleType==2)	
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
						freeReg(regop2);
					}
					else
						printf("real values not handled");					
				}
				else
				{
					printf("INPUT Invalid \n");
				}

				//printf("OUTSIDE INP\n");
				break;
			}
		}

		//regState();
		q=q->next;	
	}

	fprintf(fp,"section .data\n");
	fprintf(fp,"ErrorHigh:\n\tdb \"Index Greater Than Bound! RUNTIME ERROR! \",10,0\n");
	fprintf(fp,"ErrorLow:\n\tdb \"Index Smaller Than Bound! RUNTIME ERROR! \",10,0\n");
	fprintf(fp,"Output:\n\tdb \"Output =\",10,0\n");
	fprintf(fp,"BoolTrue:\n\tdb \"true\",10,0\n");
	fprintf(fp,"BoolFalse:\n\tdb \"false\",10,0\n");
	fprintf(fp,"IntOutput:\n\tdb \"%%hd\",10,0\n");
	fprintf(fp,"IntInput:\n\tdb \"%%hd\",0,\n");
	fprintf(fp,"IntInputString:\n\tdb \"Please Enter an Integer\",10,0\n");
	fprintf(fp,"BoolInputString:\n\tdb \"Please Enter 0 for false and 1 for true\",10,0\n");
	fprintf(fp,"IntArrInputString:\n\tdb \"Please Enter an %%d Integers\",10,0 \n");
	fprintf(fp,"BoolArrInputString:\n\tdb \"Please Enter %%d 0s or 1s for false and true\",10,0\n");
	fprintf(fp,"section .bss\n");
	fprintf(fp,"intvar: resw 1\n");
	fclose(fp);

}

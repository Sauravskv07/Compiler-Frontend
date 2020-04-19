#include <stdio.h>
/*
int getVar(var_item* var)//takes var as input, loads it into reg if not present otherwise gives the reg in which it is present;
int getTemp(int temp_index)//takes as input a temp and returns  the reg in which it is present; if temp not present then sets temp into new reg;
setVar(var_item* var,int reg)//tells that var will be set to new value in reg
int getTok(tk_item* token)//copies token in a register and returns it
int getReg()//returns a free register or frees some register and then return
getRegSpecific(ECX);
{
return 0;
}

*/

void codeGen(quad_row *q)
{

	FILE* fp= fopen("code.asm",'w');
	if(fp==NULL)
	{
		printf("some error while opening the code file\n");
		return;
	}

	symnode* current_func=symroot->child;
	int func_start =1;
	int main_label=-1;
	char buf[12];
	int reg;
	int regop1,regop2,regop3,regres;

	while(q!=NULL)
	{
		switch(q->op)
		{

			case ASSIGN:
			{
				if(q->tag[0]==0)
					
			}
			case START:
			{
				
				break;

			}
			case END:
			{
				fprintf(fp,"\tmov\trax,\t60\t\t; system call for exit\n\txor\trdi,\trdi\t\t; exit code 0\n\tsyscall\t\t; invoke operating system to exit\n");
				break;
			}
			case RET:
			{
				fprintf(fp,"\tret\n");
				current_func=current_func->right;
				func_start=1;
			}
			case NOP:
			{
				if(q->srno==main_label)
					fprintf(fp,"main:\n");
				else
				{
      					fprintf(fp,"Label%d:\n", q->srno);
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
			}

			case PUSH:
			{
				if(q->tag[0]==0)
				{
					reg=getVar(q->val[0].v_item)
					fprintf(fp,"\tPUSH\t%s\n",registers[reg]);
				}
				else
					printf("Other types of push not possible\n");
				break;
			}

			case POP:
			{
				if(q->tag[0]==0)
				{
					reg=getVar(q->val[0].v_item);
					fprintf(fp,"\tPOP\t%s\n",registers[reg]);
					if(q->val[0].v_item->baseType<=2)
					{
						setAddr(q->val[0].v_item,1);
						fprintf(fp,"\tmov\t[RBX],\t%s\n",registers[reg]);
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
				if(q->tag[0]==0)
					regop1=getVar(q->val[0].v_item);//new function
				else if(q->tag[0]==1)
					regop1=getTok(q->val[0].tk_item);//new function
				else if(q->tag[0]==3)
					regop1=getTemp(q->val[0].t_item);
				else
					printf("Invalid Operand for Arith Operator");

				if(q->tag[1]==0)
					regop2=getVar(q->val[1].v_item);
				else if(q->tag[1]==1)
					regop1=getTok(q->val[1].tk_item);
				else if(q->tag[1]==3)
					regop2=getTemp(q->val[1].t_item);
				else
					printf("Invalid Operand for Arith Operator");	
				
				if(q->tag[2]==0)
				{	
					setAddr(q->val[2].v_item,1);//new function
					fprintf(fp,"\tmov\t[RBX],\t%s\n",registers[regop1]);
				}
				else if(q->tag[2]==3)
				{
					regres=getTemp(q->val[2].t_item);//new function
					fprintf(fp,"\tMOV\t%s,\t%s",regres,regop1);
				}
				else
					printf("Invalid Operand for Arith Operator");

				
				switch(q->op)
				{
					case PLUS:
					{
						if(q->tag[2]==3)
							fprintf(fp,"\tADD\t%s,\t%s",registers[regres],registers[regop2]);
						else
							fprintf(fp,"\tADD\t[RBX],\t%s\n",registers[regop2]);
						break;	
					}
					case MINUS:
					{
						if(q->tag[2]==3)
							fprintf(fp,"\tSUB\t%s,\t%s",registers[regres],registers[regop2]);
						else
							fprintf(fp,"\tSUB\t[RBX],\t%s\n",registers[regop2]);
						break;	
					}
					case MUL:
					{
						if(q->tag[2]==3)
							fprintf(fp,"\tMUL\t%s,\t%s",registers[regres],registers[regop2]);
						else
							fprintf(fp,"\tMUL\t[RBX],\t%s\n",registers[regop2]);
						break;	
					}
					case DIV:
					{
						if(q->tag[2]==3)
							fprintf(fp,"\tDIV\t%s,\t%s",registers[regres],registers[regop2]);
						else
							fprintf(fp,"\tDIV\t[RBX],\t%s\n",registers[regop2]);
						break;	
					}
					case AND:
					{
						if(q->tag[2]==3)
							fprintf(fp,"\tAND\t%s,\t%s",registers[regres],registers[regop2]);
						else
							fprintf(fp,"\tAND\t[RBX],\t%s\n",registers[regop2]);
						break;	
					}
					case OR:
					{
						if(q->tag[2]==3)
							fprintf(fp,"\tOR\t%s,\t%s",registers[regres],registers[regop2]);
						else
							fprintf(fp,"\tOR\t[RBX],\t%s\n",registers[regop2]);
						break;	
					}

				}
				
				break;	
			}
			case PRINT:
			{
				if((q->tag[0]!=2)
				{	
					if(q->tag[0]==3)
						reg=getTemp(q->val[0].t_item);	
					else  if(q->tag[0]==0)
						reg=getVar(q->val[0].v_item);
					else if(q->tag[0]==1)
						reg=getTok(q->val[0].tk_item);
						
					if(q->tag[0]==0 && q->val[0].v_item->baseType==3)
					{
						fprintf("\tmov\trbx,\t%s\n",registers[reg]);
						regop1=getReg(2);//new fucntion
						regop2=getReg(2);
						fprintf(fp,"\tmov\t%s,\t[RBX]\n",registers[regop1]);
						fprintf(fp,"\tmov\t%s,\t[RBX-2]\n",registers[regop2]);
						fprintf(fp,"\tsub\t%s,\t%s\n",registers[regop2],registers[regop1]);
						fprintf(fp,"inc\t%s\n",registers[regop2]);
						regop3=regop1-16;//make it single byte
						fprintf(fp,"\tPLA%d:\n",q->srno);
						if(q->val[0].v_item->eleType==2)
						{
				
							fprintf(fp,"\tmov\t%s,\t[RBX+4]\n",registers[regop3]);
							fprintf(fp,"\tCMP\t%s,\t0\n\tJZ PLF%d\n",registers[regop3],qr->srno);
							fprintf(fp,"\tmov\trdi,\tBoolTrue\n\tJMP\tPLT%s\n",qr->srno);
							fprintf(fp,"PLF%d:\n",qr->srno);
							fprintf(fp,"\tmov\trdi,\tBoolFalse\n");
							fprintf(fp,"PLT%d:\n",qr->srno);
							fprintf("\tadd\trbx,1");				
						}
						else if(q->val[0].v_item->eleType==0)
						{
							fprintf(fp,"\tmov\tesi,\t[RBX+4]\n",registers[regop1]);
							fprintf("\tadd\trbx,2");
						}

						fprintf(fp,"\txor\trax,\trax\n");
						fprintf(fp,"\tcall printf\n");

						if(q->val[0].v_item->eleType==1)
						{
							printf("REAL Values are not handled");
						}						
						fprintf(fp,"\tdec\t%s\n",registers[regop2]);
						fprintf(fp,"\tjnz PLA%d\n",q->srno);
						
					}
					else
					{
						if((q->tag[0]==3 && q->val[0].t_item.baseType==2) ||(q->tag[0]==0 && q->val[0].v_item.baseType==2)||(q->tag[0]==1 && (q->val[0].tk_item.index==8 || q->val[0].tk_item.index==9)))
						{
							fprintf(fp,"\tCMP\t%s,\t0\n\tJZ PLF%d\n",registers[reg],qr->srno);
							fprintf(fp,"\tmov\trdi,\tBoolTrue\n\tJMP\tPLT%s\n",qr->srno);
							fprintf(fp,"PLF%d:\n",qr->srno);
							fprintf(fp,"\tmov\trdi,\tBoolFalse\n");
							fprintf(fp,"PLT%d:\n",qr->srno);
						}
						else if((q->tag[0]==3 && q->val[0].t_item.baseType==0) ||(q->tag[0]==0 && q->val[0].v_item.baseType==0)||(q->tag[0]==1 && q->val[0].tk_item.index==2))
						{
							fprintf(fp,"\tmov\trdi,\tIntOutput\n");
							fprintf(fp,"\tmov\trsi,\t%s\n",registers[reg]);
						}
						else
						{
							printf("printf only handles bool and int");
						}
						
						fprintf(fp,"\txor\trax,\trax\n");
						fprintf(fp,"\tcall printf\n");
					}			
				}
				break;
			}
			case INP:
			{
				if(q->tag[0]==0)
				{
					setAddr(q->val[0].v_item,1);
					if(q->val[0].v_item.baseType==0 ||q->val[0].v_item.baseType==2)
					{
						if(q->val[0].v_item.baseType==0)
							fprintf(fp,"\tmov\trdi,\tIntInputString\n");
						if(q->val[0].v_item.baseType==2)	
							fprintf(fp,"\tmov\trdi,\tBoolInputString\n");

						fprintf(fp,"\txor\trax,\trax\n");
						fprintf(fp,"\tcall printf\n");
					
						fprintf(fp,"\tmov\trdi,\tIntInput\n");
						fprintf(fp,"\tmov\trsi,intvar\t\n");
						fprintf(fp,"\txor\trax,\trax\n");
						fprintf(fp,"\tcall\tscanf\n");
					
						fprintf("\tmov\tax,\t[intvar]\n");
						if(q->val[0].v_item.baseType==0)
							fprintf("\tmov\t[RBX],\tax\n");
						if(q->val[0].v_item.baseType==2)	
							fprintf("\tmov\t[RBX],\tal\n");

					}
					else if(q->val[0].v_item.baseType==3)
					{
						regop1=getReg(2);//new fucntion
						regop2=getReg(2);
						fprintf(fp,"\tmov\t%s,\t[RBX]\n",registers[regop1]);
						fprintf(fp,"\tmov\t%s,\t[RBX-2]\n",registers[regop2]);
						fprintf(fp,"\tsub\t%s,\t%s\n",registers[regop2],registers[regop1]);
						fprintf(fp,"inc\t%s\n",registers[regop2]);
						fprintf(fp,"\tmov\trcx,\t0\n");
						fprintf(fp,"\tadd\trbx,\t4\n");

						if(q->val[0].v_item.baseType==0)
							fprintf(fp,"\tmov\trdi,\tIntArrInputString\n");
						if(q->val[0].v_item.baseType==2)	
							fprintf(fp,"\tmov\trdi,\tBoolArrInputString\n");

						fprintf(fp,"\txor\trax,\trax\n");
						fprintf(fp,"\tcall printf\n");

						fprintf(fp,"\tILA%d:\n",q->srno);
					
						fprintf(fp,"\tmov\trdi,\tIntInput\n");
						fprintf(fp,"\tmov\trsi,intvar\t\n");
						fprintf(fp,"\txor\trax,\trax\n");
						fprintf(fp,"\tcall\tscanf\n");
							
						fprintf("\tmov\tax,\t[intvar]\n");
						if(q->val[0].v_item.baseType==0)
						{
							fprintf("\tmov\t[RBX],\tax\n");
							fprintf("\tadd\trbx,2");
						if(q->val[0].v_item.baseType==2)	
						{	
							fprintf("\tmov\t[RBX],\tal\n");
							fprintf("\tadd\trbx,1");
						}
						
						else if(q->val[0].v_item->eleType==1)
						{
							printf("REAL Values are not handled");
						}						
						fprintf(fp,"\tdec\t%s\n",registers[regop2]);
						fprintf(fp,"\tjnz PLA%d\n",q->srno);
		
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
	}

	fprintf(fp,"section .data\nIntOutput:\n\tdb \"Output is = %hd\",10,0\nIntInput:\n\tdb \"%hd\",0,\nIntInputString:\n\tdb \"Please Enter an Integer\",10,0      ;newline at the end\nBoolInputString:\n\tdb \"Please Enter 0 for false and 1 for true\",10,0      ;newline at the end");
	fprintf(fp,"section .bss\nintvar: resw 1")
	
}

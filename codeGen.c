#include <stdio.h>

int temp_arr[1000];
int reg_arr[16];
const char *registers[] = {AL, CL, DL, BL, SPL, BPL, SIL, DIL, R8B, R9B, R10B, R11B, R12B, R13B, R14B, R15B, 

void allocMemory(symnode* current_func);
{
	offset=current_func->current_offset;
	
	int i=16-offset%16;
	if(i>8)
		i=i-8;
	if(i<8)
		i=i+8;
	
	fprintf(fp,"\tsub\tRSP,\t%d\n",offset+i);
}

int getTok(tk_item* tk)
{
	int reg=0;	
	if(tk->index==2)
	{
		reg=getReg(2);
		fprintf(fp,"\nmov\t%s,\t%d\n",registers[reg],tk->val.i_val);
	}
	else if(tk->index==8)
	{
		reg=getReg(1);
		fprintf(fp,"\nmov\t%s,\t1\n",registers[reg]);
	}
	else if(tk->index==9)
	{
		reg=getReg(1);
		fprintf(fp,"\nmov\t%s,\t0\n",registers[reg]);
	}
	else
	{
		printf("Real Tokens Not Handled\n");
	}
	return reg;
}

int getVar(v_item* v)
{
	int reg;
	int offset;

	if(offset>=0)
		offset=offset+8;
	fprintf(fp,"\tmov\trbx,\trsp\n");
	fprintf(fp,"\tadd\trbx,\t%d\n",offset);

	if(v->baseType==1)
	{
		reg=getReg(1);
		fprintf(fp,"\nmov\t%s,\t[rbx]\n",registers[reg]);
	}
	if(v->baseType==0)
	{
		reg=getReg(2);
		fprintf(fp,"\nmov\t%s,\t[rbx]\n",registers[reg]);
	}
	if(v->baseType==3)
	{
		reg=getReg(4);
		if(v->low==-1 || v->high==-1)
		{
			fprintf(fp,"\nmov\t%s,\t[rbx]\n",registers[reg]);
		}
		else
			fprintf(fp,"\nmov\t%s,\trbx\n",registers[reg]);
	}

	return reg;
}

int getTemp(t_item* t)
{
	int arr[]={2,1,4,8};
	int reg;
	if(temp_arr[t->offset]==-1)
	{
		reg=getReg(arr[t->baseType]);
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

void setAddr(v_item* v)
{
	int reg;
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

	return reg;
}

int getReg(int size)
{

}

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

	freeAllReg();

	while(q!=NULL)
	{

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
									regop1=getVar(q->val[0].v_item->lowNode->data->v_item);
								}
								else
									printf("Error Dynamic Array\n");
							}
							else
							{
								regop1=getReg(2);
								fprintf(fp,"\tmov\t%s,\t%d\n",registers[regop1],q->val[0].v_item->low);
							}
							if(q->val[0].v_item->high==-1)
							{
								if(q->val[0].v_item->highNode!=NULL)
								{
									regop2=getVar(q->val[0].v_item->highNode->data->v_item);
								}
								else
									printf("Error Dynamic Array\n");
							}
							else
							{
								regop2=getReg(2);
								fprintf(fp,"\tmov\t%s,\t%d\n",registers[regop2],q->val[0].v_item->high);	
							}
							
							regop3=getReg(3);
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
							setAddr(q->val[0].v_item,1)
							fprintf(fp,"\tmov\tword [RBX],%d\n",q->val[0].v_item->low);
							fprintf(fp,"\tmov\tword [RBX+2],%d\n",q->val[0].v_item->high)	
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
					setAddr(q->val[0].v_item,1);
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
					regop1=getVar(q->val[0].v_item);
				else if(q->tag[0]==3)
					regop1=getTemp(q->val[0].t_item);

				else printf("Relation operator error 1 \n");

				if(q->tag[1]==1)
					regop2=getTok(q->val[1].tk_item);
				else if(q->tag[1]==3)
					regop2=getTemp(q->val[1].t_item);
				else printf("Relation operator error 2 \n");

				if(q->tag[2]==3)
					regop3=getTemp(q->val[2].t_item);
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
					regop1=getTemp(q->val[0].t_item);
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
					regop1=getVar(q->val[1].v_item);
				else if(q->tag[1]==1)
					regop1=getTok(q->val[1].tk_item);
				else if(q->tag[1]==3)
					regop1=getTemp(q->val[1].t_item);
				else
					printf("error unary op 2\n");
				if(q->tag[0]==1)
				{
					if(q->tag[2]==3)
						regop2=getTemp(q->val[2].t_item);
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
					regop2=getTemp(q->val[0].t_item);
				else
					printf("Invalid operand arr_get 1\n");

				if(q->tag[1]==1)
					regop1=getTok(q->val[1].tk_item);
				
				else if(q->tag[1]==0)
					regop1=getVar(q->val[1].v_item);
				
				else
					printf("Invalid operand arr_get 2\n");
				
				fprintf("\tmovsx\tRSI,\t%s\n",registers[regop1]);

				if(q->tag[2]==0)
				{	setAddr(q->val[2].v_item,1);
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
					setAddr(q->val[0].v_item,1);
				else
					printf("Invalid operand arr_get 1\n");
				if(q->tag[1]==1)
					regop1=getTok(q->val[1].tk_item);
				else if(q->tag[1]==3)
					regop1=getTemp(q->val[1].v_item);
				else if(q->tag[1]==0)
					regop1=getVar(q->val[1].v_item);
				else
					printf("Invalid operand arr_get 2\n");

				fprintf("\tmovsx\tRSI,\t%s\n",registers[regop1]);

				if(q->tag[2]==3)
				{
					regop2=getTemp(q->val[2].t_item);
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
					regop1=getVar(q->val[0].v_item);
				else if(q->tag[0]==1)
					regop1=getTok(q->val[0].tk_item);
				else if(q->tag[0]==3)
					regop1=getTemp(q->val[0].t_item);
				else
					printf("Invalid op to assign 1\n");

				if(q->tag[2]==0)
				{
					setAddr(q->val[0].v_item,1);
					fprintf(fp,"\tmov\t[RBX],\t%s\n",registers[regop1]);
				}
				else if(q->tag[2]==3)
				{
					regop2=getTemp(q->val[0].t_item);
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
				fprintf("\tcall\tLabel%d\n",q->val[0].qr_item->srno);
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
					allocMemory(current_funct);//new function
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
					setAddr(q->val[0].v_item,1);
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
					setAddr(q->val[0].v_item,1);
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
					reg=getVar(q->val[0].v_item);
					fprintf(fp,"\tPOP\t%s\n",registers[reg]);
					freeReg(reg)//new function
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
					regop1=getTemp(q->val[0].t_item);
				else
					printf("Invalid Operand for Arith Operator\t");

				else if(q->tag[1]==3)
					regop2=getTemp(q->val[1].t_item);
				else
					printf("Invalid Operand for Arith Operator");	
				
				else if(q->tag[2]==3)
				{
					regop3=getTemp(q->val[2].t_item);//new function
					fprintf(fp,"\tMOV\t%s,\t%s\n",registers[regres,regop1);
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
				if((q->tag[0]!=2 && q->tag[0]!=-1)
				{	
					if(q->tag[0]==3)
						reg=getTemp(q->val[0].t_item);//getTemp	
					else  if(q->tag[0]==0)
						reg=getVar(q->val[0].v_item);//getVar
					else if(q->tag[0]==1)
						reg=getTok(q->val[0].tk_item);//getTok
					
					fprintf(fp,"\tmov\trdi,\tOutput\n");
					fprintf(fp,"\txor\trax,\trax\n");
					fprintf(fp,"\tcall\tprintf\n");	
	
					if(q->tag[0]==0 && q->val[0].v_item->baseType==3)
					{
						fprintf("\tmov\trbx,\t%s\n",registers[reg]);
						regop1=getReg(2);//new fucntion
						regop2=getReg(2);
						fprintf(fp,"\tmov\t%s,\t[RBX]\n",registers[regop1]);
						fprintf(fp,"\tmov\t%s,\t[RBX+2]\n",registers[regop2]);
						fprintf(fp,"\tsub\t%s,\t%s\n",registers[regop2],registers[regop1]);
						fprintf(fp,"inc\t%s\n",registers[regop2]);
						fprintf(fp,"\tadd\trbx,\t4\n");
						fprintf(fp,"\tPLA%d:\n",q->srno);
						if(q->val[0].v_item->eleType==2)
						{
							fprintf(fp,"\tCMP\tbyte [RBX],\t0\n\tJZ PLF%d\n",registers[regop3],qr->srno);
							fprintf(fp,"\tmov\trdi,\tBoolTrue\n\tJMP\tPLT%s\n",qr->srno);
							fprintf(fp,"PLF%d:\n",qr->srno);
							fprintf(fp,"\tmov\trdi,\tBoolFalse\n");
							fprintf(fp,"PLT%d:\n",qr->srno);
							fprintf("\tadd\trbx,\t1\n");				
						}
						else if(q->val[0].v_item->eleType==0)
						{
							fprintf(fp,"\tmov\tesi,\t[RBX]\n",registers[regop1]);
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
						freeReg(regop1);
						freeReg(regop2);						
					}
					else
					{
						if((q->tag[0]==3 && q->val[0].t_item.baseType==2) ||(q->tag[0]==0 && q->val[0].v_item.baseType==2)||(q->tag[0]==1 && (q->val[0].tk_item.index==8 || q->val[0].tk_item.index==9)))
						{
							fprintf(fp,"\tCMP\t%s,\t0\n\tJE PLF%d\n",registers[reg],qr->srno);
							fprintf(fp,"\tmov\trdi,\tBoolTrue\n\tJMP\tPLT%d\n",qr->srno);
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
						fprintf(fp,"\tmov\t%s,\t[RBX+2]\n",registers[regop2]);
						fprintf(fp,"\tsub\t%s,\t%s\n",registers[regop2],registers[regop1]);
						fprintf(fp,"inc\t%s\n",registers[regop2]);
						fprintf(fp,"\tadd\trbx,\t4\n");

						if(q->val[0].v_item.baseType==0)
							fprintf(fp,"\tmov\trdi,\tIntArrInputString\n");
						if(q->val[0].v_item.baseType==2)	
							fprintf(fp,"\tmov\trdi,\tBoolArrInputString\n");
						
						fprintf(fp,"\tmovzx\trsi,\t%s\n",registers[regop2]);

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
	fprintf(fp,"IntOutput:\n\tdb \"%hd\",10,0\n");
	fprintf(fp,"IntInput:\n\tdb \"%hd\",0,\n");
	fprintf(fp,"IntInputString:\n\tdb \"Please Enter an Integer\",10,0\n");
	fprintf(fp,"BoolInputString:\n\tdb \"Please Enter 0 for false and 1 for true\",10,0\n");
	fprintf(fp,"IntArrInputString:\n\tdb \"Please Enter an %d Integers\",10,0 \n");
	fprintf(fp,"BoolInputString:\n\tdb \"Please Enter %d 0s or 1s for false and true\",10,0");
	fprintf(fp,"section .bss\n");
	fprintf(fp,"intvar: resw 1\n");
	fclose(fp);
}

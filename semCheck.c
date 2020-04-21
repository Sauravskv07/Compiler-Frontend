/*
GROUP NO. = 46
2017A7PS0006P PIKLU PAUL
2017A7PS0007P RAJABABU SAIKIA
2017A7PS0090P SAURAV VIRMANI
2017A7PS0111P SIDDHANT KHARBANDA
2017A7PS0275P SREYAS RAVICHANDRAN
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"
#include "hash_table.h"
#include "parseRules.h"
#include "utils.h"
#include "adt.h"
#include "ast.h"
#include "symbol_table.h"
#include "semCheck.h"

void printSize(symnode* root)
{
	if(root==NULL)
	{
		printf("sym root error\n");
	}
	else
	{
		symnode* current_func=root->child;

		while(current_func != NULL)
		{
			if(current_func->module_name==NULL)
				printf("DRIVER:\t");
			else
			{
				printf("%s\t",current_func->module_name);
			}
				printf("%d\n",current_func->current_offset);
				current_func=current_func->right;
		}
	}
}

void setOutput(astnode *t, symnode *current)
{
	symnode* temp2=current;
	ht_item *temp;

	if(current==NULL)
		printf("Symbol Table Error\n");

	do
	{
		if(temp2->isModuleScope==1)
		{
			//printf("module name = %s\n",current->module_name);
			temp=ht_search(ht_search(sym_root->symbol_table,current->module_name)->data->f_item->pr->output_list, t->data->token->lexeme);
			//printf("module name = %s\n",current->module_name);
			if(temp!= NULL && temp->data->v_item->low!=1)
			{
				temp->data->v_item->low=1;
				current->num_output_defined++;
			}			
		}

		temp=ht_search(temp2->symbol_table, t->data->token->lexeme);
	
		temp2=temp2->parent;
	
	}while(temp == NULL && temp2!=sym_root);

}



int isRedefined(astnode *t,symnode *current)
{
	symnode* temp2=current;
	ht_item *temp;

	if(current==NULL)
		printf("Symbol Table Error\n");

	do
	{
		temp=ht_search(temp2->symbol_table, t->data->token->lexeme);
		
		if(temp!=NULL)
			break;
		else if(temp2->isForScope==1 && strcmp(t->data->token->lexeme,temp2->for_id)==0)
			return 1;

		temp2=temp2->parent;
	
	}while(temp == NULL && temp2!=sym_root);

	return 0;

}

int incrementOffset(int current_offset,VarType baseType,VarType eleType,int low,int high)
{
	int arr[]={2,4,1};

	if(baseType==0)//integer
		current_offset+=2;
	else if(baseType==1)//real
		current_offset+=4;
	else if(baseType==2)//boolean
		current_offset+=1;
	else if(baseType==3 && (low==-1 ||high==-1))
		current_offset+=8;
	else if(baseType==3)
		current_offset+=4+(high-low+1)*arr[eleType];
	return current_offset;
}

int incrementOffset2(int current_offset,VarType type)
{
	if(type==0)//integer
		current_offset+=2;
	else if(type==1)//real
		current_offset+=4;
	else if(type==2)//boolean
		current_offset+=1;
	else if(type==3)
		current_offset+=8;
	return current_offset;
}

ht_item* getID(astnode *t,symnode *current)
{
	//printf("inside get id\n");
	symnode* temp2=current;
	ht_item *temp;	
	if(current==NULL)
		printf("Symbol Table Error\n");
	do
	{
		temp=ht_search(temp2->symbol_table, t->data->token->lexeme);
		temp2=temp2->parent;
	
	}while(temp == NULL && temp2!=sym_root);

	if(temp==NULL && current->module_name!=NULL)
		temp= ht_search(ht_search(sym_root->symbol_table,current->module_name)->data->f_item->pr->input_list, t->data->token->lexeme);

	if(temp==NULL && current->module_name!=NULL)
		temp= ht_search(ht_search(sym_root->symbol_table,current->module_name)->data->f_item->pr->output_list, t->data->token->lexeme);
	
	if(temp==NULL)
	{	
		printf("Undeclared Variable Found at LINE NUMBER =  %d  LEXEME = %s\n",t->data->token->LN, t->data->token->lexeme);
		sem_num_errors++;
		t->attr->baseType=4;
	}
	else{
		t->attr->baseType = temp->data->v_item->baseType;
		t->attr->eleType = temp->data->v_item->eleType;
		t->attr->low = temp->data->v_item->low;
		t->attr->high = temp->data->v_item->high;
	}

	return temp;
	//printf("outside get it\n");
}

void checkOutputList(astnode *t,symnode *current,parameters *pr)
{
	astnode *st=t;
	ht_item* temp = NULL;
	int cnt=0;

	while(t!=NULL)
	{
		if(cnt>=pr->num_outputs)
		{
			printf("Number Mismatch with Output Parameters Found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
			sem_num_errors++;
			break;
		}
		getID(t->child,current);
		if(isRedefined(t->child,current)==1)
		{
			printf("Redefinition of Loop Variable at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
			sem_num_errors++;
		}
		
		setOutput(t->child,current);

		if(t->child->attr->baseType==4)
		{
			break;
		}
		if(t->child->attr->baseType!=pr->output_types[cnt])
		{
			printf("Type Mismatch with Output Parameters Found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
			sem_num_errors++;
			//break;
		}
		cnt++;
		t=t->child->right;		
	}
	if(cnt<pr->num_outputs)
	{
		printf("Number Mismatch with Output Parameters Found at LINE NUMBER =  %d  LEXEME = %s\n",st->child->data->token->LN, st->child->data->token->lexeme);
		sem_num_errors++;
	}		
}

void checkInputList(astnode *t,symnode *current,parameters *pr)
{
	astnode *st=t;
	ht_item* temp = NULL;
	int cnt=0;

	while(t!=NULL)
	{
		if(cnt>=pr->num_inputs)
		{
			printf("Number Mismatch with Input Parameters Found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
			sem_num_errors++;
			break;
		}
		getID(t->child,current);
		if(t->child->attr->baseType==4)
		{
			break;
		}
		if(t->child->attr->baseType!=pr->input_types[cnt].baseType)
		{
			printf("Type Mismatch with Input Parameters Found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
			sem_num_errors++;
		}

		if(t->child->attr->baseType==3)
		{
			if(t->child->attr->eleType!=pr->input_types[cnt].eleType)
			{
				printf("Type Mismatch with Input Parameters Found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
				sem_num_errors++;
			}

			if(t->child->attr->low !=-1 && pr->input_types[cnt].low !=-1 && t->child->attr->low!=pr->input_types[cnt].low)
			{
				printf("Bound Mismatch with Input Parameters Found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
				sem_num_errors++;
			}

			if(t->child->attr->high !=-1 && pr->input_types[cnt].high !=-1 && t->child->attr->high!=pr->input_types[cnt].high)
			{
				printf("Bound Mismatch with Input Parameters Found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
				sem_num_errors++;
			}

		}
		cnt++;
		t=t->child->right;		
	}
	if(cnt<pr->num_inputs)
	{
		printf("Number Mismatch with Input Parameters Found at LINE NUMBER =  %d  LEXEME = %s\n",st->child->data->token->LN, st->child->data->token->lexeme);
		sem_num_errors++;
	}		
}


VarType typeCheck(astnode *op,astnode *first,astnode *second)
{

	VarType f_type=first->attr->baseType;
	VarType s_type=second->attr->baseType;
	VarType r_type=4;

	if(f_type==4 || s_type==4)
	{
		//printf("Operand Type is Undefined at LINE NUMBER =  %d  LEXEME = %s\n",op->data->token->LN, op->data->token->lexeme);		
		return r_type;
	}

	if(f_type==3 || s_type==3)
	{
		printf("Operand cannot be of Array Type at LINE NUMBER =  %d  LEXEME = %s\n",op->data->token->LN, op->data->token->lexeme);
		sem_num_errors++;	
		return r_type;	
	}

	switch(op->data->token->index)
	{
		case 10:
		case 11:
		case 12:
		case 13://arithOperators
		{
			if(f_type==s_type && f_type<=1)
			{
				r_type=f_type;
			}
			else
			{
				printf("Operand Types Mismatch found at LINE NUMBER =  %d  LEXEME = %s type1=%d type2=%d\n",op->data->token->LN, op->data->token->lexeme,f_type,s_type);
				sem_num_errors++;
			}
			break;
		}
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19://relationalOperators
		{
			if(f_type==s_type && f_type<=1)
			{
				r_type=2;
			}
			else
			{
				printf("Operand Mismatch found at LINE NUMBER =  %d  LEXEME = %s\n",op->data->token->LN, op->data->token->lexeme);
				sem_num_errors++;			
			}
			break;		
		}
		case 20:
		case 21://BoolOperators
		{
			if(f_type==s_type && f_type==2)
			{
				r_type=2;
			}
			else
			{
				printf("Operand Mismatch found at LINE NUMBER =  %d  LEXEME = %s\n",op->data->token->LN, op->data->token->lexeme);
				sem_num_errors++;
			}
			break;	
		}
	}

	return r_type;
}

void checkSemRules(astnode *t,symnode* current)
{

	ht_item* temp = NULL;
	symnode* new_node = NULL;

	if(t!=NULL)
	{
		if(t->tag==1)
		{
			//printf("here at this %s with index no. = %d\n",t->data->token->lexeme,t->data->token->index);
			switch(t->data->token->index)
			{
				case 5://INTEGER
				{
					t->attr->baseType=0;
					t->attr->eleType=4;
					break;
				}

				case 6://REAL
				{
					t->attr->baseType=1;
					t->attr->eleType=4;
					break;
				}

				case 7://BOOLEAN
				{
					t->attr->baseType=2;
					t->attr->eleType=4;
					break;
				}
				case 3://RNUM
				{
					t->attr->baseType=1;
					t->attr->eleType=4;
					break;
				}
				case 2://NUM	
				{
					t->attr->baseType=0;
					t->attr->eleType=4;
					break;
				}
				case 4://ID
				{
					getID(t,current);
					break;
				}
				case 8://TRUE
				case 9://FALSE
				{
					t->attr->baseType=2;
					t->attr->eleType=4;
					break;
				}
			}
		}
		else
		{
			//moduleDef
			switch(t->data->nonterm->data->t_item->index)
			{
				case 64://moduleDeclaration:
				{
					//printf("moduleDeclaration\n");
					if(ht_search(current->symbol_table, t->child->data->token->lexeme)->data->f_item->isDef!=1)
						ht_search(current->symbol_table, t->child->data->token->lexeme)->data->f_item->isDef=-1;
					break;
				}
	
				case 75://module
				{	
					//printf("module");
					temp = ht_search(current->symbol_table, t->child->data->token->lexeme);

					if(temp->data->f_item->isDef==-1)
					{
						printf("Function both declared and defined before use at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
						sem_num_errors++;
					}

					if(temp->data->f_item->isDef==-2)
					{	
						break;
					}
					else
						temp->data->f_item->isDef=-2;

					new_node=create_new_symnode();
					current= insert_as_symchild(current,new_node);
					new_node->module_name=(char *)malloc(sizeof(char)*21);
					strcpy(current->module_name, t->child->data->token->lexeme);
					current->isModuleScope=1;

					astnode* rt=t->child->right;

					while(rt!=NULL)
					{
						checkSemRules(rt,current);
						rt=rt->right;
					}
					
					if(current->num_output_defined!=ht_search(sym_root->symbol_table,current->module_name)->data->f_item->pr->num_outputs)
					{
						printf("Certain Output Variables are Undefined for Function at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
						sem_num_errors++;
					}
				

					current=current->parent;
					break;					
				}

				case 77://input_plist
				case 76://ret
					break;

				case 74://driverModule
				{
					//printf("driverModule\n");
					current= insert_as_symchild(current,create_new_symnode());
					
					if(t->child!=NULL)
						checkSemRules(t->child,current);
					
					current=current->parent;
					
					break;
				}


	
				case 86://assignmentStmt:
				{
					//printf("assignmentStmt\n");

					checkSemRules(t->child,current);	
					checkSemRules(t->child->right,current);
					
					if(isRedefined(t->child,current)==1)
					{
						printf("Redefinition of Loop Variable at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
						sem_num_errors++;
					}
					else if(t->child->right->data->nonterm->data->t_item->index==ht_search(mapping_table,"lvalueIDStmt")->data->t_item->index)
					{
						//printf("here\n");
						setOutput(t->child,current);
						//printf("here too\n");
						if(t->child->attr->baseType!=t->child->right->attr->baseType)
						{
							printf("Type Mismatch Found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
							sem_num_errors++;
						}
						else if(t->child->attr->baseType==3)
						{
							if((t->child->attr->eleType!=t->child->right->attr->eleType) ||(t->child->attr->low!=-1 && t->child->right->attr->low!=-1 && t->child->attr->low!=t->child->right->attr->low) || (t->child->attr->high!=-1 && t->child->right->attr->high!=-1 && t->child->attr->high!=t->child->right->attr->high))
							{
								printf("Array Type Mismatch Found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
								sem_num_errors++;
							}

						} 
							
						
					}
					else
					{
						if(t->child->attr->baseType!=3)
						{
							printf("Variable not of Array Type at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
							sem_num_errors++;
						}
						else
						{
							if(t->child->right->child->data->token->index==2)
							{
								if(t->child->attr->low!=-1 && t->child->attr->low > t->child->right->child->data->token->val.i_val)
								{
									printf("Index smaller than lower Bound at LINE NUMBER =  %d  LEXEME = %s\n",t->child->right->child->data->token->LN, t->child->right->child->data->token->lexeme);
									sem_num_errors++;
								}

								if(t->child->attr->high!=-1 && t->child->attr->high < t->child->right->child->data->token->val.i_val)
								{
									printf("Index greater than upper Bound at LINE NUMBER =  %d  LEXEME = %s\n",t->child->right->child->data->token->LN, t->child->right->child->data->token->lexeme);
									sem_num_errors++;
								}
							}

					
							if(t->child->attr->eleType != t->child->right->attr->baseType)
							{
								printf("Type Mismatch Found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
								sem_num_errors++;
							}
						}		
					}
					break;
				}
 			
				case 72://var_id_num:
				{
					//printf("var_id_num\n");

					checkSemRules(t->child,current);

					if(t->child->attr->baseType!=4)
					{
						if(t->child->right!=NULL && t->child->attr->baseType==3)
						{
							if(t->child->right->child->data->token->index==2)
							{
								if(t->child->attr->low!=-1 && t->child->attr->low > t->child->right->child->data->token->val.i_val)
								{
									printf("Index smaller than lower Bound at LINE NUMBER =  %d  LEXEME = %s\n",t->child->right->child->data->token->LN, t->child->right->child->data->token->lexeme);
									sem_num_errors++;
								}

								if(t->child->attr->high!=-1 && t->child->attr->high < t->child->right->child->data->token->val.i_val)
								{
									printf("Index greater than upper Bound at LINE NUMBER =  %d  LEXEME = %s\n",t->child->right->child->data->token->LN, t->child->right->child->data->token->lexeme);
									sem_num_errors++;
								}
							}
							else
								checkSemRules(t->child->right,current);

							t->attr->baseType=t->child->attr->eleType;
						}
						else if(t->child->right!=NULL && t->child->attr->baseType!=3)
						{
							printf("Variable not of Array Type at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
							sem_num_errors++;

							t->attr->baseType=4;
						}
						else
						{
							t->attr->baseType=t->child->attr->baseType;
							t->attr->eleType=t->child->attr->eleType;
							t->attr->low=t->child->attr->low;
							t->attr->high=t->child->attr->high;
						}
					}
					else
					{
						t->attr->baseType=4;					
					}

					//printf("LINE NUMBER =  %d  LEXEME = %s TYPE= %d\n",t->child->data->token->LN, t->child->data->token->lexeme,t->attr->baseType);
					break;
				}

				case 93://lvalueARRStmt:
				{
					//printf("lvalueARRStmt\n");
					checkSemRules(t->child,current);
					if(t->child->attr->baseType!=0)
					{
						printf("Array of Index should be Integer at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
						sem_num_errors++;		
					}
					checkSemRules(t->child->right,current);
					t->attr->baseType=t->child->right->attr->baseType;
					break;
				}

				case 84://whichId:
				{
					//printf("whichId\n");
					checkSemRules(t->child,current);
					if(t->child->attr->baseType!=0)
					{
						printf("Array of Index should be Integer at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
						sem_num_errors++;
					}
					break;
				}

				case 92://lvalueIDStmt:
				case 97://newNT:
				case 107://factor:
				{
					//printf("factor | lvalueIDStmt | newNT \n");
					checkSemRules(t->child,current);
					
					t->attr->baseType=t->child->attr->baseType;
					t->attr->eleType=t->child->attr->eleType;
					t->attr->low=t->child->attr->low;
					t->attr->high=t->child->attr->high;
					break;
				}

				case 96://U:
				{
					//printf("U\n");
					checkSemRules(t->child->right,current);
					if(t->child->right->attr->baseType<=1)
						t->attr->baseType=t->child->right->attr->baseType;
					else
					{
						printf("Operand To Unary must be INT or REAL at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
						sem_num_errors++;
						t->attr->baseType=4;
					}
					break;
				}

				case 100://AnyTerm:
				case 99://arithmeticOrBooleanExpr:
				case 103://arithmeticExpr:
				case 105://term:
				{
					//printf("term | arithmeticOrBooleanExpr | arithmeticExpr | AnyTerm\n");
					checkSemRules(t->child,current);
					if(t->child->right==NULL)
					{
						t->attr->baseType=t->child->attr->baseType;
						t->attr->eleType=t->child->attr->eleType;
						t->attr->low=t->child->attr->low;
						t->attr->high=t->child->attr->high;
					}
					else
					{
						checkSemRules(t->child->right,current);
						t->attr->baseType=typeCheck(t->child->right->child,t->child,t->child->right);
					}
					break;
				}

				case 102://N8:
				{
					//printf("N8\n");
					checkSemRules(t->child->right,current);
					t->attr->baseType=t->child->right->attr->baseType;
					break;
				}

				case 101://N7:
				case 104://N4:
				case 106://N5:
				{
					//printf("N4 | 5| 7\n");
					checkSemRules(t->child->right,current);
					if(t->child->right->right!=NULL)
					{
						checkSemRules(t->child->right->right,current);
						t->attr->baseType=typeCheck(t->child,t->child->right,t->child->right->right);
					}
					else
						t->attr->baseType=t->child->right->attr->baseType;
		
					break;
				}

				case 89://moduleReuseStmt:
				{
					//printf("moduleReuseStmt\n");

					if(t->child->tag==1)
					{
						temp=ht_search(sym_root->symbol_table, t->child->data->token->lexeme);
						if(temp==NULL || temp->data->f_item->isDef>=0)
						{
							printf("Unknown Function at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
							sem_num_errors++;
							break;
						}
						if(temp->data->f_item->isDef==-1)
							temp->data->f_item->isDef=-3;

						parameters *pr= temp->data->f_item->pr;
						if(current->module_name!=NULL && strcmp(t->child->data->token->lexeme,current->module_name)==0)
						{
							printf("Recursive Call at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
							sem_num_errors++;
							break;	
						}
						
						checkInputList(t->child->right,current,pr);
					}
					else
					{
						temp=ht_search(sym_root->symbol_table, t->child->right->data->token->lexeme);
						if(temp==NULL || temp->data->f_item->isDef>=0)
						{
							printf("Unknown Function at LINE NUMBER =  %d  LEXEME = %s\n",t->child->right->data->token->LN, t->child->right->data->token->lexeme);
							sem_num_errors++;
							break;
						}

						if(temp->data->f_item->isDef==-1)
							temp->data->f_item->isDef=-3;

						parameters *pr= temp->data->f_item->pr;
						if(current->module_name!=NULL && strcmp(t->child->right->data->token->lexeme,current->module_name)==0)
						{
							printf("Recursive Call at LINE NUMBER =  %d  LEXEME = %s\n",t->child->right->data->token->LN, t->child->right->data->token->lexeme);
							sem_num_errors++;
							break;	
						}
						checkOutputList(t->child->child,current,pr);
						checkInputList(t->child->right->right,current,pr);
					}
					break;

				}
				
				case 118://iterStmt
				{
					//printf("iterStmt\n");

					checkSemRules(t->child,current);
					new_node=create_new_symnode();
					if(current->module_name!=NULL)
						new_node->module_name=(char *)malloc(sizeof(char)*21);
					new_node->current_offset=current->current_offset;
					current= insert_as_symchild(current,new_node);

					if(t->child->tag==1)
					{
						current->isForScope=1;
						strcpy(current->for_id,t->child->data->token->lexeme);
						checkSemRules(t->child->right,current);
						checkSemRules(t->child->right->right,current);
					}
					else
					{
						checkSemRules(t->child->right,current);
					}
					
					current->parent->current_offset=current->current_offset;
					current=current->parent;

					break;
				}

				case 119://range
				{
					//printf("range\n");

					checkSemRules(t->child,current);
					checkSemRules(t->child->right,current);
					if(t->child->data->token->val.i_val>t->child->right->data->token->val.i_val)
					{
						printf("Error in Ranges Found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
						sem_num_errors++;
					}
					break;
				}

				case 116://conditionalStmt	
				{
					//printf("conditionalStmt\n");

					checkSemRules(t->child,current);

					new_node=create_new_symnode();
					if(current->module_name!=NULL)
						new_node->module_name=(char *)malloc(sizeof(char)*21);
					new_node->current_offset=current->current_offset;
					current= insert_as_symchild(current,new_node);

					if(t->child->attr->baseType==0)
						current->switchStatus=0;
					else if(t->child->attr->baseType==2)
						current->switchStatus=2;
					else
					{
						printf("Incompatible Variable for Switch Found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
						sem_num_errors++;
						break;
					}
					
					checkSemRules(t->child->right,current);//check case Stmts
					
					if(t->child->right->right==NULL && current->switchStatus==0)
					{
						printf("INT Switch without Default at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
						sem_num_errors++;
					}
					else if(t->child->right->right!=NULL && current->switchStatus==2)
					{
						printf("BOOL Switch with Default at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
						sem_num_errors++;
					}
					
					checkSemRules(t->child->right->right,current);

					current->parent->current_offset=current->current_offset;
					current=current->parent;

					return;						
				}
			
				case 113://N9
				case 112://caseStmts
				{
					//printf("caseStmts | N9\n");

					checkSemRules(t->child,current);
					if(t->child->attr->baseType!=current->switchStatus)
					{
						printf("Invalid Case Value at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
						sem_num_errors++;
					}
					checkSemRules(t->child->right,current);//can be statements or N9

					if(t->child->right!=NULL)//means there exists statement or N9
						checkSemRules(t->child->right->right,current);
					
					break;
					
				}

				case 80://range_arrays
				{
					if(t->child->data->token->index == 2)
						t->attr->low = t->child->data->token->val.i_val;
					else
					{
						checkSemRules(t->child,current);
						if(t->child->attr->baseType!=0)
						{
							printf("Non Integer Range Variable found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
							sem_num_errors++;
						}
						else
							t->attr->lowNode=getID(t->child,current);
						t->attr->low=-1;
					}

					if(t->child->right->data->token->index ==2)
						t->attr->high = t->child->right->data->token->val.i_val;
					else
					{
						checkSemRules(t->child->right,current);
						if(t->child->right->attr->baseType!=0)
						{
							printf("Non Integer Range Variable found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->right->data->token->LN, t->child->right->data->token->lexeme);
							sem_num_errors++;
						}
						else
							t->attr->highNode=getID(t->child->right,current);
						t->attr->high=-1;
					}

					if(t->child->data->token->index == 2 && (t->child->right->data->token->index == 2) && (t->child->right->data->token->val.i_val < t->child->data->token->val.i_val))
					{
						t->attr->high=-1;
						t->attr->low=-1;
						printf("Low > High Array Bound found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
						sem_num_errors++;
					}
					break;
				}

				case 79://dataType
				{
					//printf("dataType\n");
					checkSemRules(t->child,current);
					checkSemRules(t->child->right,current);
					t->attr->baseType=3;
					t->attr->eleType=t->child->right->attr->baseType;
					t->attr->low=t->child->attr->low;
					t->attr->high=t->child->attr->high;
					t->attr->lowNode=t->child->attr->lowNode;
					t->attr->highNode=t->child->attr->highNode;
					break;
				}
				
				case 115://DeclareStmt:
				{
					//printf("DeclareStmt\n");
					checkSemRules(t->child->right,current);
					t->child->attr->baseType=t->child->right->attr->baseType;
					t->child->attr->eleType=t->child->right->attr->eleType;
					t->child->attr->low=t->child->right->attr->low;
					t->child->attr->high=t->child->right->attr->high;
					t->child->attr->lowNode=t->child->right->attr->lowNode;
					t->child->attr->highNode=t->child->right->attr->highNode;
					checkSemRules(t->child,current);
					break;
				}

				case 94://N3:
				case 91://idList:
				{
					//printf("idList\n");

					if(ht_search(current->symbol_table,t->child->data->token->lexeme)==NULL && !(current->isModuleScope==1 && ht_search(ht_search(sym_root->symbol_table,current->module_name)->data->f_item->pr->output_list, t->child->data->token->lexeme)!=NULL))
					{
						current->current_offset=incrementOffset(current->current_offset,t->attr->baseType,t->attr->eleType,t->attr->low,t->attr->high);	
						temp=ht_insert_var_item(current->symbol_table,t->child->data->token->lexeme ,-current->current_offset, t->attr->baseType, t->attr->eleType, t->attr->low, t->attr->high);
						temp->data->v_item->lowNode=t->attr->lowNode;
						temp->data->v_item->highNode=t->attr->highNode;
						
					}
					else
					{
						printf("Redeclaraton of Variable found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
						sem_num_errors++;
					}
					
					if(t->child->right!=NULL)
					{
						t->child->right->attr->baseType=t->attr->baseType;
						t->child->right->attr->eleType=t->attr->eleType;
						t->child->right->attr->high=t->attr->high;
						t->child->right->attr->low=t->attr->low;
						t->child->right->attr->highNode=t->attr->highNode;
						t->child->right->attr->lowNode=t->attr->lowNode;						
						checkSemRules(t->child->right,current);
					}
					
					break;
				}
				
				case 117://default
				{
					checkSemRules(t->child,current);

					break;
				}

				case 83://ioStmt
				{
					if(t->child->tag==1 && t->child->data->token->index==4)
					{
						if(isRedefined(t->child,current)==1)
						{
							printf("Redefinition of Loop Variable at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
							sem_num_errors++;
						}
						setOutput(t->child,current);
					}
				}
				
				default:
				{
					checkSemRules(t->child,current);

					astnode* rt=t->child->right;

					while(rt!=NULL)
					{
						checkSemRules(rt,current);
						rt=rt->right;
					}
				}
			}
		}
	}
}


int fillTheParams(astnode* t,parameters *pr,int current_offset)
{
	ht_item* temp = NULL;

	if(t!=NULL)
	{
		if(t->tag==1)
		{
			//printf("here at this %s with index no. = %d\n",t->data->token->lexeme,t->data->token->index);
			switch(t->data->token->index)
			{
				case 5://INTEGER
				{
					t->attr->baseType=0;
					t->attr->eleType=4;
					break;
				}

				case 6://REAL
				{
					t->attr->baseType=1;
					t->attr->eleType=4;
					break;
				}

				case 7://BOOLEAN
				{
					t->attr->baseType=2;
					t->attr->eleType=4;
					break;
				}
				case 3://RNUM
				{
					t->attr->baseType=1;
					t->attr->eleType=4;
					break;
				}
				case 2://NUM	
				{
					t->attr->baseType=0;
					t->attr->eleType=4;
					break;
				}
				case 4://ID
					break;
				case 8://TRUE
				case 9://FALSE
				{
					t->attr->baseType=2;
					t->attr->eleType=4;
					break;
				}
			}
		}	
		else
		{
			switch(t->data->nonterm->data->t_item->index)
			{
				case 80://range_arrays
				{
					if(t->child->data->token->index == 2)
						t->attr->low = t->child->data->token->val.i_val;
					else
						t->attr->low = -1;

					if(t->child->right->data->token->index ==2)
						t->attr->high = t->child->right->data->token->val.i_val;
					else
						t->attr->high = -1;

					if(t->child->data->token->index == 2 && t->child->right->data->token->index == 2 && t->child->right->data->token->val.i_val< t->child->data->token->val.i_val)
					{
						t->attr->high=-1;
						t->attr->low=-1;
						printf("Low > High Array Bound found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
						sem_num_errors++;
					}
					break;
				
				}
				case 79://dataType
				{
					fillTheParams(t->child,pr,current_offset);
					fillTheParams(t->child->right,pr,current_offset);
					t->attr->baseType=3;
					t->attr->eleType=t->child->right->attr->baseType;
					t->attr->low=t->child->attr->low;
					t->attr->high=t->child->attr->high;
					break;
				}

				case 67://N1
				case 77://input_plist
				{
					//printf("input_plist | N1\n");
					//printf("current_offset= %d\n",current_offset);
					fillTheParams(t->child->right,pr,current_offset);
					if(ht_search(pr->input_list,t->child->data->token->lexeme)==NULL)
					{
						ht_insert_var_item(pr->input_list,t->child->data->token->lexeme ,current_offset, t->child->right->attr->baseType, t->child->right->attr->eleType,t->child->right->attr->low,t->child->right->attr->high);
						current_offset=incrementOffset2(current_offset,t->child->right->attr->baseType);
						pr->input_types[pr->num_inputs].eleType=t->child->right->attr->eleType;
						pr->input_types[pr->num_inputs].baseType=t->child->right->attr->baseType;
						pr->input_types[pr->num_inputs].low=t->child->right->attr->low;
						pr->input_types[pr->num_inputs].high=t->child->right->attr->high;
						pr->num_inputs++;
					}
					else
					{
						printf("Input Variable Collision found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
						sem_num_errors++;
					}
						
					current_offset=fillTheParams(t->child->right->right,pr,current_offset);

					break;
				}
				case 76://ret
				{
					current_offset=fillTheParams(t->child,pr,current_offset);
					break;
				}
				case 68://N2
				case 78://output_plist
				{
					fillTheParams(t->child->right,pr,current_offset);
					if(ht_search(pr->output_list,t->child->data->token->lexeme)==NULL)
					{
						ht_insert_var_item(pr->output_list,t->child->data->token->lexeme ,current_offset, t->child->right->attr->baseType, t->child->right->attr->eleType,-1,-1);
						current_offset=incrementOffset2(current_offset,t->child->right->attr->baseType);
						pr->output_types[pr->num_outputs]=t->child->right->attr->baseType;
						pr->num_outputs++;
					}
					else
					{
						//printf("Output Variable Collision found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
						sem_num_errors++;
					}
					
					current_offset=fillTheParams(t->child->right->right,pr,current_offset);

					break;
				}				
				default:
					break;
			
			}
		}
	}
	return current_offset;
}


symnode* modulesParHamla(astnode *t,symnode* root)
{
	ht_item* temp = NULL;

	if(t!=NULL)
	{
		if(t->tag==1)
		{

		}
		else
		{
			switch(t->data->nonterm->data->t_item->index)
			{
				case 63://program
				{
						root= insert_as_symchild(root,create_new_symnode());
				}
				case 65://moduleDeclarations
				case 66://otherModules
				{
					//printf("program | moduleDeclarations | otherModules\n");
					modulesParHamla(t->child,root);

					astnode*rt= t->child->right;

					while(rt!=NULL)
					{
						modulesParHamla(rt,root);
						rt=rt->right;
					}
					break;
				}
				case 75://module
				{
					//printf("MODULE %s\n",t->child->data->token->lexeme);
					temp = ht_search(root->symbol_table, t->child->data->token->lexeme);

					if(temp==NULL)
						temp=ht_insert_func_item(root->symbol_table, t->child->data->token->lexeme, 2);

					else if(temp->data->f_item->isDef==2)
					{	printf("Redefinition of function found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
						sem_num_errors++;
						break;
					}
					else
						temp->data->f_item->isDef=2;

					temp->data->f_item->pr=(parameters*)malloc(sizeof(parameters));
					temp->data->f_item->pr->num_inputs=0;
					temp->data->f_item->pr->num_outputs=0;
					temp->data->f_item->pr->input_list=ht_new();
					temp->data->f_item->pr->output_list=ht_new();

					astnode* rt=t->child->right;

					int current_offset=0;

					while(rt!=NULL)
					{
						//printf("current offset = %d\n",current_offset);
						current_offset=fillTheParams(rt,temp->data->f_item->pr,current_offset);
						rt=rt->right;
					}
					break;				
				}

				case 64://moduleDeclaration:
				{
					//printf("moduleDeclaration %s\n",t->child->data->token->lexeme);
					if(ht_search(root->symbol_table, t->child->data->token->lexeme)==NULL)
						ht_insert_func_item(root->symbol_table, t->child->data->token->lexeme, 1);
					else
					{
						printf("Redeclaraton of function found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
						sem_num_errors++;
					}
					break;				
				}

				default: break;
			
			}
		}
	}
	return root;
}

void semCheck(astnode* t)
{
	sem_num_errors=0;

	symnode* root=NULL;
	printf("Generating Functions Parameters\n");
	root=modulesParHamla(t,NULL);
	printf("Function Parameter Generation Done\n");
	checkSemRules(t,root);

}



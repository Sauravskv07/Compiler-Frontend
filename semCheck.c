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

void getID(astnode *t,symnode *current)
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
	
	}while(temp == NULL && temp2!=NULL);

	if(temp==NULL)
	{	
		printf("Undeclared Variable Found at LINE NUMBER =  %d  LEXEME = %s\n",t->data->token->LN, t->data->token->lexeme);
		t->attr->baseType=4;
	}
	else{
		t->attr->baseType = temp->data->v_item->baseType;
		t->attr->eleType = temp->data->v_item->eleType;
	}

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
			break;
		}
		getID(t->child,current);
		if(t->child->attr->baseType==4)
		{
			break;
		}
		if(t->child->attr->baseType!=pr->output_types[cnt])
		{
			printf("Type Mismatch with Output Parameters Found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
			break;
		}
		cnt++;
		t=t->child->right;		
	}
	if(cnt<pr->num_outputs)
		printf("Number Mismatch with Output Parameters Found at LINE NUMBER =  %d  LEXEME = %s\n",st->child->data->token->LN, st->child->data->token->lexeme);		
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
			break;
		}
		getID(t->child,current);
		if(t->child->attr->baseType==4)
		{
			break;
		}
		if(t->child->attr->baseType!=pr->input_types[cnt].baseType ||(t->child->attr->baseType==3 && t->child->attr->eleType!=pr->input_types[cnt].eleType))
		{
			printf("Type Mismatch with Input Parameters Found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
			break;
		}
		cnt++;
		t=t->child->right;		
	}
	if(cnt<pr->num_inputs)
		printf("Number Mismatch with Input Parameters Found at LINE NUMBER =  %d  LEXEME = %s\n",st->child->data->token->LN, st->child->data->token->lexeme);		
}


VarType typeCheck(astnode *op,astnode *first,astnode *second)
{

	VarType f_type=first->attr->baseType;
	VarType s_type=second->attr->baseType;
	VarType r_type=4;

	if(f_type==4 || s_type==4)
	{
		printf("Operand Type is Undefined at LINE NUMBER =  %d  LEXEME = %s\n",op->data->token->LN, op->data->token->lexeme);		
		return r_type;
	}

	if(f_type==3 || s_type==3)
	{
		printf("Operand cannot be of Array Type at LINE NUMBER =  %d  LEXEME = %s\n",op->data->token->LN, op->data->token->lexeme);		
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
				printf("Operand Mismatch found at LINE NUMBER =  %d  LEXEME = %s\n",op->data->token->LN, op->data->token->lexeme);
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
			}
			break;	
		}
	}

	return r_type;
}

void checkSemRules(astnode *t,symnode* current)
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
				case 65://moduleDeclaration:
				{
					printf("moduleDeclaration\n");
					ht_search(current->symbol_table, t->child->data->token->lexeme)->data->f_item->isDef=-1;
					break;
				}
				case 68://module
				{	
					printf("");
					temp = ht_search(current->symbol_table, t->child->data->token->lexeme);

					if(temp->data->f_item->isDef<=-2)
					{	
						break;
					}
					else if(temp->data->f_item->isDef==-1)
						temp->data->f_item->isDef=-3;
					else
						temp->data->f_item->isDef=-2;

					current= insert_as_symchild(current,create_new_symnode());
					strcpy(current->module_name, t->child->data->token->lexeme);
					current->isModuleScope=1;

					astnode* rt=t->child->right;

					while(rt!=NULL)
					{
						checkSemRules(rt,current);
						rt=rt->right;
					}
					
					current=current->parent;
					break;					
				}

				case 70://input_plist
				case 69://ret
					break;

				case 67://driverModule
				{
					printf("driverModule\n");
					current= insert_as_symchild(current,create_new_symnode());
					
					if(t->child!=NULL)
						checkSemRules(t->child,current);
					
					current=current->parent;
					
					break;
				}

				case 74://dataType
				{
					printf("dataType\n");
					checkSemRules(t->child->right,current);
					t->attr->baseType=3;
					t->attr->eleType=t->child->right->attr->baseType;
					break;
				}
				
				case 112://DeclareStmt:
				{
					printf("DeclareStmt\n");
					checkSemRules(t->child->right,current);
					t->child->attr->baseType=t->child->right->attr->baseType;
					t->child->attr->eleType=t->child->right->attr->eleType;
					checkSemRules(t->child,current);
					break;
				}

				case 93://idList:
				{
					printf("idList\n");

					if(ht_search(current->symbol_table,t->child->data->token->lexeme)==NULL)
					{	
						ht_item* temp= ht_insert_var_item(current->symbol_table,t->child->data->token->lexeme ,0, t->attr->baseType, t->attr->eleType);
					}
					else
						printf("Redeclaraton of Variable found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
					
					if(t->child->right!=NULL)
					{
						t->child->right->attr->baseType=t->attr->eleType;
						t->child->right->attr->eleType=t->attr->baseType;
						checkSemRules(t->child->right,current);
					}
					
					break;
				}
				
				case 94://N3:
				{
					printf("N3\n");

					if(ht_search(current->symbol_table,t->child->data->token->lexeme)==NULL)
						ht_insert_var_item(current->symbol_table,t->child->data->token->lexeme ,0, t->attr->baseType, t->attr->eleType);
					else
						printf("Redeclaraton of Variable found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
					if(t->child->right!=NULL)
					{
						t->child->right->attr->baseType=t->attr->eleType;
						t->child->right->attr->eleType=t->attr->baseType;
						checkSemRules(t->child->right,current);
					}
					break;					
				}
	
				case 86://assignmentStmt:
				{
					printf("assignmentStmt\n");

					checkSemRules(t->child,current);	
					checkSemRules(t->child->right,current);
					if(current->isForScope==1 && (strcmp(t->child->data->token->lexeme,current->for_id)==0) && (ht_search(current->symbol_table, t->child->data->token->lexeme)==NULL))
					{
						printf("Redefinition of Loop Variable at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
						break;
					}
					if(t->child->right->data->nonterm->data->t_item->index==ht_search(mapping_table,"lvalueIDStmt")->data->t_item->index)
					{

						if(t->child->attr->baseType!=t->child->right->attr->baseType)
							printf("Type Mismatch Found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);	
					}
					else
					{
						if(t->child->attr->baseType!=3)
							printf("Variable not of Array Type at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
						else
						{
							if(t->child->attr->eleType != t->child->right->attr->baseType)
								printf("Type Mismatch Found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
						}		
					}
					break;
				}			
				case 82://var_id_num:
				{
					printf("var_id_num\n");

					checkSemRules(t->child,current);

					if(t->child->attr->baseType!=4)
					{
						if(t->child->right!=NULL && t->child->attr->baseType==3)
						{
							t->attr->baseType=t->child->attr->eleType;
							checkSemRules(t->child->right,current);
						}
						else if(t->child->right!=NULL && t->child->attr->baseType!=3)
						{
							printf("Variable not of Array Type at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
							t->attr->baseType=4;
							checkSemRules(t->child->right,current);
						}
						else
						{
							t->attr->baseType=t->child->attr->baseType;
						}
					}
					else
					{
						t->attr->baseType=4;					
					}

					//printf("LINE NUMBER =  %d  LEXEME = %s TYPE= %d\n",t->child->data->token->LN, t->child->data->token->lexeme,t->attr->baseType);
					break;
				}

				case 89://lvalueARRStmt:
				{
					printf("lvalueARRStmt\n");
					checkSemRules(t->child,current);
					if(t->child->attr->baseType!=0)
					{
						printf("Array of Index should be Integer at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);		
					}
					checkSemRules(t->child->right,current);
					t->attr->baseType=t->child->right->attr->baseType;
					break;
				}

				case 84://whichId:
				{
					printf("whichId\n");
					checkSemRules(t->child,current);
					if(t->child->attr->baseType!=0)
					{
						printf("Array of Index should be Integer at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);	
					}
					break;
				}
				case 88://lvalueIDStmt:
				case 97://newNT:
				case 107://factor:
				{
					printf("factor | lvalueIDStmt | newNT \n");
					checkSemRules(t->child,current);
					t->attr->baseType=t->child->attr->baseType;
					break;
				}

				case 96://U:
				{
					printf("U\n");
					checkSemRules(t->child->right,current);
					if(t->child->right->attr->baseType<=1)
						t->attr->baseType=t->child->right->attr->baseType;
					else
						t->attr->baseType=4;
					break;
				}

				case 100://AnyTerm:
				case 99://arithmeticOrBooleanExpr:
				case 103://arithmeticExpr:
				case 105://term:
				{
					printf("term\n");
					checkSemRules(t->child,current);
					if(t->child->right==NULL)
						t->attr->baseType=t->child->attr->baseType;
					else
					{
						checkSemRules(t->child->right,current);
						t->attr->baseType=typeCheck(t->child->right->child,t->child,t->child->right);
					}
					break;
				}
				case 102://N8:
				{
					printf("N8\n");
					checkSemRules(t->child->right,current);
					t->attr->baseType=t->child->right->attr->baseType;
					break;
				}
				case 101://N7:
				case 104://N4:
				case 106://N5:
				{
					printf("N4 | 5| 7\n");
					checkSemRules(t->child->right,current);
					if(t->child->right->right!=NULL)
					{
						checkSemRules(t->child->right->right,current);
						t->attr->baseType=typeCheck(t->child,t->child->right,t->child->right->right);
					}
					else
						t->attr->baseType=t->child->attr->baseType;
		
					break;
				}
				case 91://moduleReuseStmt:
				{
					printf("moduleReuseStmt\n");

					if(t->child->tag==1)
					{
						temp=ht_search(sym_root->symbol_table, t->child->data->token->lexeme);
						if(temp==NULL || temp->data->f_item->isDef>=0)
						{
							printf("Unknown Function at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
							break;
						}
						if(temp->data->f_item->isDef==-3)
							printf("Function with bot Def and Dec at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);

						parameters *pr= temp->data->f_item->pr;
						if(strcmp(t->child->data->token->lexeme,current->module_name)==0)
						{
							printf("Recursive Call at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
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
							break;
						}
						if(temp->data->f_item->isDef==-3)
							printf("Function with bot Def and Dec at LINE NUMBER =  %d  LEXEME = %s\n",t->child->right->data->token->LN, t->child->data->token->lexeme);

						parameters *pr= temp->data->f_item->pr;
						if(strcmp(t->child->right->data->token->lexeme,current->module_name)==0)
						{
							printf("Recursive Call at LINE NUMBER =  %d  LEXEME = %s\n",t->child->right->data->token->LN, t->child->right->data->token->lexeme);
							break;	
						}
						checkOutputList(t->child->child,current,pr);
						checkInputList(t->child->right->right,current,pr);
					}
					break;

				}
				
				case 118://iterStmt
				{
					printf("iterStmt\n");

					checkSemRules(t->child,current);
					current= insert_as_symchild(current,create_new_symnode());
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

					current=current->parent;

					break;
				}

				case 119://range
				{
					printf("range\n");

					checkSemRules(t->child,current);
					checkSemRules(t->child->right,current);
					if(t->child->data->token->val.i_val>t->child->right->data->token->val.i_val)
						printf("Error in Ranges Found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
					break;
				}
				case 113://conditionalStmt	
				{
					printf("conditionalStmt\n");

					checkSemRules(t->child,current);
					if(t->child->attr->baseType==0)
						current->switchStatus=0;
					else if(t->child->attr->baseType==2)
						current->switchStatus=2;
					else
					{
						printf("Incompatible Variable for Switch Found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
						break;
					}
					
					checkSemRules(t->child->right,current);
					
					if(t->child->right->right==NULL && current->switchStatus==0)
					{
						printf("INT Switch without Default at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
					}
					else if(t->child->right->right!=NULL && current->switchStatus==2)
					{
						printf("BOOL Switch with Default at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
					}
					
					checkSemRules(t->child->right->right,current);
					return;						
				}
			
				case 115://N9
				case 114://caseStmts
				{
					printf("caseStmts | N9\n");

					checkSemRules(t->child,current);
					if(t->child->attr->baseType!=current->switchStatus)
						printf("Invalid Case Value at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
					if(t->child->right!=NULL && t->child->right->data->nonterm->data->t_item->index==ht_search(mapping_table,"statements")->data->t_item->index)
					{
						current= insert_as_symchild(current,create_new_symnode());
						checkSemRules(t->child->right,current);
						current=current->parent;
					}
					if(t->child->right!=NULL)
						checkSemRules(t->child->right->right,current);
					
					break;
					
				}
				
				case 117://default
				{
					printf("default\n");

					current= insert_as_symchild(current,create_new_symnode());
					checkSemRules(t->child,current);
					current=current->parent;
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

void fillTheParams(astnode* t,parameters *pr)
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
				case 74://dataType
				{
					fillTheParams(t->child->right,pr);
					t->attr->baseType=3;
					t->attr->eleType=t->child->right->attr->baseType;
					break;
				}

				case 71://N1
				case 70://input_plist
				{
					fillTheParams(t->child->right,pr);
					ht_insert_var_item(pr->input_list,t->child->data->token->lexeme ,0, t->child->right->attr->baseType, t->child->right->attr->eleType);
					pr->input_types[pr->num_inputs].eleType=t->child->right->attr->eleType;
					pr->input_types[pr->num_inputs].baseType=t->child->right->attr->baseType;
					pr->num_inputs++;
					if(t->child->right->right!=NULL)
						fillTheParams(t->child->right->right,pr);

					break;
				}
				case 69://ret
				{
					fillTheParams(t->child,pr);
					break;
				}
				case 73://N2
				case 72://output_plist
				{
					fillTheParams(t->child->right,pr);
					ht_insert_var_item(pr->output_list,t->child->data->token->lexeme ,0, t->child->right->attr->baseType, t->child->right->attr->eleType);
					pr->output_types[pr->num_outputs]=t->child->right->attr->baseType;
					pr->num_outputs++;
					if(t->child->right->right!=NULL)
						fillTheParams(t->child->right->right,pr);

					break;
				}				
				default:
					break;
			
			}
		}
	}
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
				case 64://moduleDeclarations
				case 66://otherModules
				{
					modulesParHamla(t->child,root);

					astnode*rt= t->child->right;

					while(rt!=NULL)
					{
						modulesParHamla(rt,root);
						rt=rt->right;
					}
				}
				case 68://module
				{	
					temp = ht_search(root->symbol_table, t->child->data->token->lexeme);

					if(temp==NULL)
						temp=ht_insert_func_item(root->symbol_table, t->child->data->token->lexeme, 2);

					else if(temp->data->f_item->isDef==2)
					{	printf("Redefinition of function found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
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

					while(rt!=NULL)
					{
						fillTheParams(rt,temp->data->f_item->pr);
						rt=rt->right;
					}
					break;				
				}

				case 65://moduleDeclaration:
				{
					if(ht_search(root->symbol_table, t->child->data->token->lexeme)==NULL)
						ht_insert_func_item(root->symbol_table, t->child->data->token->lexeme, 1);
					else
						printf("Redeclaraton of function found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
					break;				
				}

				default: break;
			
			}
		}
	}
	return root;
}

symnode* semCheck(astnode* t)
{
	symnode* root=NULL;
	root=modulesParHamla(t,NULL);
	checkSemRules(t,root);
}


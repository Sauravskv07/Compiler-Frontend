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

symnode* checkSemRules(astnode *t,symnode* current)
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
					break;
				}

				case 6://REAL
				{
					t->attr->baseType=1;
					break;
				}

				case 7://BOOLEAN
				{
					t->attr->baseType=2;
					break;
				}
				case 3://RNUM
				{
					t->attr->baseType=1;
					break;
				}
				case 2://NUM	
				{
					t->attr->baseType=0;
					break;
				}
				case 4://ID
				{
					symnode* temp2=current;
					if(current==NULL)
						printf("Symbol Table Error\n");
					do
					{
						temp=ht_search(temp2->symbol_table, t->data->token->lexeme);
						temp2=temp2->parent;
	
					}while(temp == NULL && temp2!=NULL);

					if(temp==NULL)
					{
						t->attr->baseType=4;
						return current;
					}
					t->attr->baseType = temp->data->v_item->baseType;
					t->attr->eleType = temp->data->v_item->eleType;
					break;
				}
				case 8://TRUE
				case 9://FALSE
				{
					t->attr->baseType=2;
					break;
				}
			}
		}
		else
		{
			//moduleDef
			switch(t->data->nonterm->data->t_item->index)
			{
				case 68://module
				{	
					temp = ht_search(current->symbol_table, t->child->data->token->lexeme);

					if(temp==NULL)
						ht_insert_func_item(current->symbol_table, t->child->data->token->lexeme, 1);

					else if(temp->data->f_item->isDef==1)
						printf("Redefinition of function found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
					else
						temp->data->f_item->isDef=1;

					current= insert_as_symchild(current,create_new_symnode());
					strcpy(current->module_name, t->child->data->token->lexeme);

					astnode* rt=t->child->right;

					while(rt!=NULL)
					{
						current=checkSemRules(rt,current);
						rt=rt->right;
					}
					
					current=current->parent;
					break;					
				}

				case 67://driverModule
				{
					current= insert_as_symchild(current,create_new_symnode());
					
					if(t->child!=NULL)
						current=checkSemRules(t->child,current);
					
					current=current->parent;
					
					break;
				}
				case 65://moduleDeclaration:
				{
					if(ht_search(current->symbol_table, t->child->data->token->lexeme)==NULL)
						ht_insert_func_item(current->symbol_table, t->child->data->token->lexeme, 0);
					else
						printf("Redeclaraton of function found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
					break;				
				}
				case 74://dataType
				{
					current=checkSemRules(t->child->right,current);
					t->attr->baseType=3;
					t->attr->eleType=t->child->right->attr->baseType;

				}
				
				case 112://DeclareStmt:
				{
					current=checkSemRules(t->child->right,current);
					t->child->attr->baseType=t->child->right->attr->baseType;
					t->child->attr->eleType=t->child->right->attr->eleType;
					current=checkSemRules(t->child,current);
					break;
				}

				case 93://idList:
				{
					if(t->parent->data->nonterm->data->t_item->index==ht_search(mapping_table,"declareStmt")->data->t_item->index)
					{
						if(ht_search(current->symbol_table,t->child->data->token->lexeme)==NULL)
						{	
							ht_item* temp= ht_insert_var_item(current->symbol_table,t->child->data->token->lexeme ,0, t->attr->baseType, t->attr->eleType);
							//printf("KEY= %s DATATYPE= %d\n",temp->key,temp->data->v_item->baseType);
						}
						else
							printf("Redeclaraton of Variable found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
					
						if(t->child->right!=NULL)
						{
							t->child->right->attr->baseType=t->attr->eleType;
							t->child->right->attr->eleType=t->attr->baseType;
							t->child->right->attr->typeCheckReq=1;
							current=checkSemRules(t->child->right,current);
						}
					}
					
					break;
				}
				
				case 94://N3:
				{
					if(t->attr->typeCheckReq==1)
					{
						if(ht_search(current->symbol_table,t->child->data->token->lexeme)==NULL)
							ht_insert_var_item(current->symbol_table,t->child->data->token->lexeme ,0, t->attr->baseType, t->attr->eleType);
						else
							printf("Redeclaraton of Variable found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
						if(t->child->right!=NULL)
						{
							t->child->right->attr->baseType=t->attr->eleType;
							t->child->right->attr->eleType=t->attr->baseType;
							t->child->right->attr->typeCheckReq=1;
							current=checkSemRules(t->child->right,current);
						}
					}
					break;					
				}
	
				case 86://assignmentStmt:
				{
					current=checkSemRules(t->child,current);
					if(t->child->attr->baseType==4)
						printf("Undeclared Variable Found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
					else
					{
						current=checkSemRules(t->child->right,current);
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
					}
					break;
				}			
				case 82://var_id_num:
				{
					current=checkSemRules(t->child,current);

					if(t->child->attr->baseType!=4)
					{
						if(t->child->right!=NULL && t->child->attr->baseType==3)
						{
							t->attr->baseType=t->child->attr->eleType;
							current=checkSemRules(t->child->right,current);
						}
						else if(t->child->right!=NULL && t->child->attr->baseType!=3)
						{
							printf("Variable not of Array Type at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
							t->attr->baseType=4;
							current=checkSemRules(t->child->right,current);
						}
						else
						{
							t->attr->baseType=t->child->attr->baseType;
						}
					}
					else
					{
						printf("Undeclared Variable Found at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
						t->attr->baseType=4;					
					}

					//printf("LINE NUMBER =  %d  LEXEME = %s TYPE= %d\n",t->child->data->token->LN, t->child->data->token->lexeme,t->attr->baseType);
					break;
				}

				case 89://lvalueARRStmt:
				{
					current=checkSemRules(t->child,current);
					if(t->child->attr->baseType!=0)
					{
						printf("Array of Index should be Integer at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);		
					}
					current=checkSemRules(t->child->right,current);
					t->attr->baseType=t->child->right->attr->baseType;
					break;
				}

				case 84://whichId:
				{
					current=checkSemRules(t->child,current);
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
					current=checkSemRules(t->child,current);
					t->attr->baseType=t->child->attr->baseType;
					break;
				}

				case 96://U:
				{
					current=checkSemRules(t->child->right,current);
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
					current=checkSemRules(t->child,current);
					if(t->child->right==NULL)
						t->attr->baseType=t->child->attr->baseType;
					else
					{
						current=checkSemRules(t->child->right,current);
						t->attr->baseType=typeCheck(t->child->right->child,t->child,t->child->right);
					}
					break;
				}
				case 102://N8:
				{
					current=checkSemRules(t->child->right,current);
					t->attr->baseType=t->child->right->attr->baseType;
					break;
				}
				case 101://N7:
				case 104://N4:
				case 106://N5:
				{
					current=checkSemRules(t->child->right,current);
					if(t->child->right->right!=NULL)
					{
						current=checkSemRules(t->child->right->right,current);
						t->attr->baseType=typeCheck(t->child,t->child->right,t->child->right->right);
					}
					else
						t->attr->baseType=t->child->attr->baseType;
		
					break;
				}
				case 91://moduleReuseStmt:
				{

					//printf("HOLA %s\n",current->module_name);
					if(t->child->tag==1)
					{
						if(strcmp(t->child->data->token->lexeme,current->module_name)==0)
						{
							printf("Recursive Call at LINE NUMBER =  %d  LEXEME = %s\n",t->child->data->token->LN, t->child->data->token->lexeme);
							break;	
						}
					}
					else
					{
						//printf("child%s\n",t->child->right->data->token->lexeme);
						if(strcmp(t->child->right->data->token->lexeme,current->module_name)==0)
						{
							printf("Recursive Call at LINE NUMBER =  %d  LEXEME = %s\n",t->child->right->data->token->LN, t->child->right->data->token->lexeme);
							break;	
						}
					}
					break;

				}								

				case 63://program
				{
					current= insert_as_symchild(current,create_new_symnode());
				}

				default:
				{
					current=checkSemRules(t->child,current);

					astnode* rt=t->child->right;

					while(rt!=NULL)
					{
						current=checkSemRules(rt,current);
						rt=rt->right;
					}
				}
			}
		}
	}
	return current;
}

/*
symnode* semAction(astnode *t,symnode* current)			// All semantic rules to be written here w.r.t "t->data->nonterm" specifications.
{
	switch(t->data->nonterm->data->t_item->index)
	{

		
		case 77:		// moduleDef
		{
		}
		case 80:		//ioStmt
		{
			if(t->child->child!=NULL)
			{
				scopelist* n = var;
				while(n!=NULL && ht_search(n->top,t->child->child->data->token->lexeme)==NULL)
				{
					n = n->prev;
				}
				if(n==NULL)
				{printf("Variable %s not defined.\n",t->child->child->data->token->lexeme);}	// Add it in the error linked list. 
			}
			break;
		}
		case 91:		//moduleReuseStmt
		{
			if(ht_search(func_table,t->child->right->data->token->lexeme)==NULL)
			{printf("Function %s not defined.\n",t->child->right->data->token->lexeme);}	// Add it in the error linked list. 
			break;
		}
		case 112:		// declareStmt
		{
			ht_item *x = ht_insert_var_item(var->top, t->child->child->data->token->lexeme, 0, INTEGER, false);
			break;
		}

	}
}

*/

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

symnode* checkSemRules(astnode *t,symnode* current)
{

	if(t!=NULL)
	{
		if(t->tag==1)
		{
			switch(t->parent->data->token->index)
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
					t->attr->baseType=1;
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
					if(ht_search(current->symbol_table, t->child->data->token->lexeme)==NULL)
						ht_insert_func_item(current->symbol_table, t->child->data->token->lexeme, 1);

					else if(ht_search(current->symbol_table, t->child->data->token->lexeme)->data->f_item->isDef==1)
						printf("Redefinition of function found at LINE NUMBER =  %d  LEXEME = %s",t->child->data->token->LN, t->child->data->token->lexeme);
					else
						ht_search(current->symbol_table, t->child->data->token->lexeme)->data->f_item->isDef=1;

					current= insert_as_symchild(current,create_new_symnode());

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
						printf("Redeclaraton of function found at LINE NUMBER =  %d  LEXEME = %s",t->child->data->token->LN, t->child->data->token->lexeme);
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
					t->child->attr->baseType=t->child->right->attr->eleType;
					t->child->attr->eleType=t->child->right->attr->baseType;
					current=checkSemRules(t->child,current);
					break;
				}

				case 93://idList:
				{
					if(t->parent->data->nonterm->data->t_item->index==ht_search(mapping_table,"declareStmt")->data->t_item->index)
					{
						if(ht_search(current->symbol_table,t->child->data->token->lexeme)==NULL)
							ht_insert_var_item(current->symbol_table,t->child->data->token->lexeme ,0, t->attr->baseType, t->attr->eleType);
						else
							printf("Redeclaraton of Variable found at LINE NUMBER =  %d  LEXEME = %s",t->child->data->token->LN, t->child->data->token->lexeme);
					
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
							printf("Redeclaraton of Variable found at LINE NUMBER =  %d  LEXEME = %s",t->child->data->token->LN, t->child->data->token->lexeme);
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

		
		case 68:		// module
		{

		}
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

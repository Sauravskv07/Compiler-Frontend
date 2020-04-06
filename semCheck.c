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
#include "semCheck.h"

void checkSemRules(astnode *t)
{
	if(t!=NULL)
	{
		if(t->child==NULL)
		{
			if(t->tag==1)
			{
				return;
			}
		}
		else
		{
			semAction(t);
			checkSemRules(t->child);
			astnode* rt=t->child->right;
			while(rt!=NULL)
			{
				checkSemRules(rt);
				rt=rt->right;
			}
		}
	}
}
void semAction(astnode *t)			// All semantic rules to be written here w.r.t "t->data->nonterm" specifications.
{
	switch(t->data->nonterm->data->t_item->index)
	{
		case 68:		// module
		{
			ht_item *x = ht_insert_func_item(func_table, t->child->data->token->lexeme, 0);
			break;
		}
		case 77:		// moduleDef
		{
			scopelist* n = malloc(sizeof(scopelist));
			n->prev = var;
			n->top = ht_new();
			var = n;
			break;
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



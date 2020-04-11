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

symnode* create_new_symnode(){
	symnode* temp=malloc(sizeof(symnode));
	temp->symbol_table=ht_new();
	temp->current_offset=0;
	temp->isModuleScope=0;
	temp->module_name=NULL;
	temp->isForScope=0;
	temp->switchStatus=-1;
	temp->child=NULL;
	temp->parent=NULL;
	temp->right=NULL;
}

symnode* insert_as_symchild(symnode* parent,symnode* child){
	if(parent==NULL)
	{
		sym_root=child;
		return child;
	}

	else
	{
		if(parent->module_name!=NULL)
			strcpy(child->module_name,parent->module_name);
		symnode* prev=parent->child;
		if(prev==NULL)
		{
			parent->child=child;
			child->parent=parent;
		}
		else
		{
			symnode* temp=prev->right;
			while(temp!=NULL)
			{
				prev=temp;
				temp=temp->right;
			}
			
			child->parent=parent;
			prev->right=child;
		}
		return child;
	}
}

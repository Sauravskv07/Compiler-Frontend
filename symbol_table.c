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
	temp->num_output_defined=0;
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

int nestlevel = 0;
int off = 0;

void printsymnodes(symnode* root)
{
	if(root!=NULL)
	{
		if(root->parent == sym_root) {off = 0;}
		if(root == sym_root)
		{
			printhashparams();
		}
		else
		{
		printhashtable(root->module_name,-1,-1,root->symbol_table);
		nextSt(0);
		nextEn();
		}
		nestlevel = nestlevel + 1;
		printsymnodes(root->child);
		symnode* rt = root->child;
		while(rt!=NULL)
		{
			rt = rt->right;
			printsymnodes(rt);
		}
		nestlevel = nestlevel -1;
	}
}
void printsymnode(symnode* rt)
{
	st = root;
	en = root;
	nextSt(1);
	nextEn();
	printf("\n Please note that since entire activation record is allocated at function call,\n");
	printf(" the variables allocation may have different order since they are fetched from hash table.\n");
	printf(" Therefore offsets calcualtions are according to the order of allocation.\n");
	printf(" You can check that by seeing that offset increments by the width value and \n");
	printf(" and total memory of the activation record.\n\n");
	printf("Name\t\tModule\t\tLine\tWidth\tisArray\tStatic/Dynamic\tRange\tType\tOffset\tNesting level\t\n");
	printsymnodes(rt);
}
void printhashtable(char *module_name, int linstart, int linend, ht_hash_table* ht)
{
	const char *tp[] = {"INTEGER","REAL","BOOLEAN","ARRAY","UNDEFINED"};
	int width[] = {2,4,1,1,0};
	if(module_name==NULL) {module_name="driver";}
	for(int i=0;i<MAX_SIZE_MAPPING_TABLE;i++)
	{
		if(ht->items[i]!=NULL)
		{
			char *type = "";
			char *isArr = "";
			char *statdy = "---";
			char *range = "---";
			int wd = width[ht->items[i]->data->v_item->baseType];
			char a[5];char b[5];
			if(linstart==-1){linstart=st->data->token->LN;}
			if(linend==-1){linend=en->data->token->LN;}
			sprintf(a, "%d", linstart);
			sprintf(b, "%d", linend);
			char *lin = "---";
			lin = a;
			lin = strcat(lin,",");
			lin = strcat(lin,b);
			if(ht->items[i]->data->v_item->baseType==3) 
			{
				statdy = "static";
				char a[5];char b[5];
				type = tp[ht->items[i]->data->v_item->eleType]; isArr = "yes";
				if(ht->items[i]->data->v_item->lowNode!=NULL)
				{statdy="dynamic"; range = ht->items[i]->data->v_item->lowNode->key;}
				else {sprintf(a, "%d", ht->items[i]->data->v_item->low); range = a;}
				if(ht->items[i]->data->v_item->highNode!=NULL)
				{statdy="dynamic";range = ht->items[i]->data->v_item->highNode->key;}
				else {sprintf(b, "%d", ht->items[i]->data->v_item->high); range =  strcat(range,",");range =  strcat(range,b);}
				if(ht->items[i]->data->v_item->lowNode==NULL && ht->items[i]->data->v_item->highNode==NULL)
				{wd = wd + (ht->items[i]->data->v_item->high - ht->items[i]->data->v_item->low +1)*width[ht->items[i]->data->v_item->eleType];;}
			}
			else {type = tp[ht->items[i]->data->v_item->baseType]; isArr = "no";}
			printf("%s\t\t%s\t\t%s\t%d\t%s\t%s\t\t%s\t%s\t%d\t%d\t\n",ht->items[i]->key,module_name,lin,wd,isArr,statdy,range,type,off,nestlevel);
			off = off + wd;
		}
	}
}
void printhashparams()
{
	ht_hash_table* ht = sym_root->symbol_table;
	for(int i=0;i<MAX_SIZE_MAPPING_TABLE;i++)
	{
		if(ht->items[i]!=NULL)
		{
			if(ht_search(sym_root->symbol_table,ht->items[i]->key)->data->f_item->pr!=NULL)
			{
			ht_hash_table* ht2 = ht_search(sym_root->symbol_table,ht->items[i]->key)->data->f_item->pr->input_list;
			printhashtable(ht->items[i]->key, getFunclinSt(ht->items[i]),getFunclinEn(ht->items[i]), ht2);
			ht2 = ht_search(sym_root->symbol_table,ht->items[i]->key)->data->f_item->pr->output_list;
			printhashtable(ht->items[i]->key, getFunclinSt(ht->items[i]),getFunclinEn(ht->items[i]), ht2);
			}
			off = 0;
		}
	}
}
int getFunclinSt(ht_item* ht)
{
	treenode * p = root;
	bool flag = false;
	while(p!=NULL)
	{
		if(p->child==NULL && strcmp(ht->key, p->data->token->lexeme)==0)
		{
			flag = true;
		}
		if(flag ==true && p->child==NULL && strcmp("start", p->data->token->lexeme)==0)
		{
			return p->data->token->LN;
		}
		
		if(p->child!=NULL){p = p->child;continue;}
		while(p!=NULL && p->right==NULL)
		{
			p=p->parent;
		}
		if(p==NULL) return 0;
		p = p->right;
	}
}
int getFunclinEn(ht_item* ht)
{
	treenode * p = root;
	bool flag = false;
	int i = 0;
	while(p!=NULL)
	{
		if(p->child==NULL && strcmp(ht->key, p->data->token->lexeme)==0)
		{
			flag = true;
		}
		if(flag ==true && p->child==NULL && strcmp("start", p->data->token->lexeme)==0)
		{
			i = i + 1;
		}
		if(flag ==true && p->child==NULL && strcmp("end", p->data->token->lexeme)==0)
		{
			i = i - 1; 
			if(i==0) return p->data->token->LN;
		}
		
		if(p->child!=NULL){p = p->child;continue;}
		while(p!=NULL && p->right==NULL)
		{
			p=p->parent;
		}
		if(p==NULL) return 0;
		p = p->right;
	}
}
int nextSt(int i)
{
	treenode * p = st;
	bool flag=false;
	if(i==1) flag=true;
	while(p!=NULL)
	{
		if(p->child==NULL && strcmp("start", p->data->token->lexeme)==0)
		{
			if(flag==true)
			{st = p;
			return 0;}
			flag = true;
		}
		
		if(p->child!=NULL){p = p->child;continue;}
		while(p!=NULL && p->right==NULL)
		{
			p=p->parent;
		}
		if(p==NULL) return 0;
		p = p->right;
	}
}
int nextEn()
{
	treenode * p = st;
	int i = 0;
	while(p!=NULL)
	{
		if(p->child==NULL && strcmp("start", p->data->token->lexeme)==0)
		{
			i = i + 1;
		}
		if(p->child==NULL && strcmp("end", p->data->token->lexeme)==0)
		{
			i = i - 1; 
			if(i==0) {en=p; return 0; }
		}
		
		if(p->child!=NULL){p = p->child;continue;}
		while(p!=NULL && p->right==NULL)
		{
			p=p->parent;
		}
		if(p==NULL) return 0;
		p = p->right;
	}
}







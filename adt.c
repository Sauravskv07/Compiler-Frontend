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

stack *pop(stack *st)
{
	if(st==NULL)
	{return NULL;}
	
	stack *t = st;
	st = st->prev;
	if(st!=NULL)
	st->next = NULL;

	free(t);

	return st;
}
stack *push(stack *st,ht_item *b)
{
	stack *d = malloc(sizeof(stack));
	d->data = b;
	d->prev = st;
	st->next = d;
	st = st->next;
	return st;
}
ht_item *peek(stack *st)
{
	if(st==NULL)
	{return NULL;}
	return st->data;
}

treenode *insertAsChild(treenode *parent, node *child, Type tag)
{
/*
	printf("Inserting as Child\n");
	if(parent!=NULL)
		if(parent->tag==1)
			printf("Parent = %s \n",parent->data->token->lexeme);
		else
			printf("Parent = %s \n",parent->data->nonterm->key);

	if(child!=NULL)
		if(tag==1)
			printf("Child = %s \n",child->token->lexeme);
		else
			printf("Child = %s \n",child->nonterm->key);

*/
	if(child==NULL)
	{return NULL;}
	treenode *t = malloc(sizeof(treenode));
	t->data = child;
	t->tag = tag;
	t->right = NULL;
	t->parent = parent;
	t->child = NULL;
	if(parent==NULL)
	{
		//printf("Inserted Root %s\n",child->nonterm->key);
		root=t;}
	else
	parent->child = t;
	return t;
}
treenode *insertAsNextRightSibling(treenode *child, node *right, Type tag)
{
/*
	printf("Inserting as Right\n");
	if(child!=NULL)
		if(child->tag==1)
			printf("Child = %s \n",child->data->token->lexeme);
		else
			printf("Child = %s \n",child->data->nonterm->key);

	if(right!=NULL)
		if(tag==1)
			printf("Right = %s \n",right->token->lexeme);
		else
			printf("Right = %s \n",right->nonterm->key);

*/
	if(child==NULL||right==NULL||child->right!=NULL)
	{return NULL;}
	treenode *t = malloc(sizeof(treenode));
	t->data = right;
	t->tag = tag;
	t->right = NULL;
	t->child = NULL;
	t->parent = child->parent;
	child->right = t;
	return t;
	 
}
treenode *getRoot(treenode *child)
{
	treenode *t = child;
	if(t==NULL)
	{return NULL;}
	while(t->parent!=NULL)
	{
		t=t->parent;
	}
	return t;
}
void printTraversal(treenode *t,FILE* fp)
{
	if(t!=NULL)
	{
		if(t->child==NULL)
		{
			if(t->tag==1)
			{
				printf(" %s ",t->data->token->lexeme);
				if(t->data->token->index==ht_search(mapping_table,"RNUM")->index)
					fprintf(fp,"%s\t%d\t\t%s\t\t\t\t%f\t%s\t\tYES\n",t->data->token->lexeme,t->data->token->LN,tokensList[t->data->token->index]->key,t->data->token->val.f_val,t->parent->data->nonterm->key);
				else
					fprintf(fp,"%s\t\t%d\t\t%s\t\t\t\t%d\t\t%s\t\tYES\n",t->data->token->lexeme,t->data->token->LN,tokensList[t->data->token->index]->key,t->data->token->val.i_val,t->parent->data->nonterm->key);
			}
			else
			{
				printf(" %s ",t->data->nonterm->key);

				fprintf(fp,"%s\t\t\t\tEPSILON\t\t\t\t\t\t%s\t\tYES\t\n",t->data->nonterm->key,t->parent->data->nonterm->key);
			}
		}
		else
		{
			printTraversal(t->child,fp);

			printf("%s --",t->data->nonterm->key);

			if(t->parent!=NULL)
				fprintf(fp,"----\t\t\t\t\t\t\t\t\t\t%s\t\t\tNO\t\t%s\n",t->parent->data->nonterm->key,t->data->nonterm->key);
			else
				fprintf(fp,"----\t\t\t\t\t\t\t\t\t\tROOT\t\t\tNO\t\t%s\n",t->data->nonterm->key);

			treenode* rt=t->child->right;
			while(rt!=NULL)
			{
				printTraversal(rt,fp);
				rt=rt->right;
			}
		}
	}
}
